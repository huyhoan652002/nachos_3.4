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
int System2User(int virtAddr, int len, char *buffer)
{
	if (len < 0)
		return -1;
	if (len == 0)
		return len;
	int i = 0;
	int oneChar = 0;
	do
	{
		oneChar = (int)buffer[i];
		kernel->machine->WriteMem(virtAddr + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
	return i;
}

// increase the pc
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
		return;

	case PageFaultException:
	{
		DEBUG(dbgAddr, "No valid translation found\n");
		printf("\nNo valid translation found\n");
		kernel->interrupt->Halt();

		// ASSERT(FALSE);
		// assert means something is wrong, but we're not trying to fix it here so we can just ignore it and keep going (we'll crash later if we really want to fix it)
		break;
	}

	case ReadOnlyException:
	{
		DEBUG(dbgAddr, "Write attempted to page marked read-only\n");
		printf("\nWrite attempted to page marked 'read-only'\n");
		kernel->interrupt->Halt();

		break;
	}

	case BusErrorException:
	{
		DEBUG(dbgAddr, "Translation resulted in an invalid physical address\n");
		printf("Translation resulted in an invalid physical address\n");
		kernel->interrupt->Halt();

		break;
	}
	case AddressErrorException:
	{
		DEBUG(dbgAddr, "Unaligned reference or one that was beyond the end of the address space\n");
		printf("Unaligned reference or one that was beyond the end of the address space\n");
		kernel->interrupt->Halt();

		break;
	}
	case OverflowException:
	{
		DEBUG(dbgAddr, "Integer overflow in add or sub.\n");
		printf("Integer overflow in add or sub.\n");
		kernel->interrupt->Halt();

		break;
	}
	case IllegalInstrException:
	{
		DEBUG(dbgAddr, "Unimplemented or reserved instr.\n");
		printf("Unimplemented or reserved instr.\n");
		kernel->interrupt->Halt();

		// cerr << "Illegal Instruction.\n";
		break;
	}
	case NumExceptionTypes:
	{
		DEBUG(dbgAddr, "NumExceptionTypes\n");
		printf("NumExceptionTypes\n");
		kernel->interrupt->Halt();

		break;
	}

	case SyscallException:
		switch (type)
		{
		case SC_Halt:
			// Input: Khong co
			// Output: Thong bao tat may
			// Chuc nang: Tat HDH
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
			cout << endl;
			SysHalt();
			// ASSERTNOTREACHED();
			// ASSERTNOTREACHED() means something is wrong, but we're not trying to fix it here so we can just ignore it and keep going (we'll crash later if we really want to fix it)
			break;
		case SC_Exec:
		{
			// DEBUG(dbgSys, "Exec system call.\n");

			// Input: vi tri int
			// Output: Fail return -1, Success: return id cua thread dang chay
			// SpaceId Exec(char *name);
			int virtAddr = kernel->machine->ReadRegister(4);		  // doc dia chi ten chuong trinh tu thanh ghi r4
			char *name = User2System(virtAddr, MAX_FILENAME_LEN + 1); // Lay ten chuong trinh, nap vao kernel

			if (name == NULL)
			{
				DEBUG('a', "\n Not enough memory in System");
				printf("\n Not enough memory in System");
				kernel->machine->WriteRegister(2, -1);
				// IncreasePC();
				return;
			}

			delete[] name;
			ProgramCounter();
			return;
			break;
		}
		case SC_Exit:
			DEBUG(dbgSys, "Exit system call.\n");
			SysExit();
			break;
		case SC_Join:
			DEBUG(dbgSys, "Join system call.\n");
			// SysJoin(type);
			ASSERTNOTREACHED();
			break;
		case SC_Create:
			/* Create a Nachos file, with name "name" */
			/* Note: Create does not open the file.   */
			/* Return 1 on success, negative error code on failure */
			{
				// Input: address of User(int), limit of buffer(int)
				// Output: -1 if error, 0 if success
				// Function: Create a file with name "name" (the file name is the string pointed to by virtAddr)
				DEBUG(dbgSys, "Create system call.\n");

				DEBUG(dbgSys, "Reading virtual address of filename\n");
				int addr = kernel->machine->ReadRegister(4);

				DEBUG(dbgSys, "Reading filename\n");
				char *filename = User2System(addr, MAX_FILENAME_LEN);

				if (strlen(filename) == 0)
				{
					printf("\nFilename is empty\n");
					DEBUG(dbgAddr, "Filename is not valid\n");
					kernel->machine->WriteRegister(2, -1); // return -1 to user program

					break;
				}

				if (filename == NULL) // if can't read filename
				{
					printf("\nNot enough memory in system\n");
					DEBUG(dbgAddr, "Not enough memory in system\n");
					kernel->machine->WriteRegister(2, -1); // return -1 to user program

					delete filename;
					break;
				}

				DEBUG(dbgAddr, "Filename is valid\n");

				bool success = kernel->fileSystem->Create(filename, 0);
				if (!success)
				{
					// Fail to create file
					printf("\nCan't create file '%s'\n", filename);
					DEBUG(dbgAddr, "Can't create file\n");
					kernel->machine->WriteRegister(2, -1); // return -1 to user program

					delete filename;
					break;
				}
				else
				{
					// Success to create file
					kernel->machine->WriteRegister(2, 0); // return 0 to user program
					printf("\nSuccess to create file '%s'\n", filename);

					delete filename;
					break;
				}
			}

		case SC_Remove:
		{
			DEBUG(dbgSys, "Remove system call.\n");
			int virArr = kernel->machine->ReadRegister(4);
			char* filename = User2System(virArr, MAX_FILENAME_LEN + 1);
			
			if (strlen(filename) == 0)
			{
				printf("\nFilename is empty\n");
				DEBUG(dbgAddr, "Filename is not valid\n");
				kernel->machine->WriteRegister(2, -1); // return -1 to user program

				break;
			}

			if (filename == NULL)
			{
				printf("\nName should not be null.\n");
				DEBUG(dbgAddr, "Name should not be null.\n");
				
				kernel->machine->WriteRegister(2, -1); // return -1 to user program
				
				delete filename;
				break;
			}

			if (kernel->isOpen(filename))	//  If the file is open, stop removing.
			{
				printf("\nThe file is now open. Close the file to remove.\n");
				DEBUG(dbgAddr, "The file is now open. Close the file to remove.\n");
				
				kernel->machine->WriteRegister(2, -1); // return -1 to user program
				
				delete filename;
				break;
			}
			
			bool success = 1; //kernel->fileSystem->Remove(filename);
			if (!success)
			{
				// Fail to remove file
				printf("\nCan't remove file '%s'\n", filename);
				DEBUG(dbgAddr, "Can't remove file\n");
				kernel->machine->WriteRegister(2, -1); // return -1 to user program

				delete filename;
				break;
			}
			else
			{
				// Success to remove file
				kernel->machine->WriteRegister(2, 0); // return 0 to user program
				printf("\nSuccess to remove file '%s'\n", filename);

				delete filename;
				break;
			}
			break;
		}
		case SC_Open:
		{
			DEBUG(dbgSys, "Open system call.\n");
			int addr = kernel->machine->ReadRegister(4);
			char *name = User2System(addr, MAX_FILENAME_LEN);
			int fileId = kernel->Open(name);
			if (fileId == -1)
			{
				kernel->machine->WriteRegister(2, -1);
			}
			else
			{
				kernel->machine->WriteRegister(2, fileId);
			}
			delete[] name;

			ProgramCounter();
			break;
		}
		// hoan
		case SC_Read:
		{
			DEBUG(dbgSys, "Read system call.\n");
			int virAddr = kernel->machine->ReadRegister(4);
			int size = kernel->machine->ReadRegister(5);
			char *buffer = User2System(virAddr, size);
			int fileId = kernel->machine->ReadRegister(6);
			int numBytes = kernel->Read(buffer, size, fileId);
			if (numBytes < 0)
			{
				cout << "0 bytes read" << endl;
			}
			else
			{
				System2User(virAddr, numBytes, buffer);
			}
			kernel->machine->WriteRegister(2, numBytes);
			delete[] buffer;
			ProgramCounter();
			break;
		}
		case SC_Write:
		{
			DEBUG(dbgSys, "Write system call.\n");
			int VirAddr = kernel->machine->ReadRegister(4);
			int size = kernel->machine->ReadRegister(5);
			char *buffer = User2System(VirAddr, size);
			int fileId = kernel->machine->ReadRegister(6);
			int numBytes = kernel->Write(buffer, size, fileId);
			if (numBytes < 0)
			{
				cout << "0 bytes written" << endl;
			}
			else
			{
				System2User(VirAddr, numBytes, buffer);
			}
			kernel->machine->WriteRegister(2, numBytes);
			delete[] buffer;
			ProgramCounter();
			break;
		}
		case SC_Seek:
		{
			// int Seek(int position, OpenFileId id);

			DEBUG(dbgSys, "Seek system call.\n");
			int position = kernel->machine->ReadRegister(4);
			int fileId = kernel->machine->ReadRegister(5);
			int numbytes = kernel->Seek(position, fileId);
			kernel->machine->WriteRegister(2, numbytes);
			ProgramCounter();
			break;
		}
		// end of hoan
		case SC_Close:
		{
			DEBUG(dbgSys, "Close system call.\n");
			// SysClose((int)kernel->machine->ReadRegister(4));
			int result = kernel->Close((int)kernel->machine->ReadRegister(4));
			if (result == 0)
			{
				kernel->machine->WriteRegister(2, 0);
			}
			else
			{
				kernel->machine->WriteRegister(2, -1);
			}

			// for (int i = 0; i < 50; i++)
			// {
			// 	if (kernel->fileSystem->tableOfFiles[i] != NULL)
			// 	{
			// 		cout << "OpenFileID: " << i << "\tName: " << kernel->fileSystem->tableOfFiles[i]->fileName << endl;
			// 	}
			// }
			ProgramCounter();
			break;
		}
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
		{
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

			/* Process SysAdd Systemcall*/
			int result;
			result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
							/* int op2 */ (int)kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "Add returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);

			/* Modify return point */
			ProgramCounter();

			// return;
			// ASSERTNOTREACHED();

			break;
		}
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

			// cout << endl;
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
			// cout << "\n";
			ProgramCounter();
			break;
		}
		default:
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
		break;

	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
	}
}
