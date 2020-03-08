/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    30-December-2016  
  * @brief   This file provides main program functions
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
#include "main.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup MAIN
* @brief main file
* @{
*/ 

/** @defgroup MAIN_Private_TypesDefinitions
* @{
*/ 
#ifdef INCLUDE_THIRD_PARTY_MODULE  
 typedef  void (*pFunc)(void);
#endif
/**
* @}
*/ 

/** @defgroup MAIN_Private_Defines
* @{
*/ 
/**
* @}
*/ 


/** @defgroup MAIN_Private_Macros
* @{
*/ 
/**
* @}
*/ 


/** @defgroup MAIN_Private_Variables
* @{
*/
/**
* @}
*/ 


/** @defgroup MAIN_Private_FunctionPrototypes
* @{
*/ 

static void SystemClock_Config(void);
static void GUIThread(void const * argument);
static void TimerCallback(void const *n);
static void MPU_Config(void);


extern K_ModuleItem_Typedef  video_player_board;
extern K_ModuleItem_Typedef  audio_player_board;
extern K_ModuleItem_Typedef  audio_recorder_board;
extern K_ModuleItem_Typedef  vnc_server_board;
extern K_ModuleItem_Typedef  home_alarm_board;
#ifdef INCLUDE_THIRD_PARTY_MODULE
extern K_ModuleItem_Typedef  touchGFX_board;
extern K_ModuleItem_Typedef  embedWizard_board;
#endif
extern K_ModuleItem_Typedef  gardening_control_board;
extern K_ModuleItem_Typedef  settings_board;
extern K_ModuleItem_Typedef  games_board;


osTimerId lcd_timer;
#if (configAPPLICATION_ALLOCATED_HEAP == 1)
uint8_t  ucHeap[configTOTAL_HEAP_SIZE];
#endif
/**
 * @}
 */ 

/** @defgroup MAIN_Private_Functions
 * @{
 */ 

/**
 * @brief  Main program
 * @param  None
 * @retval int
 */
int main(void)
{ 
#ifdef INCLUDE_THIRD_PARTY_MODULE   
   /* Code Need to handle third party module */  
  __HAL_RCC_RTC_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_BKPSRAM_CLK_ENABLE(); 
  
  HAL_PWR_EnableBkUpAccess();

  if(*(__IO uint32_t *)(DEMO_SIGNATURE_LOCATION) == DEMO_TGFX_SIGNATURE)
  {
     CleanDisplay(0xC0000000);
    *(__IO uint32_t *)(DEMO_SIGNATURE_LOCATION) = DEMO_HOME_SIGNATURE;
    
    /* Reinitialize the Stack pointer*/
    __set_MSP(*(__IO uint32_t*) DEMO_TGFX_ADDRESS);
    /* jump to application address */  
    ((pFunc) (*(__IO uint32_t*) (DEMO_TGFX_ADDRESS + 4)))();
  }
  else if(*(__IO uint32_t *)(DEMO_SIGNATURE_LOCATION) == DEMO_EWIZ_SIGNATURE)
  {
     CleanDisplay(0xC0000000);     
    *(__IO uint32_t *)(DEMO_SIGNATURE_LOCATION) = DEMO_HOME_SIGNATURE;
    
    /* Reinitialize the Stack pointer*/
    __set_MSP(*(__IO uint32_t*) DEMO_EWIZ_ADDRESS);
    /* jump to application address */  
    ((pFunc) (*(__IO uint32_t*) (DEMO_EWIZ_ADDRESS + 4)))();
  }
#endif  
  
  /* Configure the MPU attributes as Write Through */
  MPU_Config();
	
  /* Invalidate I-Cache : ICIALLU register*/
  SCB_InvalidateICache();	
  
  /* Enable branch prediction */
  SCB->CCR |= (1 <<18); 
  __DSB();
  
  /* Invalidate I-Cache : ICIALLU register*/
  SCB_InvalidateICache();    
  
  /* Enable I-Cache */
  SCB_EnableICache();        
  
  SCB_InvalidateDCache();   
  SCB_EnableDCache();       
  
  /* STM32F7xx HAL library initialization:
  - Configure the Flash ART accelerator on ITCM interface
  - Configure the Systick to generate an interrupt each 1 msec
  - Set NVIC Group Priority to 4
  - Global MSP (MCU Support Package) initialization
  */
  HAL_Init();
  
  /* Configure the system clock @ 200 Mhz */
  SystemClock_Config();
	
  /* Configure the board */
  k_BspInit(); 

  /* Initialize RTC */
  k_CalendarBkupInit();    

  /* Create GUI task */
  osThreadDef(GUI_Thread, GUIThread, osPriorityNormal, 0, 4 * 1024);
  osThreadCreate (osThread(GUI_Thread), NULL); 
  
  /* Add Modules*/
  k_ModuleInit();
   
  /* Link modules */ 
  k_ModuleAdd(&audio_player_board);       
  k_ModuleAdd(&video_player_board); 
  k_ModuleAdd(&audio_recorder_board);
  k_ModuleAdd(&vnc_server_board);
  k_ModuleAdd(&home_alarm_board);
#ifdef INCLUDE_THIRD_PARTY_MODULE  
  k_ModuleAdd(&touchGFX_board);    
  k_ModuleAdd(&embedWizard_board); 
#else  
  k_ModuleAdd(&gardening_control_board);
  k_ModuleAdd(&games_board);
#endif  
  k_ModuleAdd(&settings_board);
  
    /* Initialize Storage Units */
  k_StorageInit();   
  
  /* Start scheduler */
  osKernelStart ();   

  /* We should never get here as control is now taken by the scheduler */
  for( ;; );
}

/**
  * @brief  Start task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void GUIThread(void const * argument)
{    
  /* Initialize GUI */
  GUI_Init();
    
  WM_MULTIBUF_Enable(1);
  GUI_SetLayerVisEx (1, 0);
  GUI_SelectLayer(0);
  
  GUI_SetBkColor(GUI_WHITE);
  GUI_Clear();   
  
  /* Set General Graphical proprieties */
  k_SetGuiProfile();
  
#ifdef INCLUDE_THIRD_PARTY_MODULE 
  if(*(__IO uint32_t *)(DEMO_SIGNATURE_LOCATION) == DEMO_HOME_SIGNATURE)
  {
    *(__IO uint32_t *)(DEMO_SIGNATURE_LOCATION) = 0; 
  }
  else
  {
    /* Demo Startup */
    k_StartUp();
  }
#else
  k_StartUp();
#endif

  /* Create Touch screen Timer */
  osTimerDef(TS_Timer, TimerCallback);
  lcd_timer =  osTimerCreate(osTimer(TS_Timer), osTimerPeriodic, (void *)0);

  /* Start the TS Timer */
  osTimerStart(lcd_timer, 30);
  
  /* Show the main menu */
  k_InitMenu();
  
  /* Gui background Task */
  while(1) {
    GUI_Exec(); /* Do the background work ... Update windows etc.) */
    k_PeriodicProcesses();
    osDelay(10); 
  }
}

/**
  * @brief  vApplicationStackOverflowHook
  * @param  xTask : task handle
  * @param  pcTaskName : pointer task name
  * @retval None
  */
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
  while(1);
}
/**
  * @brief  Timer callbacsk (40 ms)
  * @param  n: Timer index 
  * @retval None
  */
static void TimerCallback(void const *n)
{  
  k_TouchUpdate();
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 200000000
  *            HCLK(Hz)                       = 200000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 400
  *            PLL_P                          = 2
  *            PLLSAI_N                       = 384
  *            PLLSAI_P                       = 8
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 6
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;  
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
          
  ret = HAL_PWREx_EnableOverDrive();

  if(ret != HAL_OK)
  {
    while(1) { ; }
  }  
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
}


void HAL_Delay (__IO uint32_t Delay)
{
  while(Delay) 
  {
    if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) 
    {
      Delay--;
    }
  }
}

/**
  * @brief  Configure the MPU attributes as Write Through for SRAM1/2.
  * @note   The Base Address is 0x20010000 since this memory interface is the AXI.
  *         The Region Size is 256KB, it is related to SRAM1 and SRAM2  memory size.
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;
  
  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as WT for SRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x20020000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  
  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0xC0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256MB; 
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE; 
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);  
 
  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0xC0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_8MB; 
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE; 
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER3;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  
  /* Configure the MPU attributes as strongly ordred for QSPI (unused zone) */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x90000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RO;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER4;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
	
  /* Configure the MPU attributes as WT for QSPI (used 16Mbytes) */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x90000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_16MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RO;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER5;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);	
 
  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
  number,ex: printf("Wrong parameters value: file %s on line %d\r\n", 
  file, line) */
  
  /* Infinite loop */
  while (1)
  {}
}
#endif


/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/