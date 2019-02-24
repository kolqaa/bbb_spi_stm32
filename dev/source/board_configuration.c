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
void SystemClock_Config(void)
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
	clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK |
			RCC_CLOCKTYPE_HCLK   |
			RCC_CLOCKTYPE_PCLK1  |
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

void led_init(void)
{
	GPIO_InitTypeDef  gpio_init = {0};

	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* Configure the GPIO_LED pin */
	gpio_init.Pin    = GPIO_PIN_1;
	gpio_init.Mode   = GPIO_MODE_OUTPUT_OD;
	gpio_init.Pull   = GPIO_NOPULL;
	gpio_init.Speed  = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOB, &gpio_init);

	/* Turn-off led */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
}

void spi_init(SPI_HandleTypeDef *spi_data)
{
	GPIO_InitTypeDef  gpio_spi = {0};

	/* Configure the GPIO for SPI */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* initialize A4/SS open drain */
	gpio_spi.Pin = GPIO_PIN_4;
	gpio_spi.Mode = GPIO_MODE_AF_OD;
	gpio_spi.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &gpio_spi);

	/* initialize A5/SCK open-drain*/
	gpio_spi.Pin = GPIO_PIN_5;
	gpio_spi.Mode = GPIO_MODE_AF_OD;
	gpio_spi.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &gpio_spi);

	/* initialize A6/MISO push-pull */
	gpio_spi.Pin = GPIO_PIN_6;
	gpio_spi.Mode = GPIO_MODE_AF_PP;
	gpio_spi.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &gpio_spi);

	/*initialize A7/MOSI open-drain */
	gpio_spi.Pin = GPIO_PIN_7;
	gpio_spi.Mode = GPIO_MODE_AF_OD;
	gpio_spi.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &gpio_spi);

	/* Set the SPI parameters !Setup as slave!*/
	spi_data->Instance		 = SPIx;
	spi_data->Init.NSS		 = SPI_NSS_SOFT;
	spi_data->Init.Mode		 = SPI_MODE_SLAVE;
	spi_data->Init.TIMode            = SPI_TIMODE_DISABLE;
	spi_data->Init.CLKPhase          = SPI_PHASE_1EDGE;
	spi_data->Init.DataSize          = SPI_DATASIZE_8BIT;
	spi_data->Init.FirstBit          = SPI_FIRSTBIT_MSB;
	spi_data->Init.Direction         = SPI_DIRECTION_2LINES;
	spi_data->Init.CLKPolarity       = SPI_POLARITY_LOW;
	spi_data->Init.CRCPolynomial	 = 7;
	spi_data->Init.CRCCalculation	 = SPI_CRCCALCULATION_DISABLE;
	spi_data->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;


	if (HAL_SPI_Init(&spi_data) != HAL_OK) {
		/* Initialization Error */
		Error_Handler();
	}

	/* SPI block is enabled prior calling SPI transmit/receive functions,
	 * in order to get CLK signal properly pulled down.
	 * Otherwise, SPI CLK signal is not clean on this board and
	 * leads to errors during transfer
	 */
	__HAL_SPI_ENABLE(spi_data);
}
