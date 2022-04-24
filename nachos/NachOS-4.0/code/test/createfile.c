#include "syscall.h"

// Kiểm tra system call Create. 
// Bạn sẽ cho người dùng nhập tên file vào từ console từ ReadString

int main()
{
    int fd = 1;    
    char* filename;
    
    PrintString("Enter file name: ");
    ReadString(filename, 33);
    // PrintString(filename);
    
    fd = Create(filename);
    
    // fd = Create("overGeared.txt");    
    // Create("testCreateFile.txt");
    Halt();
}