/**
  @page FLASH_SwapBank  FLASH Swap Bank example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    FLASH/FLASH_SwapBank/readme.txt
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    30-December-2016
  * @brief   Description of the FLASH Swap Bank example.
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

This example guides you through the different configuration steps by mean of HAL API
how to swap execution between bank1 and bank2 of the STM32F7xx internal FLASH memory mounted on STM32F769I-EVAL.
  
At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) 
to run at 216 MHz. 

Below are the steps to run this example:
1- Choose the FLASH_SwapBank_Bank2 project and generate its binary (ie: FLASH_SwapBank.bin)

2- Configure the FLASH memory at Dual Bank mode using STM32 ST-LINK Utilities (Target -> Options Bytes: 
   "nDBANK" should be unchecked, "nDBOOT" should be checked).
   Then, Load FLASH_SwapBank.bin binary at the bank2 of the flash (at the address 0x08100000) using STM32 
   ST-LINK Utilities(www.st.com) or any similar tool.   

@note:
 - You can avoid step 1 by directly loading the binary file provided with the example
 - You have to configure your preferred toolchain in order to generate the binary
   file after compiling the project.
   
3- Choose the FLASH_SwapBank_Bank1 project and run it, this project will be loaded
   in the bank1 of the flash: at the address 0x08000000
   
4- Click the BUTTON_TAMPER button to swap between the two banks,

- If program is executing from bank1 is selected, LED1 toggles,

- If program is executing from bank2 is selected, LED2 toggles,

- If error occurs, LED3 is turn on.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Memory, Flash, Swap bank, Dual Bank, Execute

@Note  If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),
       it is recommended to configure the latters as Write Through.
       This is ensured by configuring the memory attributes at MPU level in order to ensure cache coherence on SRAM1 and SRAM2.
       Please, refer to Template project for a typical MPU configuration.

@Note  If external memory is shared between several processors, it is recommended to configure it as Write Back (bufferable), shareable and cacheable.
       The memory base address and size must be properly updated.
       The user needs to manage the cache coherence at application level.

For more details about the MPU configuration and use, please refer to AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”

@par Directory contents 

  - FLASH/FLASH_SwapBank/Inc/stm32f7xx_hal_conf.h        HAL Configuration file  
  - FLASH/FLASH_SwapBank/Inc/stm32f7xx_it.h              Header for stm32f7xx_it.c
  - FLASH/FLASH_SwapBank/Inc/main.h                      Header for main.c module 
  - FLASH/FLASH_SwapBank/Src/stm32f7xx_it.c              Interrupt handlers
  - FLASH/FLASH_SwapBank/Src/main.c                      Main program
  - FLASH/FLASH_SwapBank/Src/system_stm32f7xx.c          STM32F7xx system clock configuration file
  - FLASH/FLASH_SwapBank/Binary/FLASH_SwapBank.bin       Binary file to load at bank2

@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This example has been tested with STM32F769I-EVAL board and can be
    easily tailored to any other supported device and development board.
      
@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
