#include "syscall.h"

int main()
{
    char* filename1;
    char* filename2;
    int fileID1;
    int fileID2;
    char temp;
    int size;
    // filename1 = "a.txt";
    // filename2 = "b.txt";
    PrintString("Enter the name of the file you want to copy: ");
    ReadString(filename1, 33);
    PrintString("Enter the name of the destination file: ");
    ReadString(filename2, 33);
    
    fileID1 = Open(filename1);
    fileID2 = Open(filename2);
    size = Seek(-1, fileID1); // Size of the file you want to copy
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