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