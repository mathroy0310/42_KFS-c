/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 02:12:40 by maroy             #+#    #+#             */
/*   Updated: 2024/06/13 02:17:02 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __KERNEL_H__
#define __KERNEL_H__

#define KEYBOARD_PORT_DATA 0x60
#define KEYBOARD_PORT_STATUS 0x64
#define DEBUGCON_SERIAL_PORT 0xe9 /*DEBUG CON*/

void log(char *str, ...);

#define true 1
#define false 0

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char value);
void print(char *text);
void switch_screen(int new_screen);
void *memcpy(void *dest, void const *src, int count);
void *memsetw(void *dest, short int val, int count);
int strlen(const char *s);
void outb(unsigned short port, unsigned char value);
void move_csr(void);
void init();
void log(char *str, ...);
void keyboard_handler(void);
void cls();

#endif