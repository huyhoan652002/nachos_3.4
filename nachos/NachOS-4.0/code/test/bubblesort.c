#include "syscall.h"
#define SIZE (100)

int main() {
    int n, a[SIZE + 1], order, temp, i, j;
    do {
        PrintString("Enter n (0 <= n <= 100): ", 100);
        n = ReadNum();
        if (n < 0 || n > 100)
            PrintString(
                "n has to be an integer between 1 and 100 (inclusive), please "
                "try again\n", 100);
    } while (n < 0 || n > 100);

    for (i = 0; i < n; i++) {
        PrintString("Enter a[", 100);
        PrintNum(i);
        PrintString("]: ", 100);
        a[i] = ReadNum();
    }

    do {
        PrintString("Enter sort order (1: increasing, 2: decreasing): ", 100);
        order = ReadNum();
        if (order != 1 && order != 2)
            PrintString("Wrong input, please try again\n", 100);
    } while (order != 1 && order != 2);

    for (i = 0; i < n; i++) {
        for (j = 0; j < n - 1; j++) {
            if (order == 1) {
                if (a[j] > a[j + 1]) {
                    temp = a[j];
                    a[j] = a[j + 1];
                    a[j + 1] = temp;
                }
            } else if (order == 2) {
                if (a[j] < a[j + 1]) {
                    temp = a[j];
                    a[j] = a[j + 1];
                    a[j + 1] = temp;
                }
            }
        }
    }

    PrintString("Sorted array: ", 100);
    for (i = 0; i < n; i++) {
        PrintNum(a[i]);
        PrintChar(' ');
    }
    Halt();
}