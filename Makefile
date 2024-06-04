# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maroy <maroy@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/04 13:01:20 by maroy             #+#    #+#              #
#    Updated: 2024/06/04 16:42:19 by maroy            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ARCH = i386
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfile \
		 -nodefaultlibs -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions
		 
ASM = nasm
AFLAGS = -f elf

LD = i686-elf-gcc
LDFLAGS = -T src/kernel/linker-i386.ld -lgcc -ffreestanding -O2


##########
# KERNEL #
##########
KERNEL_NAME = KFS.bin
KERNEL_SRC_DIR = src/kernel/arch/$(ARCH)/
KERNEL_OBJ_DIR = obj/
# for asm too
KERNEL_SRC += $(wildcard $(KERNEL_SRC_DIR)*.s)
# for c
KERNEL_SRC = $(wildcard $(KERNEL_SRC_DIR)*.c)

# for c 
KERNEL_OBJ = $(patsubst $(KERNEL_SRC_DIR)/%.c, $(KERNEL_OBJ_DIR)/%.o, $(KERNEL_SRC))
# for asm too 
KERNEL_OBJ += $(patsubst $(KERNEL_SRC_DIR)/%.s, $(KERNEL_OBJ_DIR)/%.o, $(KERNEL_SRC))

all: $(KERNEL_OBJ_DIR) $(KERNEL_NAME)

$(KERNEL_OBJ_DIR):
	mkdir -p $@

$(KERNEL_NAME): $(KERNEL_OBJ)
	$(LD) $(LDFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@
