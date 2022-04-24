#include "syscall.h"

int main()
{
    int fd = 1;
    char* filename;
    
    PrintString("Enter file name: ");
    ReadString(filename, 33);
    
    fd = Create(filename);
    
    // Create("testCreateFile.txt");
    // Halt();
}