#include <tamtypes.h>
#include "string.h"

int strlen(const char * str)
{
    const char *p = str;
    while (*p) p++;
    return p - str;
}

int strcpy(char *dest, const char *src, int n)
{
    int i = 0;
    if (!dest || !src) return 0;
    if (n == 0) return 0;

    for (i = 0; i < n - 1 && src[i] != '\0'; ++i) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return i;
}
