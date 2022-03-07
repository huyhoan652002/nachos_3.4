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
//	/* Added by students: */
// 	It seems that register r0 is the kernel stack pointer
//	/* End of student code */
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------

// Input: address of User(int), limit of buffer(int)
// Output: a recoreded Buffer (char*)
// Function: Copy memory from User to Kernel
char *User2System(int virtAddr, int limit)
{
	int i; // index
	int oneChar;
	char *kernelBuf = NULL;
	kernelBuf = new char[limit + 1]; // need for terminal string

	if (kernelBuf == NULL)
		return kernelBuf;

	memset(kernelBuf, 0, limit + 1);
	// printf("\n Filename u2s:");

	for (i = 0; i < limit; i++)
	{
		kernel->machine->ReadMem(virtAddr + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		// printf("%c",kernelBuf[i]);
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}

// Input: address of User(int), limit of buffer(int), buffer(char*) that store the data
// Output: So byte da sao chep(int)
// Function: Copy memory from System memory to User memory (buffer to virtAddr)
int System2User(int virtAddr, int len, char* buffer)
{
	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0;
	do{
		oneChar = (int)buffer[i];
		kernel->machine->WriteMem(virtAddr + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
	return i;
}

void ProgramCounter()
{
	/* set previous programm counter (debugging only)*/
	kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

	/* set next programm counter for brach execution */
	kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
}

void ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);

	DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

	switch (which)
	{
	case NoException:
		break;
	case PageFaultException:
		printf("No valid translation found\n");
		ASSERT(FALSE); // assert means something is wrong, but we're not trying to fix it here so we can just ignore it and keep going (we'll crash later if we really want to fix it)
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
		switch (type)
		{
		case SC_Halt:
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
			SysHalt();
			ASSERTNOTREACHED(); // ASSERTNOTREACHED() means something is wrong, but we're not trying to fix it here so we can just ignore it and keep going (we'll crash later if we really want to fix it)
			break;
		case SC_Exec:
			DEBUG(dbgSys, "Exec system call.\n");
		case SC_Exit:
			DEBUG(dbgSys, "Exit system call.\n");
			SysExit();
			break;
		case SC_Join:
			DEBUG(dbgSys, "Join system call.\n");
			//SysJoin(type);
			ASSERTNOTREACHED();
			break;
		case SC_Create:
			DEBUG(dbgSys, "Create system call.\n");
			break;
		case SC_Remove:
			DEBUG(dbgSys, "Remove system call.\n");
			// SysRemove((char *)kernel->machine->ReadRegister(4));
			ASSERTNOTREACHED();
			break;
		case SC_Open:
			DEBUG(dbgSys, "Open system call.\n");
			// SysOpen((char *)kernel->machine->ReadRegister(4));
			ASSERTNOTREACHED();
			break;
		case SC_Read:
			DEBUG(dbgSys, "Read system call.\n");
			// SysRead((char *)kernel->machine->ReadRegister(4), (char *)kernel->machine->ReadRegister(5), (int)kernel->machine->ReadRegister(6));
			ASSERTNOTREACHED();
			break;
		case SC_Write:
			DEBUG(dbgSys, "Write system call.\n");
			// SysWrite((char *)kernel->machine->ReadRegister(4), (char *)kernel->machine->ReadRegister(5), (int)kernel->machine->ReadRegister(6));
			ASSERTNOTREACHED();
			break;
		case SC_Seek:
			DEBUG(dbgSys, "Seek system call.\n");
			// SysSeek((int)kernel->machine->ReadRegister(4), (int)kernel->machine->ReadRegister(5));
			ASSERTNOTREACHED();
			break;
		case SC_Close:
			DEBUG(dbgSys, "Close system call.\n");
			// SysClose((int)kernel->machine->ReadRegister(4));
			ASSERTNOTREACHED();
			break;
		case SC_ThreadFork:
			DEBUG(dbgSys, "ThreadFork system call.\n");
			// SysThreadFork((char *)kernel->machine->ReadRegister(4));
			ASSERTNOTREACHED();
			break;
		case SC_ThreadYield:
			DEBUG(dbgSys, "ThreadYield system call.\n");
			// SysThreadYield();
			ASSERTNOTREACHED();
			break;
		case SC_ExecV:
			DEBUG(dbgSys, "ExecV system call.\n");
			// SysExecV((char *)kernel->machine->ReadRegister(4));
			ASSERTNOTREACHED();
			break;
		case SC_ThreadExit:
			DEBUG(dbgSys, "ThreadExit system call.\n");
			// SysThreadExit();
			ASSERTNOTREACHED();
			break;
		case SC_ThreadJoin:
			DEBUG(dbgSys, "System call invoked.\n");
			kernel->currentThread->Finish();
			ASSERTNOTREACHED();
			break;
		case SC_Add:
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

			/* Process SysAdd Systemcall*/
			int result;
			result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
							/* int op2 */ (int)kernel->machine->ReadRegister(5));

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
				kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
			}

			return;

			ASSERTNOTREACHED();

			break;
		// readnum means read the number of characters in the file and store it in the buffer pointed to by buffer
		case SC_ReadNum:
		{
			int addr = kernel->machine->ReadRegister(4);
			char *buffer = User2System(addr, 200);
			kernel->ReadNum(2);
			delete[] buffer;
			ProgramCounter();
			break;
		}
		// printNum methods means print the number of characters in the file and store it in the buffer pointed to by buffer
		case SC_PrintNum:
		{
			int number = kernel->machine->ReadRegister(4);
			kernel->PrintNum(number);
			ProgramCounter();
			break;
		}
		case SC_RandomNumber:
		{
			kernel->RandomNumber();
			ProgramCounter();
			break;
		}
		// readString method means that read the string in the file and store it in the buffer pointed to by buffer
		case SC_ReadString:
		{
			// Input: buffer address (char*) to be read, length of the string
			// Output: none
			// Read the string that the user entered and store it in the buffer 
			
			int addr = kernel->machine->ReadRegister(4);
			int length = kernel->machine->ReadRegister(5);			
			char *buffer = User2System(addr, 200);

			kernel->ReadString(addr, buffer, 200);

			// System2User(addr, length, buffer); // Copy chuoi tu vung nho System Space sang vung nho User Space
			
			delete[] buffer;
			ProgramCounter();
			break;
		}
		// printString means that the string in the file and store it in the buffer pointed to by buffer
		case SC_PrintString:
		{
			int addr = kernel->machine->ReadRegister(4);
			char *buffer = User2System(addr, 200);
			kernel->PrintBuffer(buffer, 200);
			
			cout << endl;
			delete[] buffer;
			ProgramCounter();
			break;
		}
		// Read a char that the user inputed
		case SC_ReadChar:
		{
			int addr = kernel->machine->ReadRegister(4);
			char *buffer = User2System(addr, 200);

			kernel->ReadChar(2);

			delete[] buffer;

			ProgramCounter();
			break;
		}

		case SC_PrintChar:
		{
			int character = kernel->machine->ReadRegister(4);
			kernel->PrintChar(character);
			cout << "\n";
			ProgramCounter();
			break;
		}
		default:
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
		break;

	case AddressErrorException:
		//	Add your implementation here
		break;
	case OverflowException:
		//	Add your implementation here
		break;

	case IllegalInstrException:
		//	Add your implementation here
		cerr << "Illegal Instruction.\n";
		break;

	case NumExceptionTypes:
		//	Add your implementation here
		cerr << "Unexpected Exception.\n";
		break;

	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
	}
}
