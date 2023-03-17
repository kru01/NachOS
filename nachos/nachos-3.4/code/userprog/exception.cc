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
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

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
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// IncreaseProgramCounter
// 	Handy function to increase program counter.
//----------------------------------------------------------------------

void
IncreaseProgramCounter()
{
    // Line 117 to 118 in machine/mipssim.cc
    // Compute next pc, but don't install in case there's an error or branch.
    int pcAfter = registers[NextPCReg] + 4;

    // Line 560 to 564 in machine/mipssim.cc
    // Advance program counters.
    registers[PrevPCReg] = registers[PCReg];	// for debugging, in case we
						// are jumping into lala-land
    registers[PCReg] = registers[NextPCReg];
    registers[NextPCReg] = pcAfter;
}

//----------------------------------------------------------------------
// ExceptionHandler
// 	Fucntion that handles exceptions, mostly runtime errors.
// 
//  NOTE -- NoException will transfer control back to OS while Syscall
//  exceptions will be individually handled. Others will just inform of
//  the error and halt the system.
//  PS -- ExceptionType can be found in machine/machine.h
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    switch (which) {
    case NoException: return;
    case PageFaultException:
        DEBUG('a', "\n No valid translation found.");
        printf("\n\n No valid translation found.")
        interrupt->Halt();
        break;
    case ReadOnlyException:
        DEBUG('a', "\n Write attempted to page marked \"read-only\".");
        printf("\n\n Write attempted to page marked \"read-only\".");
        interrupt->Halt();
        break;
    case BusErrorException:
        DEBUG('a', "\n Translation resulted in an invalid physical address.");
        printf("\n\n Translation resulted in an invalid physical address.");
        interrupt->Halt();
        break;
    case AddressErrorException:
        DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space.");
        printf("\n\n Unaligned reference or one that was beyond the end of the address space.");
        interrupt->Halt();
        break;
    case OverflowException:
        DEBUG('a', "\n Integer overflow in add or sub.");
        printf("\n\n Integer overflow in add or sub.");
        interrupt->Halt();
        break;
    case IllegalInstrException:
        DEBUG('a', "\n Unimplemented or reserved instr.");
        printf("\n\n Unimplemented or reserved instr.");
        interrupt->Halt();
        break;
    case NumExceptionTypes:
        DEBUG('a', "\n Number Exception Types.");
        printf("\n\n Number Exception Types.");
        interrupt->Halt();
        break;
    case SyscallException:
        switch (type) {
        case SC_Halt:
            DEBUG('a', "\n Shutdown, initiated by user program.");
            printf ("\n\n Shutdown, initiated by user program.");
            interrupt->Halt();
        default:
            printf("\n Unexpected user mode exception %d %d\n", which, type);
            interrupt->Halt();
            // ASSERT(FALSE);
        }
    }
}