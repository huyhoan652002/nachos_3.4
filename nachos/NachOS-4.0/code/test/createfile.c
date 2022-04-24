#include "syscall.h"

int main()
{
    int fd;
    char* fileName;
    PrintString("Enter filename: ");
    ReadString(fileName, 100);
    PrintString("\n");
    
    fd = Create(fileName);
    Halt();
}