/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 00:45:24 by maroy             #+#    #+#             */
/*   Updated: 2024/06/05 00:45:25 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void outb(unsigned short port, unsigned char value)
{
    __asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}
unsigned char inb(unsigned short port)
{
    unsigned char value;
    __asm__ __volatile__("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}