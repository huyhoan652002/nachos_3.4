#include "syscall.h"

int main()
{
    int fileID1 = Open("a.txt");
    int fileID2 = Open("b.txt");
    char temp;
    int size = Seek(-1, fileID1);
    Seek(0, fileID1);
    while (size > 0)
    {
        Read(&temp, 1, fileID1);
        Write(&temp, 1, fileID2);
        PrintString(&temp);
        size--;
    }
    Close(fileID1);
    Close(fileID2);
    Halt();
}