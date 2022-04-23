// kernel.h
//	Global variables for the Nachos kernel.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef KERNEL_H
#define KERNEL_H

#include "copyright.h"
#include "debug.h"
#include "utility.h"
#include "thread.h"
#include "scheduler.h"
#include "interrupt.h"
#include "stats.h"
#include "alarm.h"
#include "filesys.h"
#include "machine.h"

/* A unique identifier for an open Nachos file. */
typedef int OpenFileID;

class PostOfficeInput;
class PostOfficeOutput;
class SynchConsoleInput;
class SynchConsoleOutput;
class SynchDisk;

class Kernel
{
public:
  Kernel(int argc, char **argv);
  // Interpret command line arguments
  ~Kernel(); // deallocate the kernel

  void Initialize(); // initialize the kernel -- separated
                     // from constructor because
                     // refers to "kernel" as a global

  void ThreadSelfTest(); // self test of threads and synchronization

  void ConsoleTest(); // interactive console self test

  void NetworkTest(); // interactive 2-machine network test

  // bool CreateFile(char *filename, int size);

  void PrintBuffer(char *buffer, int size);

  void ReadNum(int to_register);

  void PrintNum(int number);

  void RandomNumber();

  void ReadString(int to_addr, char *buffer, int size);

  void ReadChar(int to_register);

  void PrintChar(char ch);

  // remove
  int Remove(char* fileName)
  {
    if(fileName == NULL)
    {
      return -1;
    }
    if(strlen(fileName) == 0)
    {
      return -1;
    }
    return fileSystem->Remove(fileName);
  }

  // open
  int Open(char *name)
  {
    if (name == NULL)
    {
      return -1;
    }

    for (int i = 0; i < 50; ++i)
    {
      if (fileSystem->tableOfFiles[i] != NULL && strcmp(fileSystem->tableOfFiles[i]->fileName, name) == 0)
      {
        return i;
      }
    }

    int emptySlot = -1;
    for (int i = 2; i < 50; ++i)
    {
      if (fileSystem->tableOfFiles[i] == NULL)
      {
        emptySlot = i;
        break;
      }
    }
    if (emptySlot == -1)
    {
      return -1;
    }
    fileSystem->tableOfFiles[emptySlot] = fileSystem->Open(name);
    if (fileSystem->tableOfFiles[emptySlot] == NULL)
    {
      return -1;
    }
    fileSystem->tableOfFiles[emptySlot]->fileName = new char[strlen(name) + 1];
    strcpy(fileSystem->tableOfFiles[emptySlot]->fileName, name);
    return emptySlot;
  }

  int Close(int fileId)
  {
    if (fileId < 0 || fileId >= 50)
    {
      return -1;
    }
    if (fileSystem->tableOfFiles[fileId] == NULL)
    {
      return -1;
    }

    delete fileSystem->tableOfFiles[fileId];
    fileSystem->tableOfFiles[fileId] = NULL;
    return 0;
  }
  // read
  int Read(char *buffer, int size, int id)
  {
    if (id < 0 || id >= 50)
    {
      return -1;
    }
    if (fileSystem->tableOfFiles[id] == NULL)
    {
      return -1;
    }
    int result = fileSystem->tableOfFiles[id]->Read(buffer, size);
    return result;
  }

  // write
  int Write(char *buffer, int size, int id)
  {
    if (id < 0 || id >= 50)
    {
      return -1;
    }
    if (fileSystem->tableOfFiles[id] == NULL)
    {
      return -1;
    }
    int result = fileSystem->tableOfFiles[id]->Write(buffer, size);
    return result;
  }

  // seek
  int Seek(int position, int id)
  {
    if (id < 0 || id >= 50)
    {
      return -1;
    }
    if (fileSystem->tableOfFiles[id] == NULL)
    {
      return -1;
    }
    if (position == -1)
      position = fileSystem->tableOfFiles[id]->Length();
    if (position < 0)
      position = 0;
    int numbytes = fileSystem->tableOfFiles[id]->Seek(position);
    if (numbytes >= 0)
    {
      return numbytes;
    }
    else
    {
      return -1;
    }
  }

  // These are public for notational convenience; really,
  // they're global variables used everywhere.

  Thread *currentThread; // the thread holding the CPU
  Scheduler *scheduler;  // the ready list
  Interrupt *interrupt;  // interrupt status
  Statistics *stats;     // performance metrics
  Alarm *alarm;          // the software alarm clock
  Machine *machine;      // the simulated CPU
  SynchConsoleInput *synchConsoleIn;
  SynchConsoleOutput *synchConsoleOut;
  SynchDisk *synchDisk;
  FileSystem *fileSystem;
  PostOfficeInput *postOfficeIn;
  PostOfficeOutput *postOfficeOut;

  int hostName; // machine identifier

private:
  bool randomSlice;   // enable pseudo-random time slicing
  bool debugUserProg; // single step user program
  double reliability; // likelihood messages are dropped
  char *consoleIn;    // file to read console input from
  char *consoleOut;   // file to send console output to
#ifndef FILESYS_STUB
  bool formatFlag; // format the disk if this is true
#endif
};

#endif // KERNEL_H
