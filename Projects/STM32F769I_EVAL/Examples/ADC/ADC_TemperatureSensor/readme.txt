/**
  @page ADC_TemperatureSensor conversion using the internal Temperature Sensor

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    ADC/ADC_TemperatureSensor/readme.txt 
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    30-December-2016
  * @brief   Description of the ADC RegularConversion DMA example.
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

This example describes how to use the ADC1 and the Temperature Sensor to calculate
the junction temperature of the device.

The ADC1 is configured to convert continuously ADC_CHANNEL_TEMPSENSOR.

Each time an end of conversion occurs the DMA transfers, in circular mode, the
converted data from ADC1 DR register to the ConvertedValue variable.

The ConvertedValue read value is coded on 12 bits.

The Junction Temperature of the device is calculated via the formula:
JTemp = ((Vsens - V25)/Avg_Slope) + 25°C
 - Vsens = ConvertedValue * (Vref+ - Vref-) / (2^12 - 1) = ConvertedValue * 3.3 / 4095
 - V25 = 760 mV
 - Avg_Slope = 2.5 mV/C°

The calculated temperature is stored in the JTemp variable. The value is displayed
on the LCD and refreshed each 1s.

The internal temperature sensor supports a temperature range of –40 to 125°C with
an accuracy of +/-1.5°C.

STM32F769I-EVAL board's LEDs can be used to monitor the transfer status:
  - LED1 (Green LED) toggles each 1s when the ADC is converting.
  - LED3 (Red LED) blinks when error occurs in initialization.

@par Keywords

Analog, ADC, Temperatuer Sensor, Single Conversion, Polling, Software trigger, Signal measurement,
  
@Note  If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),
       it is recommended to configure the latters as Write Through.
       This is ensured by configuring the memory attributes at MPU level in order to ensure cache coherence on SRAM1 and SRAM2.
       Please, refer to Template project for a typical MPU configuration.

@Note  If external memory is shared between several processors, it is recommended to configure it as Write Back (bufferable), shareable and cacheable.
       The memory base address and size must be properly updated.
       The user needs to manage the cache coherence at application level.

For more details about the MPU configuration and use, please refer to AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”

@par Directory contents 

  - ADC/ADC_TemperatureSensor/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - ADC/ADC_TemperatureSensor/Inc/stm32f7xx_it.h          DMA interrupt handlers header file
  - ADC/ADC_TemperatureSensor/Inc/main.h                  Header for main.c module  
  - ADC/ADC_TemperatureSensor/Src/stm32f7xx_it.c          DMA interrupt handlers
  - ADC/ADC_TemperatureSensor/Src/main.c                  Main program
  - ADC/ADC_TemperatureSensor/Src/stm32f7xx_hal_msp.c     HAL MSP file 
  - ADC/ADC_TemperatureSensor/Src/system_stm32f7xx.c      STM32F7xx system source file

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
