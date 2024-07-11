/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 02:19:54 by maroy             #+#    #+#             */
/*   Updated: 2024/06/13 02:19:55 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kernel.h"

void *memset(void *b, int c, unsigned int len)
{
    unsigned int i;

    i = 0;
    while (i < len)
    {
        ((char *)(b))[i] = c;
        i++;
    }
    return b;
}


int strcmp(const char *s1, const char *s2)
{
    int i;

    if (!s1 || !s2)
        return (-1);
    i = 0;
    while (s1[i] == s2[i])
    {
        if (s1[i] == '\0')
            return (0);
        i++;
    }
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int intlen(int n)
{
    int i;

    i = 1;
    if (n < 0)
    {
        n = -n;
        i++;
    }
    while (n >= 10)
    {
        n = n / 10;
        i++;
    }
    return (i);
}

void itoa(int n, char *str)
{
    int nb;
    int i;
    int len;

    nb = n;
    len = intlen(n);
    memset(str, 0, len + 1);
    if (nb < 0)
        nb = -nb;
    i = len - 1;
    if (nb == 0)
    {
        str[i] = '0';
        return;
    }
    while (nb != 0)
    {
        str[i--] = (nb % 10) + '0';
        nb = nb / 10;
    }
    if (n < 0)
        str[i] = '-';
}

void *memsetw(void *dest, short int val, int count)
{
    /* Memory set in 16-bytes env */
    register char *d = dest;
    while (count-- > 0)
        *d++ = val;
    return dest;
}

void *memcpy(void *dest, void const *src, int count)
{
    /* Copies memory from src to dest */
    char *d = dest;
    char const *s = src;
    while (count-- > 0)
        *d++ = *s++;
    return dest;
}

int strlen(const char *s)
{
    /* Return length of a given string */
    int n;
    n = 0;
    while (*s++)
        n++;
    return n;
}

void hex_to_str(unsigned int value, char *str, unsigned long str_size);
void printstr(char *text);
void printchar(char c);
void printnbr(int n);
void printf(char *str, ...)
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
                printchar(*args++);
            else if (format[i] == 's')
            {
                printstr(*((char **)args++));
            }
            else if (format[i] == 'd')
            {
                printnbr(*args++);
            }
            else if (format[i] == 'p')
            {
                char tmp_addr[9];
                memset(tmp_addr, 0, sizeof(tmp_addr));
                hex_to_str(*args++, tmp_addr, sizeof(tmp_addr));
                printstr("0x");
                printstr(tmp_addr);
            }
            else
            {
                printchar('%');
                printchar(format[i]);
            }
        }
        else
        {
            printchar(format[i]);
        }
        i++;
    }
}