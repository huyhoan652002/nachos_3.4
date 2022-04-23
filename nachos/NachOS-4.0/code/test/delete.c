#include "syscall.h"

int main()
{
    int fd;
    char* filename;
    
    PrintString("Enter file name: ");
    ReadString(filename, 33);
    // PrintString(filename);
    
    fd = Remove(filename);
    Halt();
}