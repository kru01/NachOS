// pcb.cc 
//	Routines to record information and managing processes.
//
// Copyright (c) 2023 Students of University of Science - VNUHCM.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions. All the points stated
// in the copyright.h of the University of California shall also cover us.

#include "copyright.h"
#include "pcb.h"
#include "utility.h"
#include "system.h"
#include "thread.h"
#include "addrspace.h"

extern void StartProcessID(int pid);

//----------------------------------------------------------------------
// PCB::PCB(int id)
// 	Constructor to initialize the process control block.
//----------------------------------------------------------------------

PCB::PCB(int id)
{
    this->parentID = (id == 0) ? -1 : currentThread->processID;
    this->numwait = this->exitcode = this->isBackground = 0;
    this->thread = NULL;
    this->joinsem = new Semaphore("joinsem", 0);
    this->exitsem = new Semaphore("exitsem", 0);
    this->multex = new Semaphore("multex", 1);
}

//----------------------------------------------------------------------
// PCB::~PCB
// 	Destructor to terminate the process control block by safely
//	deallocating all assigned memory.
//----------------------------------------------------------------------

PCB::~PCB()
{
    if (joinsem != NULL) delete this->joinsem;
    if (exitsem != NULL) delete this->exitsem;
    if (multex != NULL) delete this->multex;
    if (thread != NULL) {
        thread->FreeSpace();
        thread->Finish();
    }
}

//----------------------------------------------------------------------
// PCB::Exec
//	Load the program from the file with the corresponding name and
//	assign its processID as pid. Create a new thread with its name
//	taken from the filename and its process from pid.
//	Return processID if successful, -1 otherwise.
//----------------------------------------------------------------------

int
PCB::Exec(char *filename, int pid)
{
    multex->P();		// Prevent 2 processes being loaded at the same time
    this->thread = new Thread(filename);

    if (this->thread == NULL) {
        printf("\n\n Unable to create thread.");
        multex->V();
        return -1;
    }

    this->thread->processID = pid;
    this->parentID = currentThread->processID;
    this->thread->Fork(StartProcessID, pid);
    multex->V();
    return pid;
}

//----------------------------------------------------------------------
// PCB::GetID
//	Return processID of the caller process.
//----------------------------------------------------------------------

int
PCB::GetID()
{
    return this->thread->processID;
}

//----------------------------------------------------------------------
// PCB::GetNumWait
//	Return the number of waiting processes.
//----------------------------------------------------------------------

int
PCB::GetNumWait()
{
    return this->numwait;
}

//----------------------------------------------------------------------
// PCB::JoinWait
//	Switch the process into block mode then stop executing and wait
//	for a signal from JoinRelease to resume.
//----------------------------------------------------------------------

void
PCB::JoinWait()
{
    joinsem->P();
}

//----------------------------------------------------------------------
// PCB::ExitWait
//	Switch the process into block mode then stop executing and wait
//	for a signal from ExitRelease to resume.
//----------------------------------------------------------------------

void
PCB::ExitWait()
{
    exitsem->P();
}

//----------------------------------------------------------------------
// PCB::JoinRelease
//	Liberate the process that called JoinWait.
//----------------------------------------------------------------------

void
PCB::JoinRelease()
{
    joinsem->V();
}

//----------------------------------------------------------------------
// PCB::ExitRelease
//	Liberate the process that called ExitWait.
//----------------------------------------------------------------------

void
PCB::ExitRelease()
{
    exitsem->V();
}

//----------------------------------------------------------------------
// PCB::IncNumWait
//	Increase the number of waiting processes.
//----------------------------------------------------------------------

void
PCB::IncNumWait()
{
    multex->P();
    numwait++;
    multex->V();
}

//----------------------------------------------------------------------
// PCB::DecNumWait
//	Decrease the number of waiting processes.
//----------------------------------------------------------------------

void
PCB::DecNumWait()
{
    multex->P();
    if (numwait > 0) numwait--;
    multex->V();
}

//----------------------------------------------------------------------
// PCB::SetExitCode
//	Set the exitcode of the process.
//----------------------------------------------------------------------

void
PCB::SetExitCode(int ec)
{
    this->exitcode = ec;
}

//----------------------------------------------------------------------
// PCB::SetExitCode
//	Return exitcode.
//----------------------------------------------------------------------

int
PCB::GetExitCode()
{
    return this->exitcode;
}

//----------------------------------------------------------------------
// PCB::SetFileName
//	Set the name of the process.
//----------------------------------------------------------------------

void
PCB::SetFileName(char *fn)
{
    strcpy(FileName, fn);
}

//----------------------------------------------------------------------
// PCB::GetFileName
//	Return the name of the process.
//----------------------------------------------------------------------

char*
PCB::GetFileName()
{
    return this->FileName;
}
