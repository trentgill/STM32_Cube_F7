/**
  ******************************************************************************
  * @file    GPS/GPS_Map_Tracker/src/touchscreen.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    30-December-2016
  * @brief   This example code shows how to use the touchscreen driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup GPS_Map_Tracker
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/ 
/* Private Structures and Enumerations ---------------------------------------*/
/* Global variables ---------------------------------------------------------*/
TS_StateTypeDef  TS_State = {0};

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  TouchScreen get touch position
  * @param  None
  * @retval None
  */
uint8_t TouchScreen_GetTouchButtonPosition(void)
{
  uint16_t x1, y1;
  uint8_t buttonnum = 0;

        /* Check in polling mode in touch screen the touch status and coordinates */
        /* of touches if touch occurred                                           */
        BSP_TS_GetState(&TS_State);
        if(TS_State.touchDetected)
        {
          /* One or dual touch have been detected          */
          /* Only take into account the first touch so far */

          /* Get X and Y position of the first */          
          x1 = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
          y1 = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);

      
         
          if (y1 > 200)
          {
            if ((x1 > 170) && (x1 < 240))
            {
    
              buttonnum = 1; /* freq+ */
            }
          }
          else
          {
            buttonnum = 0xFF;
          }
        } /* of if(TS_State.TouchDetected) */
   return buttonnum;
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
