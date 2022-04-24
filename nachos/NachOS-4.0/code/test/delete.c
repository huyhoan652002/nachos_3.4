#include "syscall.h"

// kiểm tra system call Remove

int main()
{
    char *filename;
    int del;

    PrintString("Enter file name: ");
    ReadString(filename, 33);
    // PrintString(filename);

    del = Remove(filename);

    Halt();
}