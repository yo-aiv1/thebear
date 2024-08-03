#include "../include/hashing.h"

#define K1          21
#define K2          5421

unsigned long HashStrW(const unsigned short *str) {
    unsigned long   result       = 0;
    char            CurrentValue = 0;
    int             idx          = 0;

    while (*str != 0x00) {
        if (*(str + 1) == 0x00) {
            CurrentValue = *str + 1;
        } else {
            CurrentValue = *str + *(str + 1);
        }

        result = (((CurrentValue * K2) / K1) + ((idx + *str) * (result - *(str + 1)))) >> 3;
        idx++;
        str++;
    }

    return result;
}

unsigned long HashStr(const char *str) {
    unsigned long   result       = 0;
    char            CurrentValue = 0;
    int             idx          = 0;

    while (*str != 0x00) {
        if (*(str + 1) == 0x00) {
            CurrentValue = *str + 1;
        } else {
            CurrentValue = *str + *(str + 1);
        }

        result = (((CurrentValue * K2) / K1) + ((idx + *str) * (result - *(str + 1)))) >> 3;
        idx++;
        str++;
    }

    return result;
}