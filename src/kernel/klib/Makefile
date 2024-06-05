# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maroy <maroy@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/04 21:14:18 by maroy             #+#    #+#              #
#    Updated: 2024/06/04 21:44:27 by maroy            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ARCH_FLAGS = -m32
CPU = -march=i386
LANG = -std=c89
CC = /usr/bin/gcc $(ARCH_FLAGS) $(CPU) $(LANG)
CFLAGS = -nostdlib -nodefaultlibs -fno-builtin -fno-exceptions -fno-stack-protector -Wall -Wextra -g3
AR = ar
ARFLAGS = rcs

LIBK_SRC = $(wildcard *.c)
LIBK_OBJ = $(patsubst %.c, %.o, $(LIBK_SRC))
CFLAGS += -I include
LIBK = libk.a

all: $(LIBK)

$(LIBK): $(LIBK_OBJ)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(LIBK_OBJ) $(LIBK)

.PHONY: all clean
