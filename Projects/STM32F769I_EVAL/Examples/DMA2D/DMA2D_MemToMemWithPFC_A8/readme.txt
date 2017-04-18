/**
  @page DMA2D_MemToMemWithPFC_A8 DMA2D Memory to Memory with PFC and A8 color mode example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    DMA2D/DMA2D_MemToMemWithPFC_A8/readme.txt 
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    30-December-2016
  * @brief   Description of the DMA2D Memory to Memory with PFC and A8 color mode.
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

  This example provides a description of how to configure DMA2D peripheral in 
  Memory_to_Memory with Pixel Format Conversion and A8 color mode, and display the result on LCD.
  
  The DAM2D source buffer is in A8 format and size 320x240 located in internal SRAM.
  This buffer is filled by the example to provide a transparency index table where each byte represents 
  the transparency of a pixel in the final result image.  
  the A8 source buffer is constructed in a way where values of the image first row are set to transparency = 15
  then increment the transparency for each row which gives a degraded transparency effect.
  
  The DMA2D destination buffer is located in SDRAM with color mode ARGB8888 used as the LCD display frame buffer.

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  Then the SystemClock_Config() function is used to configure the system
  clock (SYSCLK) to run at 200 MHz.
 
  In this basic example the goal is to explain how to use the DMA2D with A8 color format 
  to create a fixed color image with indexed transparency.

  First the LCD_DSI screen is initialized to display the LCD_FRAME_BUFFER using LTDC layer 1.
  
  The A8 transparency index table is then constructed as explained above .
  User can change the example to bypass this stage and use a constant A8 transparency table if available, 
  or change the transparency index table construction to make any transparency effect he desires.   
  
  Then in a while loop :
  
    - The DMA2D is configured to memory to memory with Pixel format conversion.
      Destination color mode is set to ARB8888, source color mode is set to A8 mode (indexed transparency)
      Parameter InputAlpha is set to 0x0000FF00 for a green fixed color.
      Parameter RedBlueSwap is set to DMA2D_RB_REGULAR i.e do not swap Red and blue colors
      And parameter AlphaInverted is set either to DMA2D_REGULAR_ALPHA or DMA2D_INVERTED_ALPHA
      DMA2D_REGULAR_ALPHA : keep the Alpha values given in tha A8 transparency index table.
      DMA2D_INVERTED_ALPHA : invert the  Alpha values given in tha A8 transparency index table before applying it
      example : when DMA2D_INVERTED_ALPHA is used , if alpha = 0x11 for a given pixel 
                then applied value for this pixel is 0xFF - 0x11 = 0xEE
 
   - After the DMA2D configuration , start the transfer. 
     The result , a green 320x240 rectangle with degraded transparency effect is displayed on the LCD.
     Wait for 2 sec, change the DMA2 config (AlphaInverted parameter) then go to the next loop.
  
 => Output offset is the number of pixel to be skipped after each transferred line
                      
 how to calculate the size of the transferred data ? 
 => selected destination color mode gives the number of bits per pixel and we have 
    (ARGB8888 => 32bits/pixel) the number of pixel per line and the number of line, 
    therefore :    
    data_size = (bits per pixel) X (pixel per line + output offset) X (number of line)

 how to calculate the size of the source A8 buffer data ? 
 => As source buffer is in A8 format , each byte represents the transparency for a pixel of the final result buffer. 
    Giving the number of pixel per line and the number of line :    
    A8_data_size (in bytes) = (pixel per line) X (number of line) 
       
STM32F769I-EVAL board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the DMA2D transfer is complete.
 - LED2 is ON when there is a DMA2D transfer error.
 - LED3 is ON when there is an error in LTDC transfer/Init process.
 
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this example uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.

@par Keywords

Display, Graphic, DMA2D, LCD, SRAM, ARGB8888, Memory to memory, LTDC, Pixel Format Conversion, A8

@Note  If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),
       it is recommended to configure the latters as Write Through.
       This is ensured by configuring the memory attributes at MPU level in order to ensure cache coherence on SRAM1 and SRAM2.
       Please, refer to Template project for a typical MPU configuration.

@Note  If external memory is shared between several processors, it is recommended to configure it as Write Back (bufferable), shareable and cacheable.
       The memory base address and size must be properly updated.
       The user needs to manage the cache coherence at application level.

For more details about the MPU configuration and use, please refer to AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”

@par Directory contents

    - DMA2D/DMA2D_MemToMemWithPFC_A8/Inc/main.h                Main configuration file
    - DMA2D/DMA2D_MemToMemWithPFC_A8/Inc/stm32f7xx_it.h        Interrupt handlers header file
    - DMA2D/DMA2D_MemToMemWithPFC_A8/Inc/stm32f7xx_hal_conf.h  HAL configuration file
    - DMA2D/DMA2D_MemToMemWithPFC_A8/Src/main.c                Main program  
    - DMA2D/DMA2D_MemToMemWithPFC_A8/Src/stm32f7xx_it.c        Interrupt handlers
    - DMA2D/DMA2D_MemToMemWithPFC_A8/Src/stm32f7xx_hal_msp.c   HAL MSP module
    - DMA2D/DMA2D_MemToMemWithPFC_A8/Src/system_stm32f7xx.c    STM32F7xx system clock configuration file

@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This example has been tested with STM32F769I-EVAL board and can be
    easily tailored to any other supported device and development board. 


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
                                   
