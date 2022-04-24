#include "syscall.h"

int main()
{
    char* filename;
    int del;
    PrintString("Enter filename: ");
    ReadString(filename, 100);
    del = Remove("a.txt");
    Halt();
}