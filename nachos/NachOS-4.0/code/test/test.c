#include "syscall.h"

int main()
{
    int fileID = Open("hello.txt");
    Close(fileID);
    Halt();
}