/**
  * Header file for main.c. Actually it's not required and can be removed.
  * But right now is the fastest way to build v0.1 is to leave it "as is"
  *
  * 2018 Sergey Shcherbakov <shchers@gmail.com>
  */

#ifndef __MAIN_H__
#define __MAIN_H__

// CubeMX headers
#include "stm32f1xx_hal.h"
#include "board_common.h"
#include "board_configuration.h"

/* Definition for SPIx clock resources */
#define SPIx                             SPI1
#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

/* Definition for SPIx Pins */
#define SPIx_SCK_PIN                     GPIO_PIN_5
#define SPIx_SCK_GPIO_PORT               GPIOA
#define SPIx_MISO_PIN                    GPIO_PIN_6
#define SPIx_MISO_GPIO_PORT              GPIOA
#define SPIx_MOSI_PIN                    GPIO_PIN_7
#define SPIx_MOSI_GPIO_PORT              GPIOA

/* Definition for SPIx's NVIC */
#define SPIx_IRQn                        SPI1_IRQn
#define SPIx_IRQHandler                  SPI1_IRQHandler

#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

#endif /* __MAIN_H__ */
