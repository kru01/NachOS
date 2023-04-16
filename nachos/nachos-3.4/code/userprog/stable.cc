// stable.cc 
//	Routines to manage semaphores.
//
// Copyright (c) 2023 Students of University of Science - VNUHCM.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions. All the points stated
// in the copyright.h of the University of California shall also cover us.

#include "copyright.h"
#include "stable.h"

//----------------------------------------------------------------------
// Sem::Sem(char *na, int i)
// 	Constructor to initialize the semaphore.
//----------------------------------------------------------------------

Sem::Sem(char *na, int i)
{
    strcpy(this->name, na);
    sem = new Semaphore(this->name, i);
}

//----------------------------------------------------------------------
// Sem::~Sem
// 	Destructor to terminate the semaphore.
//----------------------------------------------------------------------

Sem::~Sem()
{
    if (sem) delete sem;
}

//----------------------------------------------------------------------
// Sem::Wait
// 	Enable wait mode - Down(semaphore).
//----------------------------------------------------------------------

void
Sem::Wait()
{
    sem->P();		// Down(sem)
}

//----------------------------------------------------------------------
// Sem::Signal
// 	Send signal to free semaphore - Up(semaphore).
//----------------------------------------------------------------------

void
Sem::Signal()
{
    sem->V();		// Up(sem)
}

//----------------------------------------------------------------------
// Sem::GetName
// 	Return the name of the semaphore.
//----------------------------------------------------------------------

char*
Sem::GetName()
{
    return this->name;
}

//----------------------------------------------------------------------
// STable::STable()
// 	Constructor to initialize the semaphore table.
//----------------------------------------------------------------------

STable::STable()
{
    this->bm = new BitMap(MAX_SEMAPHORE);
    for (int i = 0; i < MAX_SEMAPHORE; i++) this->semTab[i] = NULL;
}

//----------------------------------------------------------------------
// STable::~STable
// 	Destructor to terminate the semaphore table by safely
//	deallocating all assigned memory.
//----------------------------------------------------------------------

STable::~STable()
{
    if (this->bm) {
        delete this->bm;
        this->bm = NULL;
    }

    for (int i = 0; i < MAX_SEMAPHORE; i++)
        if (this->semTab[i]) {
            delete this->semTab[i];
            this->semTab[i] = NULL;
        }
}

//----------------------------------------------------------------------
// STable::Create
// 	Create a new semaphore with name if it hasn't existed.
//	Return 0 if successful, -1 otherwise.
//----------------------------------------------------------------------

int
STable::Create(char *name, int init)
{
    for (int i = 0; i < MAX_SEMAPHORE; i++)
        if (bm->Test(i) && strcmp(name, semTab[i]->GetName()) == 0)
            return -1;

    int id = this->FindFreeSlot();
    if (id < 0) return -1;
    this->semTab[id] = new Sem(name, init);
    return 0;
}

//----------------------------------------------------------------------
// STable::Wait
// 	Put the semaphore with name into wait mode if it exists.
//	Return 0 if successful, -1 otherwise.
//----------------------------------------------------------------------

int
STable::Wait(char *name)
{
    for (int i = 0; i < MAX_SEMAPHORE; i++)
        if (bm->Test(i) && strcmp(name, semTab[i]->GetName()) == 0) {
            semTab[i]->Wait();
            return 0;
        }
    printf("\n\n Invalid semaphore.");
    return -1;
}

//----------------------------------------------------------------------
// STable::Signal
// 	Send signal to free the semaphore with name if it exists.
//	Return 0 if successful, -1 otherwise.
//----------------------------------------------------------------------

int
STable::Signal(char *name)
{
    for (int i = 0; i < MAX_SEMAPHORE; i++)
        if (bm->Test(i) && strcmp(name, semTab[i]->GetName()) == 0) {
            semTab[i]->Signal();
            return 0;
        }
    printf("\n\n Invalid semaphore.");
    return -1;
}

//----------------------------------------------------------------------
// STable::FindFreeSlot
// 	Return a free slot.
//----------------------------------------------------------------------

int
STable::FindFreeSlot()
{
    return this->bm->Find();
}
