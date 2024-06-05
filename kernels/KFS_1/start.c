/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 23:34:00 by maroy             #+#    #+#             */
/*   Updated: 2024/06/05 14:40:06 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void outb(unsigned short port, unsigned char value);
void init();
unsigned int k_printf(char *message, unsigned int line);
void log(char *str, ...);
void keyboard_handler(void);

void kmain(void)
{
    init();

    log("Hello, world! Welcome to KFS-1");
    keyboard_handler();
}