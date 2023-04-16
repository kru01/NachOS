// stable.h - Semaphore Table
//	Data structures to manage semaphore, possess a semTab (semaphore
//	table) attribute which is an array of max 10 semaphores.
//
// Copyright (c) 2023 Students of University of Science - VNUHCM.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions. All the points stated
// in the copyright.h of the University of California shall also cover us.

#ifndef STABLE_H
#define STABLE_H

#include "copyright.h"
#include "synch.h"
#include "bitmap.h"

#define MAX_SEMAPHORE 10

class Sem {
  public:
    Sem(char* na, int i);		// Constructor
    ~Sem();				// Destructor

    void Wait();			// Enable wait mode
    void Signal();			// Send signal to free semaphore
    char* GetName();			// Return the name of the semaphore

  private:
    char name[50];
    Semaphore *sem;			// create semaphore for management
};

class STable {
  public:
    STable();				// Constructor
    ~STable();				// Destructor

    int Create(char *name, int init);	// Create a new semaphore with name if hasn't existed
    int Wait(char *name);		// Put the semaphore with name into wait mode if it exists
    int Signal(char *name);		// Send signal to free the semaphore with name if it exists
    int FindFreeSlot();			// Return a free slot

  private:
    BitMap *bm;				// for management of free slot
    Sem *semTab[MAX_SEMAPHORE];		// manage max 10 semaphores
};

#endif // STABLE_H
