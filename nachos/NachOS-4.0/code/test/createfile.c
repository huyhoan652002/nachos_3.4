#include "syscall.h"

int main()
{
    int fd = 1;
    
    PrintString("\n",3);
    PrintNum(fd);
    PrintString("\n",3);
    
    fd = Create("overGeared.txt");
    
    // Create("testCreateFile.txt");
    // Halt();
}