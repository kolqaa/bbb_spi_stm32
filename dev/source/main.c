#include "main.h"

static void ledOn(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}

static void ledOff(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
}

static void board_init(void)
{
	HAL_Init();

	/* Configure the system clock to 72 MHz */
	SystemClock_Config();

	/* Configure led */
	led_init();
}

int main(void)
{
	board_init();
//	spi_init();

	while (1) {
		ledOn();
		HAL_Delay(300);

		ledOff();
		HAL_Delay(300);
	}

	/* Non reachable code */
	return 0;
}

/* XXX: hack to avoid linker error */
void __attribute__ ((weak)) _init(void)  {}
