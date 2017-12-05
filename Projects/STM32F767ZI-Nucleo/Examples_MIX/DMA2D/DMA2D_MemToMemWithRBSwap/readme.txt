/**
  @page DMA2D_MemToMemWithRBSwap DMA2D : DMA2D Memory to Memory with R&B swap example (HAL/LL mixed usage example)
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_MIX/DMA2D/DMA2D_MemToMemWithRBSwap/readme.txt
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    30-December-2016
  * @brief   Description of the DMA2D_MemToMemWithRBSwap example.
  ******************************************************************************
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
  Memory to Memory transfer mode with Pixel Format Conversion and images blending
  then display the result on LCD, in resorting to DMA2D LL APIs for performance improvement.

  The source buffer in format ARGB8888 and size 128x160 located in Flash is copied
  in Memory to Memory mode with Pixel Format Conversion to create three
  different images:
  - the first one is an artificially distorted image (converted in ARGB1555 but read as RGB565)
  - the second one is converted to RGB565 but Red and Blue are swapped for illustration purposes
  - the third one is converted to RGB565 with the correct color setting

  These three images are alternatively shown on the LCD, blended two by two so that each
  image smoothly gives way to the next one.
  The last image finally fades away to let a white screen only.
  Then the same process is repeated in an infinite loop.


  In this basic example, the goal is to show how to set some of the DMA2D IP
  parameters thru DMAD LL APIs:
  - input and output image format, alpha value
  - origin and destination buffers addresses
  - foreground and background addresses
  - transfer size
  - pixel format conversion, blending
  - DMA2D interruptions enabling
  - DMA2D transfer start 

  NUCLEO-F767ZI board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the DMA2D transfer is complete.
 - LED2 is ON when there is a DMA2D transfer error.
 - LED3 is ON if the adafruit shield is not detected.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, Graphic, DMA2D, Memory to memory, LCD, ARGB8888, Red Blue swap, PFC

@Note  If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),
       it is recommended to configure the latters as Write Through.
       This is ensured by configuring the memory attributes at MPU level in order to ensure cache coherence on SRAM1 and SRAM2.
       Please, refer to Template project for a typical MPU configuration.

@Note  If external memory is shared between several processors, it is recommended to configure it as Write Back (bufferable), shareable and cacheable.
       The memory base address and size must be properly updated.
       The user needs to manage the cache coherence at application level.

For more details about the MPU configuration and use, please refer to AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”

@par Directory contents 

  - DMA2D/DMA2D_MemToMemWithRBSwap/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - DMA2D/DMA2D_MemToMemWithRBSwap/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - DMA2D/DMA2D_MemToMemWithRBSwap/Inc/main.h                  Header for main.c module
  - DMA2D/DMA2D_MemToMemWithRBSwap/Inc/ARGB8888_128x160.h      ARGB8888 Image binary
  - DMA2D/DMA2D_MemToMemWithRBSwap/Src/stm32f7xx_it.c          Interrupt handlers
  - DMA2D/DMA2D_MemToMemWithRBSwap/Src/main.c                  Main program
  - DMA2D/DMA2D_MemToMemWithRBSwap/Src/stm32f7xx_hal_msp.c     HAL MSP module 
  - DMA2D/DMA2D_MemToMemWithRBSwap/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F767xx devices.
  
  - This example has been tested with STMicroelectronics NUCLEO-F767ZI 
    board and can be easily tailored to any other supported device 
    and development board. 

  - Adafruit 1.8" TFT shield must be connected on CN7, CN8, CN9 and CN10 Arduino connectors,
    for more details please refer to UM1726.

For more details about the adafruit 1.8" TFT shield, please visit: 
http://www.adafruit.com/blog/2012/04/26/new-product-adafruit-1-8-18-bit-color-tft-shield-wmicrosd-and-joystick/

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
