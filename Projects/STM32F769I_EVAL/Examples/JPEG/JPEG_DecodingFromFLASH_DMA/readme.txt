/**
  @page JPEG_DecodingFromFLASH_DMA : JPEG example : JPEG decoding with DMA of an image stored in FLASH example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    JPEG/JPEG_DecodingFromFLASH_DMA/readme.txt 
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    30-December-2016
  * @brief   Description of the JPEG decoding from FLASH with DMA example.
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
   @endverbatim

@par Example Description

This example demonstrates how to decode a JPEG image stored in the internal FLASH 
using the JPEG HW decoder in DMA mode and display the final ARGB8888 image on KoD LCD 
mounted on board or a HDMI monitor Connected through the DSI-HDMI bridge board MB1232.A.

The example can automatically detect if the KoD LCD is connected or if the DSI-HDMI 
bridge is connected to the DSI connector (CN15), if no DSI slave is connected the RED LED will turn On.


The example performs the following actions :
  - Init the JPEG decoder.
  - Init and configure the LCD_DSI screen
  - Call the function "JPEG_Decode_DMA" to start decoding the jpg buffer with DMA (Non blocking method).
    Function "JPEG_Decode_DMA" is implemented in a separate module decode_dma.c.
    This module is a wrapper on top of the JPEG HAL driver
  
  - Once the decoding ends :
    - Call HAL routine "HAL_JPEG_GetInfo" to retrieve the image parameters :
      image width, image height, image quality (from 1% to 100%) color space and Chroma sampling.      
    - copy the ARGB8888 result buffer to the LCD Frame buffer for display:
        Using the DMA2D and positioning the image in the center of the LCD screen.

@note The jpeg HW decoder outputs are organized in YCbCr blocks. 
      The jpeg utilities functions implemented in "jpeg_utils.c" file are used to convert these YCbCr blocks 
      to RGB pixels to the destination JPEG output buffer.
      The JPEG utilities files allowing to convert YCbCr blocks to RGB pixels are available under \Utilities\JPEG

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this example uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.
      
@note In case of EVAL's LCD use in example code, the DATA_IN_ExtSDRAM define
      need to be added  in the project preprocessor to avoid SDRAM multiple configuration
     (the LCD uses SDRAM as frame buffer, and its configuration is done by the BSP_SDRAM_Init()). 

@note The compiler optimizations level could be set to High/Balanced giving a good tradeoff between the code size and the performance (decode time).

@par Keywords

Display, Graphic, JPEG, Decoding, Flash memory, DMA, ARGB8888, Hardware decoding, LCD, HDMI, YCbCr, RGB,
      
@Note  If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),
       it is recommended to configure the latters as Write Through.
       This is ensured by configuring the memory attributes at MPU level in order to ensure cache coherence on SRAM1 and SRAM2.
       Please, refer to Template project for a typical MPU configuration.

@Note  If external memory is shared between several processors, it is recommended to configure it as Write Back (bufferable), shareable and cacheable.
       The memory base address and size must be properly updated.
       The user needs to manage the cache coherence at application level.

For more details about the MPU configuration and use, please refer to AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”

@par Directory contents

  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/stm32f7xx_hal_conf.h          HAL configuration file
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/stm32f7xx_it.h                Interrupt handlers header file
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/main.h                        Header for main.c module  
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/decode_dma.h                  Header for decode_dma.c module   
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/image_320_240_jpg.h           JPEG Image 320x240 to be decoded and displayed on LCD
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/jpeg_utils_conf.h             JPEG utilties module configuration file
  
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/stm32f7xx_it.c                Interrupt handlers
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/main.c                        Main program
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/decode_dma.c                  JPEG Decoding (from memory) with DMA module
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/stm32f7xx_hal_msp.c           HAL MSP module   
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/system_stm32f7xx.c            STM32F7xx system source file  
  

@par Hardware and Software environment

  - This example runs on STM32F769xx/STM32F779xx devices.
    
  - This example has been tested with STMicroelectronics STM32F769I_EVAL revB 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
    
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Mount either the KoD LCD pannel (Board MB1166 rev.A) or the DSI-HDMI bridge(Board MB1232 Rev.A)
   to the DSI connector (CN15). 
 - if using the DSI-HDMI bridge then connect a HDMI monitor using a HDMI cable to the 
   MB1232 board.  
 
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */