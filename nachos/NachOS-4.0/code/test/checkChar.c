#include "syscall.h"

int main()
{
    // char c;
    // c = ReadChar();
    // PrintChar(c);

    // int n;
    // n = ReadNum();
    // PrintNum(n);

    char s[100];
    ReadString(s, 100);
    PrintString(s, 100);
    Halt();
}