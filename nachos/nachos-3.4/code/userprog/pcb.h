// pcb.h - Process Control Block
//	Data structures to keep track of information pertaining to
//	the management of processes.
//
// Copyright (c) 2023 Students of University of Science - VNUHCM.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions. All the points stated
// in the copyright.h of the University of California shall also cover us.

#ifndef PCB_H
#define PCB_H

#include "copyright.h"
#include "thread.h"
#include "synch.h"

class PCB {
  public:
    int parentID;			// ID of parent process
    bool isBackground;			// Flag to check if the process is a background one

    PCB();
    PCB(int id = 0);			// Constructor
    ~PCB();				// Destructor

    int Exec(char *filename, int pid);	// Load the program from the file with the
					// corresponding name and assign its processID
					// as pid. Create a new thread with its name
					// taken from the filename and its process from pid
    int GetID();			// Return processID of the caller process
    int GetNumWait();			// Return the number of waiting processes

    void JoinWait();			// 1. The parent process waits for its child to finish
    void ExitWait();			// 4. The child process finishes
    void JoinRelease();			// 2. Signal for the parent process to continue
    void ExitRelease();			// 3. Allow the child process to end

    void IncNumWait();			// Increase the number of waiting processes
    void DecNumWait();			// Decrease the number of waiting processes

    void SetExitCode(int ec);		// Set the exitcode of the process
    int GetExitCode();			// Return exitcode

    void SetFileName(char *fn);		// Set the name of the process
    char* GetFileName();		// Return the name of the process

  private:
    Semaphore *joinsem;			// Semaphore for joining proccess
    Semaphore *exitsem;			// Semaphore for exiting process
    Semaphore *multex;			// Semaphore for exclusively accessing process

    int exitcode;
    int numwait;			// The number of already-joined processes

    char FileName[32];			// The name of the process
    Thread *thread;			// The process of the program
};

#endif // PCB_H
