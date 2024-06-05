/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 23:34:00 by maroy             #+#    #+#             */
/*   Updated: 2024/06/05 17:44:13 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define GET_ESP(x) asm volatile("mov %%esp, %0" : "=r"(x)::)
#define GET_EBP(x) asm volatile("mov %%ebp, %0" : "=r"(x)::)

void init();
void log(char *str, ...);
void keyboard_handler(void);
void gdt_init(void);
void kmain(void)
{
    gdt_init();

    init();
    log("Hello, world! Welcome to KFS-2");

    keyboard_handler();
}