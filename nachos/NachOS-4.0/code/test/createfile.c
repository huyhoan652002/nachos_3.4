#include "syscall.h"

int main()
{
    int fd = 1;
    
    PrintString("\n");
    PrintNum(fd);
    PrintString("\n");
    
    fd = Create("overGeared.txt");
    
    // Create("testCreateFile.txt");
    // Halt();
}