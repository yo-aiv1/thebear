#include <stdio.h>

unsigned long hashstr(char *str) {
    unsigned long result = 0;
    int idx = 1, temp = 0, CurrentValue = 0;

    while (*str != 0x00) {
        if (*(str + 1) == 0x00) {
            if (idx > 20) {
                idx = idx % 10;
            }
            CurrentValue = (*str << idx) ^ CurrentValue;
        } else if (*(str + 2) == 0x00) {
            CurrentValue = (*(str + 1) * idx) << 1;
        } else {
            CurrentValue = *str * ((*(str + 2) + idx));
        }

        result += *str << 2;
        result += CurrentValue;
        result += idx * CurrentValue;
        temp += CurrentValue;
        str++;
        idx++;
    }

    return result ^ temp;
}

int main(int argc, char *argv[]) {
    unsigned long hash = hashstr(argv[1]);
    printf("0x%lx\n", hash);
}
