#include "syscall.h"

int main()
{
    int fileID = Open("hello.txt");
    char temp;
    int size = Seek(-1, fileID);
    Seek(0, fileID);
    while (size > 0)
    {
        Read(&temp, 1, fileID);
        PrintString(&temp);
        size--;
    }
    Close(fileID);
    Halt();
}