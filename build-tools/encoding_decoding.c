#include <stdio.h>

#define KEY 5

void decode(unsigned char *str) {
    int TempKey = KEY;

    while (*str != 0x00) {
        if (TempKey < 0) {
            TempKey = TempKey * -1;
        } else if (TempKey == 0) {
            TempKey = KEY;
        }

        *str = *str - TempKey;
        TempKey = *str - TempKey;
        str++;
    }
}

void encode(unsigned char *str) {
    int TempKey = KEY;

    while (*str != 0x00) {
        if (TempKey < 0) {
            TempKey = TempKey * -1;
        } else if (TempKey == 0) {
            TempKey = KEY;
        }

        unsigned char CurrentChar = *str;

        *str = *str + TempKey;
        TempKey = CurrentChar - TempKey;
        str++;
    }
}
