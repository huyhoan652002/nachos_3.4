#include <syscall.h>
#include <stdio.h>

int main()
{
    int i;
    for(i = 32; i < 127; i++)
    {
        PrintNum(i);
        PrintChar(' ');
        PrintChar((char)i);
        PrintChar('\n');
    }
}