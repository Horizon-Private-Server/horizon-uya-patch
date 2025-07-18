#include <tamtypes.h>
#include "string.h"

int strlen(const char * str)
{
    const char *p = str;
    while (*p) p++;
    return p - str;
}