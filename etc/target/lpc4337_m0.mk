# Copyright 2016, Pablo Ridolfi
# All rights reserved.
#
# This file is part of Workspace.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from this
#    software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
################################################################################
#                Target Makefile for LPC4337 microcontroller                   #
################################################################################

# Target name
TARGET_NAME := lpc4337_m0

# Default cross-toolchain
CROSS_COMPILE ?= arm-none-eabi-

# variables de rutas o carpetas
OUT_PATH = out/$(TARGET_NAME)
OBJ_PATH = $(OUT_PATH)/obj

# Defined symbols
SYMBOLS += -DDEBUG -DCORE_M0 -D__USE_LPCOPEN -D__LPC43XX__ -D__CODE_RED \
           -D__MULTICORE_M0APP -DCORE_M0APP

# Compilation flags
CFLAGS  := -Wall -ggdb3 -mcpu=cortex-m0 -mthumb -fdata-sections -ffunction-sections

# Linking flags
LFLAGS  := -nostdlib -fno-builtin -mcpu=cortex-m0 -mthumb \
			  -Xlinker -Map=$(OUT_PATH)/firmware.map \
			  -Wl,--gc-sections

# Linker scripts
LD_FILE := -Tetc/ld/lpc4337_m0_lib.ld -Tetc/ld/lpc4337_m0_mem.ld \
           -Tetc/ld/lpc4337_m0.ld

# OpenOCD configuration file
CFG_FILE := etc/openocd/lpc4337.cfg

# Flash base address for OpenOCD download rule
BASE_ADDR := 0x1B000000

# Download command
DOWNLOAD_CMD := openocd -f $(CFG_FILE) -c "init" -c "halt 0" -c "flash write_image erase unlock $(OUT_PATH)/firmware.bin $(BASE_ADDR) bin" -c "reset run" -c "shutdown"

# Post-build command
POST_BUILD_CMD += $(CROSS_COMPILE)objcopy --target elf32-littlearm --verbose --strip-all --remove-section=.ARM.attributes --redefine-sym __vectors_start__=__vectors_start___core_m0slave --keep-symbol __vectors_start___core_m0slave --redefine-sym _data=__start_data_core_m0slave --keep-symbol __start_data_core_m0slave --redefine-sym _edata=__end_data_core_m0slave --keep-symbol __end_data_core_m0slave --remove-section=".bss*" --remove-section=".noinit*" --rename-section .ARM.exidx=".core_m0slave.ARM.exidx" --rename-section .ARM.extab=".core_m0slave.ARM.extab" --rename-section .text=".core_m0slave" --rename-section .data=".core_m0slave.data" --rename-section .data_RAM2=".core_m0slave.data_RAM2" --rename-section .data_RAM3=".core_m0slave.data_RAM3" "$(OUT_PATH)/firmware.axf" "$(OUT_PATH)/firmware.axf.o"	\
					&& dd if=/dev/zero of="$(OUT_PATH)/firmware.axf.o" bs=1 seek=16 conv=notrunc count=2

# Erase command
ERASE_CMD := openocd -f $(CFG_FILE) -c "init" -c "halt 0" -c "flash erase_sector 0 0 last" -c "exit"
