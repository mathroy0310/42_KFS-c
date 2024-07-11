/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 00:45:16 by maroy             #+#    #+#             */
/*   Updated: 2024/06/13 02:14:13 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kernel.h"

void log(char *str, ...)
{
    int *args;
    char *format;
    int i;

    args = (int *)(&str);
    format = (char *)(*args++);
    i = 0;
    while (format[i])
    {
        if (format[i] == '%')
        {
            i++;
            if (format[i] == 'c')
                outb(DEBUGCON_SERIAL_PORT, *args++);
            else if (format[i] == 's')
            {
                char *s = (char *)(*args++);
                while (*s)
                    outb(DEBUGCON_SERIAL_PORT, *s++);
            }
            else if (format[i] == 'd')
            {
                int num = *args++;
                char buf[32];
                int j = 0;
                if (num < 0)
                {
                    outb(DEBUGCON_SERIAL_PORT, '-');
                    num = -num;
                }
                do
                {
                    buf[j++] = '0' + num % 10;
                    num /= 10;
                } while (num);
                while (j > 0)
                    outb(DEBUGCON_SERIAL_PORT, buf[--j]);
            }
            else
            {
                outb(DEBUGCON_SERIAL_PORT, '%');
                outb(DEBUGCON_SERIAL_PORT, format[i]);
            }
        }
        else
            outb(DEBUGCON_SERIAL_PORT, format[i]);
        i++;
    }
    outb(DEBUGCON_SERIAL_PORT, '\n');
}