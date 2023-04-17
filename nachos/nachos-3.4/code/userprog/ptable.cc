// ptable.cc 
//	Routines to manage processes.
//
// Copyright (c) 2023 Students of University of Science - VNUHCM.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions. All the points stated
// in the copyright.h of the University of California shall also cover us.

#include "copyright.h"
#include "ptable.h"
#include "system.h"
#include "openfile.h"

//----------------------------------------------------------------------
// PTable::PTable(int size)
// 	Constructor to initialize the process table.
//----------------------------------------------------------------------

PTable::PTable(int size)
{
    if (size < 0) return;
    bm = new BitMap(size);
    psize = size;
    bmsem = new Semaphore("bmsem", 1);
    for (int i = 0; i < MAX_PROCESS; i++) pcb[i] = 0;
    bm->Mark(0);
    pcb[0] = new PCB(0);
    pcb[0]->SetFileName("./test/scheduler");
    pcb[0]->parentID = -1;
}

//----------------------------------------------------------------------
// PTable::~PTable
// 	Destructor to terminate the process table by safely deallocating
//	all assigned memory.
//----------------------------------------------------------------------

PTable::~PTable()
{
    if (bm != 0) delete bm;
    //for (int i = 0; i < psize; i++)	// for some reasons, trying to delete keeps throwing
    //    if (pcb[i]) delete pcb[i];	// segmentation fault. I was unable to pinpoint the
					// problem so let's just pretend this doesn't exist
    if (bmsem != 0) delete bmsem;
}

//----------------------------------------------------------------------
// PTable::ExecUpdate
//	Handle syscall SC_Exec.
//	Return processID if successful, -1 otherwise.
//----------------------------------------------------------------------

int
PTable::ExecUpdate(char *name)
{
    bmsem->P();			// Prevent 2 processes being loaded at the same time
    int index = this->GetFreeSlot();

    if (name == NULL					// if the program doesn't exist
     || index < 0					// if there is no free slot
     || strcmp(name, "./test/scheduler") == 0		// if the program is the main process
     || strcmp(name, currentThread->getName()) == 0) {	// if the program is the thread itself
        printf("\n\n Unable to execute.");
        bmsem->V();
        return -1;
    }

    pcb[index] = new PCB(index);
    pcb[index]->SetFileName(name);
    pcb[index]->parentID = currentThread->processID;

    int pid = pcb[index]->Exec(name, index);
    bmsem->V();
    return pid;
}

//----------------------------------------------------------------------
// PTable::ExitUpdate
//	Handle syscall SC_Exit.
//	Return exitcode if successful, -1 otherwise.
//----------------------------------------------------------------------

int
PTable::ExitUpdate(int ec)
{
    int id = currentThread->processID;
    if (id == 0) {		// if the process if the main one
        currentThread->FreeSpace();
        interrupt->Halt();
        return 0;
    }

    if (IsExist(id) == false) {
        printf("\n\n Invalid process id.");
        return -1;
    }

    pcb[id]->SetExitCode(ec);
    pcb[pcb[id]->parentID]->DecNumWait();
    pcb[id]->JoinRelease();	// Free the parent process that are waiting (if exists)
    pcb[id]->ExitWait();	// Ask for permission to exit from the parent process
    Remove(id);
    return ec;
}

//----------------------------------------------------------------------
// PTable::JoinUpdate
//	Handle syscall SC_Join.
//	Return exitcode if successful, -1 otherwise.
//----------------------------------------------------------------------

int
PTable::JoinUpdate(int id)
{
    if (id < 0			// if the process doesn't exist or if the caller is not
     || currentThread->processID != pcb[id]->parentID) { // the parent of the process
        printf("\n\n Unable to join processes.");
        return -1;
    }

    pcb[pcb[id]->parentID]->IncNumWait();
    pcb[id]->JoinWait();	// Wait for the process to finish executing
    int ec = pcb[id]->GetExitCode();
    pcb[id]->ExitRelease();	// Allow the process to exit
    return ec;
}

//----------------------------------------------------------------------
// PTable::GetFreeSlot
//	Find free slot to store new process.
//----------------------------------------------------------------------

int
PTable::GetFreeSlot()
{
    return bm->Find();
}

//----------------------------------------------------------------------
// PTable::IsExist
//	Check if the processID exists.
//----------------------------------------------------------------------

bool
PTable::IsExist(int pid)
{
    return bm->Test(pid);
}

//----------------------------------------------------------------------
// PTable::Remove
//	When the process finishes, remove its processID.
//----------------------------------------------------------------------

void
PTable::Remove(int pid)
{
    bm->Clear(pid);
    if (pcb[pid] != 0) delete pcb[pid];
}

//----------------------------------------------------------------------
// PTable::Remove
//	Return the name of the process.
//----------------------------------------------------------------------

char*
PTable::GetFileName(int id)
{
    return pcb[id]->GetFileName();
}
