// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "directory.h"
#include "machine.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    if ((which == SyscallException) && (type == SC_Halt)) {
	DEBUG('a', "Shutdown, initiated by user program.\n");
   	interrupt->Halt();
    } else {
	printf("Unexpected user mode exception %d %d\n", which, type);
	ASSERT(FALSE);
    }

    switch (which) {
        case NoException: // khong co exception, dung chuong trinh
            break;
        case SyscallException:
        {
            switch (type) {
                case SC_Halt:
                    DEBUG('a', "Shutdown, initiated by user program.\n");
                    interrupt->Halt();
                    break;
                case SC_Exit:
                    DEBUG('a', "Exit, initiated by user program.\n");
                    interrupt->Halt();
                    break;
                case SC_Exec:
                    DEBUG('a', "Exec, initiated by user program.\n");
                    do_Exec();
                    break;
                case SC_Join:
                    DEBUG('a', "Join, initiated by user program.\n");
                    do_Join();
                    break;
                case SC_Create:
                    DEBUG('a', "Create, initiated by user program.\n");
                    do_Create();
                    break;
                case SC_Open:
                    DEBUG('a', "Open, initiated by user program.\n");
                    do_Open();
                    break;
                case SC_Read:
                    DEBUG('a', "Read, initiated by user program.\n");
                    do_Read();
                    break;
                case SC_Write:
                    DEBUG('a', "Write, initiated by user program.\n");
                    do_Write();
                    break;
                case SC_Close:
                    DEBUG('a', "Close, initiated by user program.\n");
                    do_Close();
                    break;
                case SC_Fork:
                    DEBUG('a', "Fork, initiated by user program.\n");
                    do_Fork();
                    break;
                case SC_Yield:
                    DEBUG('a', "Yield, initiated by user program.\n");
                    do_Yield();
                    break;
                case SC_CreateFile:
                    DEBUG('a', "CreateFile, initiated by user program.\n");
                    do_CreateFile();
                    break;
                case SC_OpenFile:
                    DEBUG('a', "OpenFile, initiated by user program.\n");
                    do_OpenFile();
                    break;
                case SC_ReadFile:
                    DEBUG('a', "ReadFile, initiated by user program.\n");
                    do_ReadFile();
        }
        /* print the error and halt the machine*/
        case PageFaultException:
            printf("PageFaultException\n");
            ASSERT(FALSE);
            break;
        case ReadOnlyException:
            printf("ReadOnlyException\n");
            ASSERT(FALSE);
            break;
        case BusErrorException:
            printf("BusErrorException\n");
            ASSERT(FALSE);
            break;
        
    }
}


