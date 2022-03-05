#include "syscall.h"

int main()
{
    PrintString("Our team:\n", 100);
    PrintString("20127166 - Nguyen Huy Hoan\n", 100);
    PrintString("19120391 - To Vu Thai Hao\n", 100);
    PrintString(
        "ASCII: at directory code, run build.linux/nachos -x test/ascii to "
        "print the ASCII table\n", 200);
    PrintString(
        "Sort: at directory code, run build.linux/nachos -x test/bubblesort to "
        "start the sort program\n", 200);
    PrintString("\t- Enter n (the length of the array, 0 <= n <= 100)\n", 200);
    PrintString("\t- Enter n elements of the array\n", 200);
    PrintString(
        "\t- Enter the order you want to sort the array with (1: increasing, "
        "2: decreasing)\n", 200);
    PrintString(
        "\t- The program will print out the sorted array and then exit\n", 200);
    Halt();
}