#include "stm32f1xx_hal.h"
#include "freertos.h"
#include "task.h"



void SystemClock_Config(void);



static void vMainTask(void* pParameter)
{
  
  GPIO_InitTypeDef  gpioinitstruct = {0};
  
  /* Enable the GPIO_LED clock */
  __HAL_RCC_GPIOD_CLK_ENABLE(); //
  
  __HAL_RCC_GPIOB_CLK_ENABLE(); //
  /* Configure the GPIO_LED pin */
  gpioinitstruct.Pin    = GPIO_PIN_2|GPIO_PIN_8|GPIO_PIN_10;
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_PULLUP;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  
  HAL_GPIO_Init( GPIOD, &gpioinitstruct);
  HAL_GPIO_Init( GPIOB, &gpioinitstruct);
  
  while(pdTRUE)
  {
    
    GPIOD->ODR ^= GPIO_PIN_2|GPIO_PIN_8|GPIO_PIN_10;
    GPIOB->ODR ^= GPIO_PIN_2|GPIO_PIN_8|GPIO_PIN_10;
    
    vTaskDelay( 200 );
  }
}

void HAL_MspInit(void)
{
  
  /* Configure the system clock to 72 MHz */
  SystemClock_Config();
}

int main()
{
  
  HAL_Init();  
  
  TaskHandle_t  vMainTaskHandle_t = NULL;
  
  /*创建任务*/
  xTaskCreate(vMainTask,"Main Task",configMINIMAL_STACK_SIZE,NULL,configMAX_PRIORITIES-2,&vMainTaskHandle_t);	
  
  
  if( vMainTaskHandle_t )
  {
    vTaskStartScheduler();
  } 
  else
  {
    goto error;
  }
error:
  
  /* We should never get here as control is now taken by the scheduler */
  for( ;; );
  
  return 0;
  
}

void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Configure PLL ------------------------------------------------------*/

  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.LSEState        = RCC_LSE_ON;
  oscinitstruct.HSIState        = RCC_HSI_OFF;
  oscinitstruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  oscinitstruct.HSEPredivValue    = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
}
