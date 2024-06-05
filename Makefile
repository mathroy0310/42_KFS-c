# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maroy <maroy@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/04 13:01:20 by maroy             #+#    #+#              #
#    Updated: 2024/06/04 22:20:18 by maroy            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ARCH = i386
ARCH_FLAGS = -m32
CPU = -march=$(ARCH)
LANG = -std=c89
CC = /usr/bin/gcc $(ARCH_FLAGS) $(CPU) $(LANG)
CFLAGS = -I include -nostdlib -nodefaultlibs -fno-builtin -fno-exceptions -fno-stack-protector -Wall -Wextra -g3

NASM = /usr/bin/nasm
AFLAGS = -f elf

LD = /usr/bin/ld
LDFLAGS = -m elf_i386 -T src/kernel/linker-i386.ld

GRUB_CFG = grub/grub.cfg

LIBK_DIR = src/kernel/klib
LIBK = $(LIBK_DIR)/libk.a


##########
# KERNEL #
##########
KERNEL = KFS-1.bin
KERNEL_SRC_DIR = src/kernel/arch/$(ARCH)/
KERNEL_OBJ_DIR = obj/

KERNEL_SRC = $(wildcard $(KERNEL_SRC_DIR)*.s) $(wildcard $(KERNEL_SRC_DIR)*.c)
KERNEL_OBJ = $(patsubst $(KERNEL_SRC_DIR)%.s, $(KERNEL_OBJ_DIR)%.o, $(filter %.s, $(KERNEL_SRC))) \
             $(patsubst $(KERNEL_SRC_DIR)%.c, $(KERNEL_OBJ_DIR)%.o, $(filter %.c, $(KERNEL_SRC)))

#########
# IMAGE #
#########
ISO = KFS.iso
ISO_DIR = build

all: $(LIBK) $(KERNEL_OBJ_DIR) $(KERNEL) $(ISO)

$(KERNEL_OBJ_DIR):
	@mkdir -p $@

$(KERNEL): $(KERNEL_OBJ) $(LIBK)
	@echo "Linking ..."
	@$(LD) $(LDFLAGS) -o $@ $^
	@echo "$@ binary is ready !"

$(ISO): $(KERNEL)
	@echo "Creating the $(ISO_DIR) directory"
	@mkdir -pv $(ISO_DIR)/boot/grub
	@cp $(KERNEL) $(ISO_DIR)/boot
	@cp $(GRUB_CFG) $(ISO_DIR)/boot/grub
	@echo "Creating the $(ISO) file"
	@grub-file --is-x86-multiboot $(ISO_DIR)/boot/$(KERNEL)
	@grub-mkrescue -o $(ISO) $(ISO_DIR)
	@rm -rf $(ISO_DIR)

clean:
	@echo "Cleaning obj ..."
	@rm -rf $(KERNEL_OBJ_DIR)
	@$(MAKE) -C $(LIBK_DIR) clean

fclean: clean
	@echo "Cleaning kfs ..."
	@rm -rf $(KERNEL) $(ISO)

re: fclean all

run: $(ISO)
	@qemu-system-i386 -s -m 2G -M q35 -cdrom $(ISO) -boot d -debugcon stdio

$(KERNEL_OBJ_DIR)%.o: $(KERNEL_SRC_DIR)%.c | $(KERNEL_OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_OBJ_DIR)%.o: $(KERNEL_SRC_DIR)%.s | $(KERNEL_OBJ_DIR)
	@$(NASM) $(AFLAGS) $< -o $@

$(LIBK):
	@$(MAKE) -C $(LIBK_DIR)
	@cp -rn $(LIBK_DIR)/include/klib include
	