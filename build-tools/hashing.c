#include <stdio.h>
#include <string.h>

#define K1          21
#define K2          5421

unsigned long hashstr(char *str) {
    unsigned long result = 0;
    char CurrentValue = 0;
    int idx = 0;

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

int main(int argc, char *argv[]) {
    unsigned long hash = hashstr(argv[1]);
    printf("0x%lx\n", hash);
}
