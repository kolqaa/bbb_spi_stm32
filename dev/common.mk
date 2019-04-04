#
#
# Copyright (C) 2012, Sergey Shcherbakov <shchers@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#

TOP := $(PWD)

ifeq ($(CUBE_PATH),)
$(error ERROR: CUBE_PATH (folder with STM32CubeMX framework) is not defined)
endif

ifeq ($(CROSS_COMPILE),)
$(warning WARNING: CROSS_COMPILE variable is not defined. Looks like a native build)
ifneq ($(shell which arm-none-eabi-gcc),)
$(warning WARNING: To build image will be used ARM GCC from PATH ($(shell which arm-none-eabi-gcc)))
CROSS_COMPILE = arm-none-eabi-
endif
endif

AS      = $(CROSS_COMPILE)as
LD      = $(CROSS_COMPILE)ld
CC      = $(CROSS_COMPILE)gcc
CPP     = $(CROSS_COMPILE)g++
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
STRIP   = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
SIZE    = $(CROSS_COMPILE)size

# CPU tuning
CPU := -mthumb -march=armv7-m -mtune=cortex-m3 -mcpu=cortex-m3 -mfix-cortex-m3-ldrd
FPU := -mfloat-abi=soft

# Default C standad
C_STD ?= c99

# Common Assembler flags
AFLAGS += $(CPU) $(FPU)
AFLAGS += -MD

# Common C flags
CFLAGS += $(CPU) $(FPU)
CFLAGS += -std=$(C_STD)
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -MD
# List of warnings
CFLAGS += -Wall -Wextra -Wshadow -Wpointer-arith
CFLAGS += -Wbad-function-cast -Wsign-compare
CFLAGS += -Waggregate-return -Wstrict-prototypes -Wunused -Wno-cpp
#CFLAGS += -Wcast-align
#CFLAGS += -Wcast-align -pedantic
ifdef DEBUG
CFLAGS += -g -D DEBUG -O0
else
CFLAGS += -Os
endif

# Default GCC libs and their pathes
GCCLIBS := $(shell $(CC) $(CFLAGS) -print-file-name=libc.a)
GCCLIBS += $(shell $(CC) $(CFLAGS) -print-file-name=libgcc.a)
GCCLIBS += $(shell $(CC) $(CFLAGS) -print-file-name=libm.a)

# Default linker properties
LDFLAGS += $(CPU) $(FPU)
LDFLAGS += -Wl,-static -Wl,--gc-sections -Wl,-nostdlib -nostartfiles

CFLAGS += -I$(TOP)/include
CFLAGS += -I$(CUBE_PATH)/Drivers/CMSIS/Device/ST/STM32F1xx/Include/
CFLAGS += -I$(CUBE_PATH)/Drivers/CMSIS/Include/
CFLAGS += $(addprefix -I$(TOP)/,$(INC))

# Let's add a bit information about firmware
CFLAGS += -DGIT_VER="\"$(shell git describe --tags --dirty --always)\""

# Common Assembler flags
AFLAGS += $(CPU) $(FPU)
AFLAGS += -mthumb
AFLAGS += -MD

# Configuration for STM32Cube lib
CFLAGS += -DSTM32F1xx
CFLAGS += -DSTM32F103xB
CFLAGS += -DUSE_HAL_DRIVER
CFLAGS += -I$(CUBE_PATH)/Drivers/STM32F1xx_HAL_Driver/Inc

# Let's reuse generic source code
CUBE_SRC := \
	Projects/STM3210E_EVAL/Examples/GPIO/GPIO_IOToggle/Src/stm32f1xx_it.c \
	Projects/STM3210E_EVAL/Examples/GPIO/GPIO_IOToggle/Src/system_stm32f1xx.c

CFLAGS += -I$(CUBE_PATH)/Projects/STM3210E_EVAL/Examples/GPIO/GPIO_IOToggle/Inc \
	  -I$(CUBE_PATH)/Projects/STM32VL-Discovery/Examples/SPI/SPI_FullDuplex_ComIT/Inc \
	  -I$(CUBE_PATH)/Projects/STM3210E_EVAL/Examples/UART/UART_Printf/Inc

# Adding CubeMX drivers
CUBE_ASM_SRC += \
	Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xb.s \

CUBE_SRC += \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c \
	Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c

LDSCRIPT := source/STM32F103x8.ld
LDFLAGS += -L source
LDFLAGS += -specs=nosys.specs

OUT_NAME := slave_spi

# Objects list
OBJS := $(addprefix $(TOP)/,$(C_SRC:.c=.o))
OBJS += $(addprefix $(TOP)/,$(ASM_SRC:.s=.o))
OBJS += $(addprefix $(CUBE_PATH)/,$(CUBE_ASM_SRC:.s=.o))
OBJS += $(addprefix $(CUBE_PATH)/,$(CUBE_SRC:.c=.o))

all: $(OUT_NAME).bin

%.o : %.c
	@echo "\\033[33m--- Building $< ---\\033[0m"
	$(CC) $(CFLAGS) -c $< -o $@

%.o : %.s
	@echo "\\033[33m--- Building $< ---\\033[0m"
	$(CC) $(AFLAGS) -c $< -o $@

$(OUT_NAME).elf: $(OBJS)
	@echo "\\033[1;37;42m--- Linking $(@) ---\\033[0m"
	$(CC) -T$(LDSCRIPT) $(LDFLAGS) -Wl,-Map=$(@:.elf=.map) -o $(@) $(OBJS) $(GCCLIBS)
	$(SIZE) -A -x $(OUT_NAME).elf

$(OUT_NAME).bin: $(OUT_NAME).elf
	@echo "\\033[1;37;42m--- Generating $(@) ---\\033[0m"
	$(OBJCOPY) -O binary $(@:.bin=.elf) $(@)
	$(OBJDUMP) -d $(@:.bin=.elf) > $(@:.bin=.asm)

flash:
	@echo "\\033[1;37;42m--- Flashing $(OUT_NAME).bin to device ---\\033[0m"
	st-flash --reset write $(OUT_NAME).bin 0x8000000
clean:
	rm -fv $(OBJS) $(OBJS:.o=.d) $(OUT_NAME).* $(OUT_NAME)_jlink.*
