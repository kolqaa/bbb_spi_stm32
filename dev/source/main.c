#include "main.h"

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *	   System Clock source            = PLL (HSE)
 *	   SYSCLK(Hz)                     = 72000000
 *	   HCLK(Hz)                       = 72000000
 *	   AHB Prescaler                  = 1
 *	   APB1 Prescaler                 = 2
 *	   APB2 Prescaler                 = 1
 *	   HSE Frequency(Hz)              = 8000000
 *	   HSE PREDIV1                    = 1
 *	   PLLMUL                         = 9
 *	   Flash Latency(WS)              = 2
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef clkinitstruct = {0};
	RCC_OscInitTypeDef oscinitstruct = {0};

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
	oscinitstruct.HSEState        = RCC_HSE_ON;
	oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
	oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
	oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
	oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;

	if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK) {
		/* XXX: will stay here forever ... need to be reworked and/or
		 * added logs
		 */
		while(1);
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1
	   and PCLK2 clocks dividers */
	clkinitstruct.ClockType = (
			RCC_CLOCKTYPE_SYSCLK |
			RCC_CLOCKTYPE_HCLK |
			RCC_CLOCKTYPE_PCLK1 |
			RCC_CLOCKTYPE_PCLK2);

	clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
	clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK) {
		/* XXX: will stay here forever ... need to be reworked and/or
		 * added logs
		 */
		while(1);
	}
}


/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
	while (1) { }
}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(char *file, uint32_t line)
{
	/* User can add his own implementation to report the file name
	 * and line number ex:
	 * printf("Wrong parameters value: file %s on line %d\r\n", file, line)
	 */

	/* XXX: Infinite loop */
	while (1) { }
}

#endif

/* XXX: hack to avoid linker error */
void __attribute__ ((weak)) _init(void)  {}

void ledInit(void)
{
	GPIO_InitTypeDef  gpioinitstruct = {0};

	__HAL_RCC_GPIOB_CLK_ENABLE();
	/* Configure the GPIO_LED pin */
	gpioinitstruct.Pin    = GPIO_PIN_1;
	gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_OD;
	gpioinitstruct.Pull   = GPIO_NOPULL;
	gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOB, &gpioinitstruct);

	/* Turn-off led */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
}

void ledOn(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}

void ledOff(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
}

int main(void)
{
	HAL_Init();

	/* Configure the system clock to 72 MHz */
	SystemClock_Config();

	/* Configure led */
	ledInit();

	/* Infinite loop */
	while (1) {
		ledOn();

		HAL_Delay(300);

		ledOff();
		HAL_Delay(300);
	}

	/* Not reachable code */
	return 0;
}


