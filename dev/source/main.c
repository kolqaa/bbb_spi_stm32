#include "main.h"
//#include "/home/msimonov/Downloads/STM32Cube_FW_F1_V1.7.0/Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_hal_uart.h"
static void led_on(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}

static void led_off(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
}


/* @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
UART_HandleTypeDef uart_data = {0};
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&uart_data, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}

/* Configure the UART peripheral
 *
 * Put the USART peripheral in the Asynchronous mode (UART Mode)
 * UART configured as follows:
 * - Word Length = 8 Bits (7 data bit + 1 parity bit)
 *   : BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
 * - Stop Bit    = One Stop bit
 * - Parity      = ODD parity
 * - BaudRate    = 9600 baud
 * - Hardware flow control disabled (RTS and CTS signals)
 */
void uart_init(void)//UART_HandleTypeDef *uart_data)
{
	GPIO_InitTypeDef  GPIO_InitStruct = {0};

	/* Enable peripherals and GPIO Clocks */
	USARTx_TX_GPIO_CLK_ENABLE();
	USARTx_RX_GPIO_CLK_ENABLE();

	/* Enable USARTx clock */
	USARTx_CLK_ENABLE();

	/* Configure peripheral GPIO
	 * UART TX GPIO pin configuration
	 */
	GPIO_InitStruct.Pin       = USARTx_TX_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

	/* UART RX GPIO pin configuration  */
	GPIO_InitStruct.Pin = USARTx_RX_PIN;
	HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

	uart_data.Instance        = USARTx;

	uart_data.Init.BaudRate   = 115200;
	uart_data.Init.WordLength = UART_WORDLENGTH_8B;
	uart_data.Init.StopBits   = UART_STOPBITS_1;
	uart_data.Init.Parity     = UART_PARITY_ODD;
	uart_data.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	uart_data.Init.Mode       = UART_MODE_TX_RX;

	if (HAL_UART_Init(&uart_data) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}
}

int main(void)
{
	SPI_HandleTypeDef spi_data = {0};

	//UART_HandleTypeDef uart_data = {0};
	HAL_Init();
	/* Configure the system clock to 72 MHz */
	SystemClock_Config();

	/* Configure led */
	led_init();

	/* Configure spi */
	spi_init(&spi_data);

	/* Configure uart */
	uart_init();//&uart_data);

	printf("\n\r UART Printf UART\n\r");
	while (1) {
		led_on();
		HAL_Delay(300);

		led_off();
		HAL_Delay(300);
	}

	/* Non reachable code */
	return 0;
}

/* XXX: hack to avoid linker error */
void __attribute__ ((weak)) _init(void)  {}
