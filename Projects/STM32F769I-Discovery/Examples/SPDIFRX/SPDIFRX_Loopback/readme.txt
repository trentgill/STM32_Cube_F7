/**
  @page SPDIFRX SPDIFRX_Loopback  Description of the SPDIFRX_Loopback example

  @verbatim
  ******************************************************************************
  * @file    SPDIFRX/SPDIFRX_Loopback/readme.txt
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    30-December-2016 
  * @brief   Description of the SPDIFRX_Loopback example.
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

This example shows how to use the SPDIFRX HAL APIs to receive a data buffer
in polling mode.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 216 MHz.

This example aims to explain the basic initialization procedure of the SPDIFRX
Peripheral. Hence, it is limited to a basic data buffer tranmission between SPDIF_TX
(SAI configured in SPDIF-Tx mode) and SPDIFRX on the same board.

The SPDIFRX peripheral's configuration is ensured by HAL_SPDIFRX_Init() function.
The latter is calling HAL_SPDIFRX_MspInit() function who's implementing
the configuration of the needed SPDIFRX resources according to the used hardware (CLOCK).
User can update HAL_SPDIFRX_Init() input parameters to change the SPDIFRX configuration.

STM32F769I_Discovery LEDs are used to monitor the data transfer's status:
  - LED2 (GREEN) is ON when a correct buffer is received
  - LED1 (RED) is ON when a communication or an initialization error occurs.

@par Keywords

Communication, Audio, Spdif, Rx, S/PDIF, IEC-60958, IEC-61937, Loopback, Polling, SAI, SPDIF-Tx,
Data Transfer

@Note  If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),
       it is recommended to configure the latters as Write Through.
       This is ensured by configuring the memory attributes at MPU level in order to ensure cache coherence on SRAM1 and SRAM2.
       Please, refer to Template project for a typical MPU configuration.

@Note  If external memory is shared between several processors, it is recommended to configure it as Write Back (bufferable), shareable and cacheable.
       The memory base address and size must be properly updated.
       The user needs to manage the cache coherence at application level.

For more details about the MPU configuration and use, please refer to AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”

@par Directory contents

  - SPDIFRX/SPDIFRX_Loopback/Src/main.c                  Main program
  - SPDIFRX/SPDIFRX_Loopback/Src/system_stm32f7xx.c      STM32F7xx system source file
  - SPDIFRX/SPDIFRX_Loopback/Src/stm32f7xx_it.c          Interrupt handlers
  - SPDIFRX/SPDIFRX_Loopback/Src/stm32f7xx_hal_msp.c     HAL MSP module
  - SPDIFRX/SPDIFRX_Loopback/Inc/main.h                  Main program header file
  - SPDIFRX/SPDIFRX_Loopback/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - SPDIFRX/SPDIFRX_Loopback/Inc/stm32f7xx_it.h          Interrupt handlers header file

@par Hardware and Software environment

  - This example runs on STM32F769xx/STM32F779xx/STM32F777xx/STM32F767xx devices.

  - This example has been tested with STMicroelectronics STM32F769I_Discovery
    board and can be easily tailored to any other supported device
    and development board.

  - STM32F769I_Discovery Set-up :
    - Connect SPDIF_Tx (CN8) to SPDIF_Rx (CN12) with an AV cable.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
