/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ui.h"
#include "key.h"
#include "measurement.h"
#include "adc_sample.h"
#include "app_state.h"
#include "buck_control.h"
#include "app_freertos.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for uiTask */
osThreadId_t uiTaskHandle;
const osThreadAttr_t uiTask_attributes = {
  .name = "uiTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for inputTask */
osThreadId_t inputTaskHandle;
const osThreadAttr_t inputTask_attributes = {
  .name = "inputTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for measurementTask */
osThreadId_t measurementTaskHandle;
const osThreadAttr_t measurementTask_attributes = {
  .name = "measurementTask",
  .stack_size = 192 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for appTask */
osThreadId_t appTaskHandle;
const osThreadAttr_t appTask_attributes = {
  .name = "appTask",
  .stack_size = 192 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for controlTask */
osThreadId_t controlTaskHandle;
const osThreadAttr_t controlTask_attributes = {
  .name = "controlTask",
  .stack_size = 192 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for keyQueue */
osMessageQueueId_t keyQueueHandle;
const osMessageQueueAttr_t keyQueue_attributes = {
  .name = "keyQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartUITask(void *argument);
void StartInputTask(void *argument);
void StartMeasurementTask(void *argument);
void StartAppTask(void *argument);
void StartControlTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
   (void)xTask;
   (void)pcTaskName;

   __disable_irq();

   for(;;)
   {
   }


}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
void vApplicationMallocFailedHook(void)
{
   /* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */

   __disable_irq();

   for(;;)
   {
   }

}
/* USER CODE END 5 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of keyQueue */
  keyQueueHandle = osMessageQueueNew (4, sizeof(KeyCode_t), &keyQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of uiTask */
  uiTaskHandle = osThreadNew(StartUITask, NULL, &uiTask_attributes);

  /* creation of inputTask */
  inputTaskHandle = osThreadNew(StartInputTask, NULL, &inputTask_attributes);

  /* creation of measurementTask */
  measurementTaskHandle = osThreadNew(StartMeasurementTask, NULL, &measurementTask_attributes);

  /* creation of appTask */
  appTaskHandle = osThreadNew(StartAppTask, NULL, &appTask_attributes);

  /* creation of controlTask */
  controlTaskHandle = osThreadNew(StartControlTask, NULL, &controlTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartUITask */
/**
  * @brief  Function implementing the uiTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartUITask */
void StartUITask(void *argument)
{
  /* USER CODE BEGIN StartUITask */
  for(;;)
  {
    UI_Update();

    osDelay(200);
  }

  /* USER CODE END StartUITask */
}

/* USER CODE BEGIN Header_StartInputTask */
/**
* @brief Function implementing the inputTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartInputTask */
void StartInputTask(void *argument)
{
  /* USER CODE BEGIN StartInputTask */
  /* Infinite loop */
  for(;;)
  {
	Key_Scan1ms();

	KeyCode_t KeyEvent;

	KeyEvent = Key_GetEvent();

	if(KeyEvent != KEY_NONE)
	{
	    osMessageQueuePut(
	        keyQueueHandle,
	        &KeyEvent,
	        0u,
	        0u);
	}

    osDelay(1);
  }
  /* USER CODE END StartInputTask */
}

/* USER CODE BEGIN Header_StartMeasurementTask */
/**
* @brief Function implementing the measurementTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMeasurementTask */
void StartMeasurementTask(void *argument)
{
  /* USER CODE BEGIN StartMeasurementTask */
  /* Infinite loop */
  for(;;)
  {
	Measurement_UpdateFromAdcRaw(
	            ADC_Sample_GetVoltageRaw(),
	            ADC_Sample_GetCurrentRaw());

    osDelay(10);
  }
  /* USER CODE END StartMeasurementTask */
}

/* USER CODE BEGIN Header_StartAppTask */
/**
* @brief Function implementing the appTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAppTask */
void StartAppTask(void *argument)
{
  /* USER CODE BEGIN StartAppTask */
	KeyCode_t KeyEvent;
  /* Infinite loop */
  for(;;)
  {
	if(osMessageQueueGet(
	        keyQueueHandle,
	        &KeyEvent,
	        NULL,
	        osWaitForever) == osOK)
	{
		AppState_HandleKeyEvent(KeyEvent);
	}

  }
  /* USER CODE END StartAppTask */
}

/* USER CODE BEGIN Header_StartControlTask */
/**
* @brief Function implementing the controlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartControlTask */
void StartControlTask(void *argument)
{
  /* USER CODE BEGIN StartControlTask */
  /* Infinite loop */
  for(;;)
  {
	  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

	  if(AppState_Get() == APP_RUN)
	  {
	      BuckControl_AdjustPwmClosedLoop();
	  }
  }
  /* USER CODE END StartControlTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void ControlTask_NotifyFromISR(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(
        controlTaskHandle,
        &xHigherPriorityTaskWoken
    );

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
/* USER CODE END Application */

