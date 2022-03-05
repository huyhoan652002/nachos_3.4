#include "syscall.h"

int main()
{
    int i;
    for(i = 32; i < 127; i++)
    {
        // print the ascii characters in the range of [32, 127]
        PrintNum(i);
    }
}