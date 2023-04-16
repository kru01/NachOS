// ptable.h - Process Table
//	Data structures to manage running processes, possess a pcb (process
//	control block) attribute which is an array of max 10 processes.
//
//	The constructor of ptable will initialize the parent process on
//	position 0. From there, we will create other child processes through
//	Exec() system call.
//
// Copyright (c) 2023 Students of University of Science - VNUHCM.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions. All the points stated
// in the copyright.h of the University of California shall also cover us.

#ifndef PTABLE_H
#define PTABLE_H

#include "copyright.h"
#include "bitmap.h"
#include "pcb.h"
#include "synch.h"

#define MAX_PROCESS 10

class PTable {
  public:
    PTable(int size = 10);		// Constructor
    ~PTable();				// Destructor

    int ExecUpdate(char *name);		// Handle syscall SC_Exec
    int ExitUpdate(int ec);		// Handle syscall SC_Exit
    int JoinUpdate(int id);		// Handle syscall SC_Join

    int GetFreeSlot();			// Find free slot to store new process
    bool IsExist(int pid);		// Check if the processID exists
    void Remove(int pid);		// When the process finishes, remove its processID
    char* GetFileName(int id);		// Return the name of the process

  private:
    BitMap *bm;				// Mark the spot that are in-use in pcb
    PCB *pcb[MAX_PROCESS];
    int psize;
    Semaphore *bmsem;			// Prevent 2 processes being loaded at the same time
};

#endif // PTABLE_H
