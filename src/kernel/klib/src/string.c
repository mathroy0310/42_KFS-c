int strcmp(const char *str1, const char *str2)
{
    while (*str1)
    {
        if (*str1 != *str2)
            return 1;
        str1++;
        str2++;
    }
    if (!(*str2))
        return 0;
    return 1;
}

int strncmp(const char *str1, const char *str2, long long int n)
{
    while (n > 0)
    {
        if (*str1 != *str2)
            return 1;
        str1++;
        str2++;
        n--;
    }
    return 0;
}
