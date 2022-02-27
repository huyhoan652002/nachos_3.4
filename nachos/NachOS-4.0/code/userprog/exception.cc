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
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
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
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which) {
	case NoException: 
		break;
	case PageFaultException:
		printf("No valid translation found\n");
		ASSERT(FALSE);
		break;
	case ReadOnlyException:
		printf("Write attempted to page marked 'read-only'\n");
		ASSERT(FALSE);
		break;
	case BusErrorException:
		printf("Translation resulted in an invalid physical address\n");
		ASSERT(FALSE);
		break;
    case SyscallException:
		switch(type) {
		case SC_Halt:
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
			SysHalt();
			ASSERTNOTREACHED();
			break;
		case SC_Exec:
		case SC_Exit:
		case SC_Join:
		case SC_Create:
		case SC_Remove:
		case SC_Open:
		case SC_Read:
		case SC_Write:
		case SC_Seek:
		case SC_Close:
		case SC_ThreadFork:
		case SC_ThreadYield:
		case SC_ExecV:
		case SC_ThreadExit:
		case SC_ThreadJoin:
			DEBUG(dbgSys, "System call invoked.\n");
			kernel->currentThread->Finish();
			ASSERTNOTREACHED();
			break;
	DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

	SysHalt();

	ASSERTNOTREACHED();
	break;

      case SC_Add:
	DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
	
	/* Process SysAdd Systemcall*/
	int result;
	result = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4),
			/* int op2 */(int)kernel->machine->ReadRegister(5));

	DEBUG(dbgSys, "Add returning with " << result << "\n");
	/* Prepare Result */
	kernel->machine->WriteRegister(2, (int)result);
	
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;
	
	ASSERTNOTREACHED();

	break;

      default:
	cerr << "Unexpected system call " << type << "\n";
	break;
      }
      break;
    default:
      cerr << "Unexpected user mode exception" << (int)which << "\n";
      break;
    }
    ASSERTNOTREACHED();

	
}

int readNum() 
{
	char ch;
	int num = 0;
	while (1) {
		ch = kernel->machine->ReadRegister(4);
		if (ch == '\n') {
			break;
		}
		else if (ch == '\0') {
			break;
		}
		else {
			num = num * 10 + (ch - '0');
			kernel->machine->WriteRegister(4, kernel->machine->ReadRegister(4) + 1);
		}
	}
	return num;
}

void printNum(int number)
{
	int num = number;
	int count = 0;
	while (num > 0) {
		num = num / 10;
		count++;
	}
	for (int i = 0; i < count; i++) {
		int digit = number % 10;
		kernel->machine->WriteRegister(4, digit + '0');
		number = number / 10;
	}
	kernel->machine->WriteRegister(4, '\n');
}


