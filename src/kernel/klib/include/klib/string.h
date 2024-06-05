#ifndef _KSTRING_H
#define _KSTRING_H

#ifndef NULL
#define NULL '\0' /* ((void *)0) */
#endif

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define IS_NUMERIC(c) ((c) >= '0' && (c) <= '9')
#define IS_SPACE(c) ((c) == ' ')

int strcmp(const char *, const char *);
int strncmp(const char *, const char *, long long int);

#endif