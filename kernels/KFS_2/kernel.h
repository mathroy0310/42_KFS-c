/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 02:12:40 by maroy             #+#    #+#             */
/*   Updated: 2024/06/13 02:23:06 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __KERNEL_H__
#define __KERNEL_H__

#define KEYBOARD_PORT_DATA 0x60
#define KEYBOARD_PORT_STATUS 0x64
#define DEBUGCON_SERIAL_PORT 0xe9 /*DEBUG CON*/

#define GET_ESP(x) asm volatile("mov %%esp, %0" : "=r"(x)::)
#define GET_EBP(x) asm volatile("mov %%ebp, %0" : "=r"(x)::)

void log(char *str, ...);

#define true 1
#define false 0

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char value);
void print(char *text);
void switch_screen(int new_screen);
void *memcpy(void *dest, void const *src, int count);
void *memset(void *b, int c, unsigned int len);
void *memsetw(void *dest, short int val, int count);
int strlen(const char *s);
int strcmp(const char *s1, const char *s2);
void outb(unsigned short port, unsigned char value);
void outw(unsigned short port, unsigned short value);
void move_csr(void);
void clear(void);
void init();
void keyboard_handler(void);
void printstr(char *text);
void printchar(char c);
void shell(const char *cmd);
void printf(char *str, ...);
void hex_to_str(unsigned int value, char *str, unsigned long str_size);
void printaddr(unsigned int addr, int limit);

void gdt_init(void);

#endif