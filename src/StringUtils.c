#include "../include/StringUtils.h"


int lenW(const unsigned short *string) {
    const unsigned short *start = string;

    while (*string != 0x0000) {
        string++;        
    }
    return string - start;
}

int len(const unsigned char *string) {
    const unsigned char *start = string;

    while (*string != 0x00) {
        string++;        
    }
    return string - start;
}


void ConcatStringW(unsigned short *dest, unsigned short *src, int DestIdx) {
    while (*src != 0x0000) {
        dest[DestIdx] = *src;
        DestIdx++;
        src++;
    }
    dest[DestIdx] = 0x0000;
}


void ConcatString(unsigned char *dest, unsigned char *src, int DestIdx) {
    while (*src != 0x00) {
        dest[DestIdx] = *src;
        DestIdx++;
        src++;
    }
    dest[DestIdx] = 0x00;
}


int IntLen(int num) {
    int i, len = 0, n;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    return len;
}

void IntToString(char *str, int num) {
    int i, rem;
    int len = IntLen(num);

    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

int TotalBufferLength(unsigned short *FileName, int FileSize) {
    return 310 + (8 + lenW(FileName)) + (20 + IntLen(FileSize)) + FileSize;
}

void WideToNormal(char *OutBuffer, unsigned short *String) {
    while (*String != 0x0000)
    {
        *OutBuffer = *String;
        String++;
        OutBuffer++;
    }
    *OutBuffer = 0x00;
}