#include "main.h"

enum {
	TRANSFER_WAIT,
	TRANSFER_COMPLETE,
	TRANSFER_ERROR
};

/* @brief  SPI error callbacks.
 * @param  hspi: SPI handle
 * @note   This example shows a simple way to report transfer error, and you can
 *         add your own implementation.
 * @retval None
 */
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	hspi->ErrorCode = TRANSFER_ERROR;
}

/* @brief  TxRx Transfer completed callback.
 * @param  hspi: SPI handle
 * @note   This example shows a simple way to report end of Interrupt TxRx transfer, and
 *         you can add your own implementation.
 * @retval None
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	hspi->ErrorCode = TRANSFER_COMPLETE;
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
	spi_data->Instance = SPIx;
	spi_data->Init.NSS = SPI_NSS_SOFT;
	spi_data->Init.Mode = SPI_MODE_SLAVE;
	spi_data->Init.TIMode            = SPI_TIMODE_DISABLE;
	spi_data->Init.CLKPhase          = SPI_PHASE_1EDGE;
	spi_data->Init.DataSize          = SPI_DATASIZE_8BIT;
	spi_data->Init.FirstBit          = SPI_FIRSTBIT_MSB;
	spi_data->Init.Direction         = SPI_DIRECTION_2LINES;
	spi_data->Init.CLKPolarity       = SPI_POLARITY_LOW;
	spi_data->Init.CRCPolynomial = 7;
	spi_data->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	spi_data->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;


	if (HAL_SPI_Init(spi_data) != HAL_OK) {
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
