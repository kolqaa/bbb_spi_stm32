#include "main.h"

static void led_on(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}

static void led_off(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
}

int main(void)
{
	SPI_HandleTypeDef spi_data = {0};

	HAL_Init();
	/* Configure the system clock to 72 MHz */
	SystemClock_Config();

	/* Configure led */
	led_init();

	/* Configure spi */
	spi_init(&spi_data);

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
