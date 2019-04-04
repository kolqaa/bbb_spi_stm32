#ifndef __BOARD_CONF_H__
# define __BOARD_CONFI_H__

void SystemClock_Config(void);
void led_init(void);
void spi_init(SPI_HandleTypeDef *spi_data);

#endif /* __BOARD_CONF_H__ */
