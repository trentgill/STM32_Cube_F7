/**
  ******************************************************************************
  * @file    USB_Host/DynamicSwitch_Standalone/Src/usbh_diskio.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    30-December-2016
  * @brief   USB Key Disk I/O driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "ffconf.h"	
#include "diskio.h"
#include "usbh_msc.h"
#include "ff_gen_drv.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern USBH_HandleTypeDef  hUSBHost;

/* Private function prototypes -----------------------------------------------*/
DSTATUS USBH_initialize(BYTE lun);
DSTATUS USBH_status(BYTE lun);
DRESULT USBH_read(BYTE lun, BYTE *buff, DWORD sector, UINT count);

#if _USE_WRITE == 1
  DRESULT USBH_write(BYTE lun, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT USBH_ioctl(BYTE lun, BYTE, void*);
#endif /* _USE_IOCTL == 1 */
  
Diskio_drvTypeDef  USBH_Driver =
{
  USBH_initialize,
  USBH_status,
  USBH_read , 
#if  _USE_WRITE == 1
  USBH_write,
#endif /* _USE_WRITE == 1 */  
#if  _USE_IOCTL == 1
  USBH_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  None
  * @retval DSTATUS: Operation status
  */
DSTATUS USBH_initialize(BYTE lun)
{
  return RES_OK;
}

/**
  * @brief  Gets Disk Status
  * @param  None
  * @retval DSTATUS: Operation status
  */
DSTATUS USBH_status(BYTE lun)
{
  DRESULT res = RES_ERROR;
  
  if(USBH_MSC_UnitIsReady(&hUSBHost, 0))
  {
    res = RES_OK;
  }
  else
  {
    res = RES_ERROR;
  }
  
  return res;
}

/**
  * @brief  Reads Sector(s) 
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT USBH_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_ERROR;
  MSC_LUNTypeDef info;
  USBH_StatusTypeDef  status = USBH_OK;
  DWORD scratch [_MAX_SS / 4];
  
  if ((DWORD)buff & 3) /* DMA Alignment issue, do single up to aligned buffer */
  {
    while ((count--)&&(status == USBH_OK))
    {
      status = USBH_MSC_Read(&hUSBHost, 0, sector + count, (uint8_t *)scratch, 1);
      if(status == USBH_OK)
      {
        memcpy (&buff[count * _MAX_SS] ,scratch, _MAX_SS);
      }
      else
      {
        break;
      }
    }
  }
  else
  {
    status = USBH_MSC_Read(&hUSBHost, 0, sector, buff, count);
  }
  
  if(status == USBH_OK)
  {
    res = RES_OK;
  }
  else
  {
    USBH_MSC_GetLUNInfo(&hUSBHost, 0, &info); 
    
    switch (info.sense.asc)
    {
    case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
    case SCSI_ASC_MEDIUM_NOT_PRESENT:
    case SCSI_ASC_NOT_READY_TO_READY_CHANGE: 
      USBH_ErrLog ("USB Disk is not ready!");  
      res = RES_NOTRDY;
      break; 
      
    default:
      res = RES_ERROR;
      break;
    }
  }
  
  return res;
}

/**
  * @brief  Writes Sector(s)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT USBH_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_ERROR; 
  MSC_LUNTypeDef info;
  USBH_StatusTypeDef  status = USBH_OK;  
  DWORD scratch [_MAX_SS / 4];  
  
  if ((DWORD)buff & 3) /* DMA Alignment issue, do single up to aligned buffer */
  {
    while (count--)
    {
      memcpy (scratch, &buff[count * _MAX_SS], _MAX_SS);
      
      status = USBH_MSC_Write(&hUSBHost, 0, sector + count, (BYTE *)scratch, 1);
      if(status == USBH_FAIL)
      {
        break;
      }
    }
  }
  else
  {
    status = USBH_MSC_Write(&hUSBHost, 0, sector, (BYTE *)buff, count);
  }
  
  if(status == USBH_OK)
  {
    res = RES_OK;
  }
  else
  {
    USBH_MSC_GetLUNInfo(&hUSBHost, 0, &info); 
    
    switch (info.sense.asc)
    {
    case SCSI_ASC_WRITE_PROTECTED:
      USBH_ErrLog("USB Disk is Write protected!");
      res = RES_WRPRT;
      break;
      
    case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
    case SCSI_ASC_MEDIUM_NOT_PRESENT:
    case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
      USBH_ErrLog("USB Disk is not ready!");      
      res = RES_NOTRDY;
      break; 
      
    default:
      res = RES_ERROR;
      break;
    }
  }
  
  return res;   
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT USBH_ioctl(BYTE lun, BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;
  MSC_LUNTypeDef info;
  
  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC: 
    res = RES_OK;
    break;
    
  /* Get number of sectors on the disk (DWORD) */  
  case GET_SECTOR_COUNT : 
    if(USBH_MSC_GetLUNInfo(&hUSBHost, 0, &info) == USBH_OK)
    {
      *(DWORD*)buff = info.capacity.block_nbr;
      res = RES_OK;
    }
    else
    {
      res = RES_ERROR;
    }
    break;
    
  /* Get R/W sector size (WORD) */  
  case GET_SECTOR_SIZE :	
    if(USBH_MSC_GetLUNInfo(&hUSBHost, 0, &info) == USBH_OK)
    {
      *(DWORD*)buff = info.capacity.block_size;
      res = RES_OK;
    }
    else
    {
      res = RES_ERROR;
    }
    break;
    
    /* Get erase block size in unit of sector (DWORD) */ 
  case GET_BLOCK_SIZE : 
    
    if(USBH_MSC_GetLUNInfo(&hUSBHost, 0, &info) == USBH_OK)
    {
      *(DWORD*)buff = info.capacity.block_size;
      res = RES_OK;
    }
    else
    {
      res = RES_ERROR;
    }
    break;
    
  default:
    res = RES_PARERR;
  }
  
  return res;
}
#endif /* _USE_IOCTL == 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/