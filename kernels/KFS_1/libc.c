/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 02:13:43 by maroy             #+#    #+#             */
/*   Updated: 2024/06/13 02:13:46 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kernel.h"

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