# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maroy <maroy@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/04 13:01:20 by maroy             #+#    #+#              #
#    Updated: 2024/06/05 00:51:29 by maroy            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


ARCH = i386
ARCH_FLAGS = -m32
CPU = -march=$(ARCH)
LANG = -std=c89

# **************************************************************************** #

CC = /usr/bin/gcc $(ARCH_FLAGS) $(CPU) $(LANG)
CFLAGS = -nostdlib -nodefaultlibs -fno-builtin -fno-exceptions -fno-stack-protector -Wall -Wextra -g3

NASM = /usr/bin/nasm
AFLAGS = -f elf

LD = /usr/bin/ld
LDFLAGS = -m elf_i386 -T kernels/linker-i386.ld

GRUB_CFG = grub/grub.cfg

KERNEL_OBJ_DIR = obj/

# **************************************************************************** #
##########
# KFS_1 #
##########
KFS_1 = KFS-1.bin
KFS_1_OBJ_DIR = $(KERNEL_OBJ_DIR)KFS_1/
KFS_1_SRC_DIR = kernels/KFS_1/
KFS_1_SRC = $(wildcard $(KFS_1_SRC_DIR)*.s) $(wildcard $(KFS_1_SRC_DIR)*.c) $(wildcard src/kernel/*.c)
KFS_1_OBJ = $(patsubst $(KFS_1_SRC_DIR)%.s, $(KFS_1_OBJ_DIR)%.o, $(filter %.s, $(KFS_1_SRC))) \
             $(patsubst $(KFS_1_SRC_DIR)%.c, $(KFS_1_OBJ_DIR)%.o, $(filter %.c, $(KFS_1_SRC)))
# **************************************************************************** #
##########
# KFS_2 #
##########
KFS_2 = KFS-2.bin
KFS_2_OBJ_DIR = $(KERNEL_OBJ_DIR)KFS_2/
KFS_2_SRC_DIR = kernels/KFS_2/
KFS_2_SRC = $(wildcard $(KFS_2_SRC_DIR)*.s) $(wildcard $(KFS_2_SRC_DIR)*.c) $(wildcard src/kernel/*.c)
KFS_2_OBJ = $(patsubst $(KFS_2_SRC_DIR)%.s, $(KFS_2_OBJ_DIR)%.o, $(filter %.s, $(KFS_2_SRC))) \
             $(patsubst $(KFS_2_SRC_DIR)%.c, $(KFS_2_OBJ_DIR)%.o, $(filter %.c, $(KFS_2_SRC)))

KERNELS = $(KFS_1) $(KFS_2)

#########
# IMAGE #
#########
ISO = KFS.iso
ISO_DIR = grub/iso/

all: $(KERNEL_OBJ_DIR) $(KERNELS) $(ISO)

# **************************************************************************** #

$(KERNEL_OBJ_DIR) $(KFS_1_OBJ_DIR) $(KFS_2_OBJ_DIR):
	@mkdir -p $@

# **************************************************************************** #
##########
# KFS_1 #
##########

$(KFS_1_OBJ_DIR)%.o: $(KFS_1_SRC_DIR)%.c | $(KFS_1_OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $(KFS_1_SRC_DIR)$< to $@"

$(KFS_1_OBJ_DIR)%.o: $(KFS_1_SRC_DIR)%.s | $(KFS_1_OBJ_DIR)
	@$(NASM) $(AFLAGS) $< -o $@
	@echo "Assembled $(KFS_1_SRC_DIR)$< to $@"

$(KFS_1): $(KFS_1_OBJ)
	@echo "Linking ..."
	@$(LD) $(LDFLAGS) -o $@ $^
	@echo "$@ binary is ready !"

# **************************************************************************** #
##########
# KFS_2 #
##########

$(KFS_2_OBJ_DIR)%.o: $(KFS_2_SRC_DIR)%.c | $(KFS_2_OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(KFS_2_OBJ_DIR)%.o: $(KFS_2_SRC_DIR)%.s | $(KFS_2_OBJ_DIR)
	@$(NASM) $(AFLAGS) $< -o $@

$(KFS_2): $(KFS_2_OBJ)
	@echo "Linking ..."
	@$(LD) $(LDFLAGS) -o $@ $^
	@echo "$@ binary is ready !"
	
# **************************************************************************** #

$(ISO): $(KERNELS)
	@echo "Creating the $(ISO_DIR) directory"
	@mkdir -pv $(ISO_DIR)/boot/grub
	@mv $(KERNELS) $(ISO_DIR)/boot
	@cp $(GRUB_CFG) $(ISO_DIR)/boot/grub
	@echo "Creating the $(ISO) file"
	@find $(ISO_DIR)/boot -name 'KFS*.bin' -exec bash -c 'grub-file --is-x86-multiboot "{}"' \;
	@grub-mkrescue -o $(ISO) $(ISO_DIR)
	@mv $(ISO) $(ISO_DIR)

# **************************************************************************** #

clean:
	@echo "Cleaning obj ..."
	@rm -rf $(KERNEL_OBJ_DIR)

fclean: clean
	@echo "Cleaning ISO and Bin ..."
	@rm -rf $(ISO_DIR)
# **************************************************************************** #

re: clean all

# **************************************************************************** #
run: $(ISO)
	@qemu-system-i386 -s -m 2G -M q35 -cdrom $(ISO_DIR)$(ISO) -boot d -debugcon stdio
	