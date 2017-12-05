/**
  ******************************************************************************
  * @file    Examples_MIX/DMA2D/DMA2D_MemToMemWithRBSwap/Src/main.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    30-December-2016
  * @brief   This example provides a description of how to configure 
  *          DMA2D peripheral in Memory to Memory transfer mode while applying 
  *          Red and Blue Swap and display the result on LCD, in resorting to 
  *          DMA2D LL APIs.
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
#include "ARGB8888_128x160.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup DMA2D_MemToMemWithLCD
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum 
{
  SHIELD_NOT_DETECTED = 0,
  SHIELD_DETECTED
}ShieldStatus;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Buffers filled up by DMA2D IP */
uint32_t aBuffer0[(LAYER_SIZE_X * LAYER_SIZE_Y * LAYER_BYTE_PER_PIXEL) / 4];
uint32_t aBuffer1[(LAYER_SIZE_X * LAYER_SIZE_Y * LAYER_BYTE_PER_PIXEL) / 4];
uint32_t aBufferOutput[(LAYER_SIZE_X * LAYER_SIZE_Y * LAYER_BYTE_PER_PIXEL) / 4];
/* Blended image Ready flag */
__IO uint32_t   blended_image_ready = 0;

/* Private function prototypes -----------------------------------------------*/
static ShieldStatus TFT_ShieldDetect(void);
static void LCD_ImageDisplay(void);
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
void Error_Handler(void);
/* Private functions ---------------------------------------------------------*/


/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t i = 0;

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32F7xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure LED1, LED2 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* Check the availability of adafruit 1.8" TFT shield on top of STM32NUCLEO
     board. This is done by reading the state of IO PF.03 pin (mapped to 
     JoyStick available on adafruit 1.8" TFT shield). If the state of PF.03 
     is high then the adafruit 1.8" TFT shield is available. */

  if(TFT_ShieldDetect() != SHIELD_DETECTED)
  {
    Error_Handler();
  }

  /* Initialize the LCD */
  BSP_LCD_Init();

  /*## DMA2D Clock enable ################################################*/
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2D);
  NVIC_SetPriority(DMA2D_IRQn, 0);
  NVIC_EnableIRQ(DMA2D_IRQn);

  /* Infinite loop start */
  while (1)
  {
    /*## 1 - Prepare first two images to blend ###############################*/
    blended_image_ready = 0;

    /*** 1a - First image preparation *****************************************/
    /* The first image is created from the ARGB8888 input image */
    /* Configure the DMA2D Ouput Mode to Pixel Format Conversion */
    LL_DMA2D_SetMode(DMA2D, LL_DMA2D_MODE_M2M_PFC);

    /* Configure the DMA2D output Color Mode */
    /* For the first image, force ARGB1555 setting not supported by LCD in order
       to artificially create a distorted image */
    LL_DMA2D_SetOutputColorMode(DMA2D, LL_DMA2D_OUTPUT_MODE_ARGB1555);

    /* Foreground (Input) Configuration */
    /* Set Alpha value to full opaque */
    LL_DMA2D_FGND_SetAlpha(DMA2D, 0xFF);
    /* Foreground (Input) layer format is ARGB8888 (32 bpp) */
    LL_DMA2D_FGND_SetColorMode(DMA2D, LL_DMA2D_INPUT_MODE_ARGB8888);
    /* Set Foreground (Input) memory address */
    LL_DMA2D_FGND_SetMemAddr(DMA2D, (uint32_t)&ARGB8888_128x160);

    /* Set output buffer address */
    LL_DMA2D_SetOutputMemAddr(DMA2D, (uint32_t)&aBuffer0);

    /* Config DMA2D size transfer */
    LL_DMA2D_ConfigSize(DMA2D, LAYER_SIZE_Y, LAYER_SIZE_X);

    /* Enable the transfer complete, transfer error and configuration error interrupts */
    LL_DMA2D_EnableIT_TC(DMA2D);
    LL_DMA2D_EnableIT_TE(DMA2D);
    LL_DMA2D_EnableIT_CE(DMA2D);

    /* Enable the Peripheral */
    LL_DMA2D_Start(DMA2D);

    /* Wait until first output buffer is ready */
    while(blended_image_ready == 0) {;}

    /*** 1b - Second image preparation ****************************************/
    /* Likewise, the second image is created from the ARGB8888 input image */
    /* Update only needed parameters */
    blended_image_ready = 0;

    /* Configure the DMA2D output Color Mode */
    /* Set proper output mode, supported by the LCD */
    LL_DMA2D_SetOutputColorMode(DMA2D, LL_DMA2D_OUTPUT_MODE_RGB565);
    /* Force Red and Blue swap for illustration purposes */
    LL_DMA2D_SetOutputRBSwapMode(DMA2D, LL_DMA2D_RB_MODE_SWAP);

    /* Set output buffer address */
    LL_DMA2D_SetOutputMemAddr(DMA2D, (uint32_t)&aBuffer1);

    /* Enable the transfer complete, transfer error and configuration error interrupts */
    LL_DMA2D_EnableIT_TC(DMA2D);
    LL_DMA2D_EnableIT_TE(DMA2D);
    LL_DMA2D_EnableIT_CE(DMA2D);

    /* Enable the Peripheral */
    LL_DMA2D_Start(DMA2D);

    /* Wait until second output buffer is ready */
    while(blended_image_ready == 0) {;}

    /*## 2 - Blend first two images  #########################################*/
    /* The first image (distorted image) gradually and smoothly gives way to the
       second one (image with R&B swapped for illustration purposes) */

    /* Update only needed parameters */
    /* Set alpha mode for foreground (first image) and background (second image) buffers */
    LL_DMA2D_FGND_SetAlphaMode(DMA2D, LL_DMA2D_ALPHA_MODE_REPLACE);
    LL_DMA2D_BGND_SetAlphaMode(DMA2D, LL_DMA2D_ALPHA_MODE_REPLACE);
    /* Set input color mode for foreground (first image) and background (second image) buffers */
    LL_DMA2D_FGND_SetColorMode(DMA2D, LL_DMA2D_INPUT_MODE_RGB565);
    LL_DMA2D_BGND_SetColorMode(DMA2D, LL_DMA2D_INPUT_MODE_RGB565);

    /* Set blending mode */
    LL_DMA2D_SetMode(DMA2D, LL_DMA2D_MODE_M2M_BLEND);
    /* No R&B swap for the blended output */
    LL_DMA2D_SetOutputRBSwapMode(DMA2D, LL_DMA2D_RB_MODE_REGULAR);

    /* Set foreground, background and output address */
    LL_DMA2D_FGND_SetMemAddr(DMA2D, (uint32_t)&aBuffer0);
    LL_DMA2D_BGND_SetMemAddr(DMA2D, (uint32_t)&aBuffer1);
    LL_DMA2D_SetOutputMemAddr(DMA2D, (uint32_t)aBufferOutput);

    /* Gradual images blending process */
    for (i=0; i< 256; i+=51)
    {
      HAL_Delay(10);

      /* 2a - Blending process ************************************************/
      blended_image_ready = 0;

      /* While decreasing alpha value for foreground image, increase alpha value
        of background image */
      LL_DMA2D_FGND_SetAlpha(DMA2D, 0xFF - (i%256));
      LL_DMA2D_BGND_SetAlpha(DMA2D, (i%256));

      /* Enable the transfer complete, transfer error and configuration error interrupts */
      LL_DMA2D_EnableIT_TC(DMA2D);
      LL_DMA2D_EnableIT_TE(DMA2D);
      LL_DMA2D_EnableIT_CE(DMA2D);

      /* Enable the Peripheral */
      LL_DMA2D_Start(DMA2D);

      /* 2b- Copy blended image in center of LCD frame buffer *****************/
      /* Wait until blended image is ready to be displayed */
      while(blended_image_ready == 0) {;}

      /* Display output buffer on LCD */
      LCD_ImageDisplay();
    }


    /*## 3 - Third image preparation #########################################*/
    /* Create third image from same ARGB8888 input image.
     DMA2D transfer parameters are set to get the correct format at the output.
     Again, update only necessary parameters  */

    blended_image_ready = 0;

    /* Configure the DMA2D Ouput Mode to Pixel Format Conversion */
    LL_DMA2D_SetMode(DMA2D, LL_DMA2D_MODE_M2M_PFC);

    /* Set third image buffer address */
    LL_DMA2D_SetOutputMemAddr(DMA2D, (uint32_t)&aBuffer0);
    /* Set Foreground (Input) memory address */
    LL_DMA2D_FGND_SetMemAddr(DMA2D, (uint32_t)&ARGB8888_128x160);
    /* Foreground layer format is ARGB8888 (32 bpp) */
    LL_DMA2D_FGND_SetColorMode(DMA2D, LL_DMA2D_INPUT_MODE_ARGB8888);

    /* Config DMA2D size transfer */
    LL_DMA2D_ConfigSize(DMA2D, LAYER_SIZE_Y, LAYER_SIZE_X);

    /* Enable the transfer complete, transfer error and configuration error interrupts */
    LL_DMA2D_EnableIT_TC(DMA2D);
    LL_DMA2D_EnableIT_TE(DMA2D);
    LL_DMA2D_EnableIT_CE(DMA2D);

    /* Enable the Peripheral */
    LL_DMA2D_Start(DMA2D);

    /* Wait until output buffer is ready */
    while(blended_image_ready == 0) {;}

    /*## 4 - Blend next two images  ##########################################*/
    /* The second image (R&B swapped) now gradually and smoothly gives way to the
       third one (that with the correct format) */

    /* Update only needed parameters */
    LL_DMA2D_FGND_SetColorMode(DMA2D, LL_DMA2D_INPUT_MODE_RGB565);
    LL_DMA2D_SetMode(DMA2D, LL_DMA2D_MODE_M2M_BLEND);

    LL_DMA2D_FGND_SetMemAddr(DMA2D, (uint32_t)&aBuffer1);
    LL_DMA2D_BGND_SetMemAddr(DMA2D, (uint32_t)&aBuffer0);
    LL_DMA2D_SetOutputMemAddr(DMA2D, (uint32_t)aBufferOutput);

    /* Gradual images blending process */
    for (i=0; i< 256; i+=51)
    {
      HAL_Delay(10);

      /* 4a - Blending process ************************************************/
      blended_image_ready = 0;

      /* While decreasing alpha value for foreground image, increase alpha value
        of background image */
      LL_DMA2D_FGND_SetAlpha(DMA2D, 0xFF - (i%256));
      LL_DMA2D_BGND_SetAlpha(DMA2D, (i%256));

      /* Enable the transfer complete, transfer error and configuration error interrupts */
      LL_DMA2D_EnableIT_TC(DMA2D);
      LL_DMA2D_EnableIT_TE(DMA2D);
      LL_DMA2D_EnableIT_CE(DMA2D);

      /* Enable the Peripheral */
      LL_DMA2D_Start(DMA2D);

      /* 4b- Copy blended image in center of LCD frame buffer *****************/
      /* Wait until blended image is ready to be displayed */
      while(blended_image_ready == 0) {;}

      /* Display output buffer on LCD */
      LCD_ImageDisplay();
    }

    /* One-second delay */
    HAL_Delay(1000);


    /*## 5 - Fourth image preparation ########################################*/
    /* Create a fully white image, stored in aBuffer1 */
    /* This image will be used to ensure a smooth fading of the last image,
       giving way to a blank screen */
    for (i=0; i< (LAYER_SIZE_X * LAYER_SIZE_Y)/2 ; i++)
    {
      *(aBuffer1+i) = 0xFFFFFFFF;
    }


    /*## 6 - Blend next two images  ##########################################*/
    /* The third image (correct format one) now gradually and smoothly fades away 
       to give way to a blank screen (the last image) */  

    /* The only parameters to update is that of the foreground and background addresses */
     LL_DMA2D_FGND_SetMemAddr(DMA2D, (uint32_t)&aBuffer0);
     LL_DMA2D_BGND_SetMemAddr(DMA2D, (uint32_t)&aBuffer1);

    /* Gradual images blending process */
    for (i=0; i< 256; i+=51)
    {
      HAL_Delay(10);

      /* 6a - Blending process ************************************************/
      blended_image_ready = 0;

      LL_DMA2D_FGND_SetAlpha(DMA2D, 0xFF - (i%256));
      LL_DMA2D_BGND_SetAlpha(DMA2D, (i%256));

      /* Enable the transfer complete, transfer error and configuration error interrupts */
      LL_DMA2D_EnableIT_TC(DMA2D);
      LL_DMA2D_EnableIT_TE(DMA2D);
      LL_DMA2D_EnableIT_CE(DMA2D);

      /* Enable the Peripheral */
      LL_DMA2D_Start(DMA2D);

      /* 6b- Copy blended image in center of LCD frame buffer *****************/
      /* Wait until blended image is ready to be displayed */
      while(blended_image_ready == 0) {;}

      /* Display output buffer on LCD */
      LCD_ImageDisplay();
    }

    /* One-second delay before re-starting the complete loop */
    HAL_Delay(1000);

  } /*  while (1) */

}

/**
  * @brief  On Error Handler on condition TRUE.
  * @param  condition : Can be TRUE or FALSE
  * @retval None
  */
void OnError_Handler(uint32_t condition)
{
  if(condition)
  {
    BSP_LED_On(LED2);
    while(1) { ; } /* Blocking on error */
  }
}

/**
  * @brief  Check the availability of adafruit 1.8" TFT shield on top of STM32NUCLEO
  *         board. This is done by reading the state of IO PF.03 pin (mapped to 
  *         JoyStick available on adafruit 1.8" TFT shield). If the state of PF.03 
  *         is high then the adafruit 1.8" TFT shield is available.
  * @param  None
  * @retval SHIELD_DETECTED: 1.8" TFT shield is available
  *         SHIELD_NOT_DETECTED: 1.8" TFT shield is not available
  */
static ShieldStatus TFT_ShieldDetect(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct; 

  /* Enable GPIO clock */
  NUCLEO_ADCx_GPIO_CLK_ENABLE();

  GPIO_InitStruct.Pin = NUCLEO_ADCx_GPIO_PIN ;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(NUCLEO_ADCx_GPIO_PORT , &GPIO_InitStruct);

  if(HAL_GPIO_ReadPin(NUCLEO_ADCx_GPIO_PORT, NUCLEO_ADCx_GPIO_PIN) != 0)
  {
    return SHIELD_DETECTED;
  }
  else
  {
    return SHIELD_NOT_DETECTED;
  }
}

/**
  * @brief  LCD display image on Adafruit LCD.
  * @param  None
  * @retval None
  */
static void LCD_ImageDisplay(void)
{
  int16_t line = 0;
  uint16_t pixel = 0;
  uint16_t *img_ptr = NULL;

  /* Set pointer to DMA2D output buffer */
  img_ptr = (uint16_t *)aBufferOutput;
  line = LAYER_SIZE_Y - 1;
  while (line >= 0)
  {
    /* Write pixels */
    BSP_LCD_DrawPixel(pixel, line, (uint16_t)*img_ptr);
    img_ptr++;
    pixel++;
    if (pixel == LAYER_SIZE_X)
    {
      /* Move to next line */
      pixel = 0;
      line--;
    }
  }
}

/**
  * @brief  This function handles the test program fail.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  BSP_LED_On(LED3);
  while(1)
  {
  }
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            PLL_R                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  RCC_OscInitStruct.PLL.PLLR = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1) {};
  }

  /* Activate the OverDrive to reach the 216 Mhz Frequency */
  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    while(1) {};
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    while(1) {};
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/