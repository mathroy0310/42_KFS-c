# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maroy <maroy@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/04 13:01:20 by maroy             #+#    #+#              #
#    Updated: 2024/06/05 01:28:06 by maroy            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


ARCH = i386
ARCH_FLAGS = -m32
CPU = -march=$(ARCH)

# **************************************************************************** #

CC = gcc $(ARCH_FLAGS) $(CPU)
CFLAGS = -nostdlib -nodefaultlibs -fno-builtin -fno-exceptions -fno-stack-protector -Wall -Wextra -g3

NASM = nasm
AFLAGS = -f elf

LD = ld
LDFLAGS = -m elf_i386 -T kernels/linker-i386.ld

GRUB_CFG = grub/grub.cfg # GRUB configuration file

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

KFS-1: $(KFS_1)

$(KFS_1_OBJ_DIR)%.o: $(KFS_1_SRC_DIR)%.c | $(KFS_1_OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $(KFS_1_SRC_DIR)$< to $@"

$(KFS_1_OBJ_DIR)%.o: $(KFS_1_SRC_DIR)%.s | $(KFS_1_OBJ_DIR)
	@$(NASM) $(AFLAGS) $< -o $@
	@echo "Assembled $(KFS_1_SRC_DIR)$< to $@"

$(KFS_1): $(KFS_1_OBJ)
	@$(LD) $(LDFLAGS) -o $@ $^ &>/dev/null
	@echo -e "$(GREEN)$@ DONE$(NC)"

# **************************************************************************** #
##########
# KFS_2 #
##########

KFS-2: $(KFS_2)

$(KFS_2_OBJ_DIR)%.o: $(KFS_2_SRC_DIR)%.c | $(KFS_2_OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $(KFS_2_SRC_DIR)$< to $@"

$(KFS_2_OBJ_DIR)%.o: $(KFS_2_SRC_DIR)%.s | $(KFS_2_OBJ_DIR)
	@$(NASM) $(AFLAGS) $< -o $@
	@echo "Assembled $(KFS_2_SRC_DIR)$< to $@"

$(KFS_2): $(KFS_2_OBJ)
	@$(LD) $(LDFLAGS) -o $@ $^ &>/dev/null
	@echo -e "$(GREEN)$@ DONE$(NC)"
	
# **************************************************************************** #

iso: $(ISO)

$(ISO): $(KERNELS)
	@echo "Creating the $(ISO_DIR) directory"
	@mkdir -pv $(ISO_DIR)/boot/grub > /dev/null
	@mv $(KERNELS) $(ISO_DIR)/boot
	@cp $(GRUB_CFG) $(ISO_DIR)/boot/grub
	@echo "Creating the $(ISO) file"
	@xorriso -outdev 'stdio:KFS.iso' &>/dev/null
	@find $(ISO_DIR)/boot -name 'KFS*.bin' -exec bash -c 'grub-file --is-x86-multiboot "{}"' \;
	@grub-mkrescue -o $(ISO) $(ISO_DIR) &>/dev/null
	@mv $(ISO) $(ISO_DIR)
	@echo -e "$(GREEN)$@ DONE$(NC)"

# **************************************************************************** #

clean:
	@rm -rf $(KERNEL_OBJ_DIR)
	@echo -e "$(GREEN)$@ DONE$(NC)"

fclean: clean
	@rm -rf $(ISO_DIR)
	@echo -e "$(GREEN)$@ DONE$(NC)"
# **************************************************************************** #

re: fclean all

# **************************************************************************** #
run:
	@echo -e "$(CYAN)QEMU Booting KFS..."
	@echo -e "$(YELLOW)KFS LOG:$(NC)" 
	@qemu-system-i386 -s -m 2G -M q35 -cdrom $(ISO_DIR)$(ISO) -boot d -debugcon stdio
	
CYAN=\033[0;36m
GREEN=\033[0;32m
YELLOW=\033[0;33m
NC=\033[0m