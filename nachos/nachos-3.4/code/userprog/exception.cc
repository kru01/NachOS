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
//
//  NOTE -- The function's code is derived from actual code in machine/mipssim.cc,
//  each line has a comment showing which original code is being referenced.
//  PS -- The block comment below the code informs of where the original code can
//  be found. Alternatively, the original code can be made functional by using
//  machine->registers[..].
//----------------------------------------------------------------------

void
IncreaseProgramCounter()
{
    int counter = machine->ReadRegister(PCReg);     // registers[PCReg]
    // Advance program counters.
    machine->WriteRegister(PrevPCReg, counter);     // registers[PrevPCReg] = registers[PCReg]
    counter = machine->ReadRegister(NextPCReg);     // registers[NextPCReg]
    machine->WriteRegister(PCReg, counter);         // registers[PCReg] = registers[NextPCReg]
    machine->WriteRegister(NextPCReg, counter + 4); // registers[NextPCReg] = pcAfter (registers[NextPCReg] + 4)
    /*
    - Line 117 to 118 in machine/mipssim.cc
    // Compute next pc, but don't install in case there's an error or branch.
    int pcAfter = registers[NextPCReg] + 4;

    - Line 560 to 564 in machine/mipssim.cc
    // Advance program counters.
    registers[PrevPCReg] = registers[PCReg];	// for debugging, in case we
						// are jumping into lala-land
    registers[PCReg] = registers[NextPCReg];
    registers[NextPCReg] = pcAfter;
    */
}

//----------------------------------------------------------------------
// ReadIntHandler
// 	Function to read integer input from user and return it.
//	Return 0 if the input is not an integer.
//----------------------------------------------------------------------

void
ReadIntHandler()
{
    char* buffer;
    int MAX_BUFFER = 255;
    buffer = new char[MAX_BUFFER + 1];
    int inputLen = gSynchConsole->Read(buffer, MAX_BUFFER);
    int startInd = 0;
    int endInd = 0;
    if (buffer[0] == '-') startInd = endInd = 1; // if it is negative, we read from next position

    for (int i = startInd; i < inputLen; i++) {
        if (buffer[i] == '.') { // check for floating point, we count X.000 as integer
            for (int j = i + 1; j < inputLen; j++)
                if (buffer[j] != '0') { // if it is not 0 after the dot, it's not an integer
                    DEBUG('a', "\n Invalid integer inputted.");
                    printf("\n\n Invalid integer inputted.");
                    machine->WriteRegister(2, 0);
                    IncreaseProgramCounter();
                    delete[] buffer;
                    return;
                }
            endInd = i - 1; // if it's X.000 then we only get X
            break;
        } else if (buffer[i] < '0' || buffer[i] > '9') { // if it's not a number, it's not an integer
            DEBUG('a', "\n Invalid integer inputted.");
            printf("\n\n Invalid integer inputted.");
            machine->WriteRegister(2, 0);
            IncreaseProgramCounter();
            delete[] buffer;
            return;
        }
        endInd = i;
    }

    int output = 0;
    for (int i = startInd; i <= endInd; i++) output = output * 10 + (int)(buffer[i] - 48);
    if (buffer[0] == '-') output *= -1; // apply negativity if exists
    machine->WriteRegister(2, output);
    IncreaseProgramCounter();
    delete[] buffer;
}

//----------------------------------------------------------------------
// PrintIntHandler
// 	Function to print integer to console.
//----------------------------------------------------------------------

void
PrintIntHandler()
{
    char* buffer, neg = '-';
    int MAX_BUFFER = 255, number = machine->ReadRegister(4);
    buffer = new char[MAX_BUFFER + 1];

    if (number < 0) { // if negative, we print the '-' first then proceed as if the number was positive
        gSynchConsole->Write(&neg, 1);
        number *= -1;
    }

    int size = 0;
    while (number != 0) { // read all digits into char array, the number will be in reverse
        buffer[size++] = number % 10 + 48;
        number /= 10;
    }
    buffer[size] = '\0';

    int start = 0, end = 0;
    char temp = ' ';
    end = size - 1;
    while (start < end) { // reversing the char array to get the correct number
        temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
    gSynchConsole->Write(buffer, size);
    IncreaseProgramCounter();
    delete[] buffer;
}

//----------------------------------------------------------------------
// ReadCharHandler
// 	Function to read character input from user and return it.
//      Return 0 if nothing or more than one character was inputted.
//----------------------------------------------------------------------

void
ReadCharHandler()
{
    char* buffer;
    int MAX_BUFFER = 255;
    buffer = new char[MAX_BUFFER + 1];
    int inputLen = gSynchConsole->Read(buffer, MAX_BUFFER);

    if(inputLen == 0) {
        DEBUG('a', "\n No character received.");
        printf("\n\n No character received.");
        machine->WriteRegister(2, 0);
    } else if (inputLen > 1) {
        DEBUG('a', "\n More than one character inputted.");
        printf("\n\n More than one character inputted.");
        machine->WriteRegister(2, 0);
    } else machine->WriteRegister(2, buffer[0]);

    IncreaseProgramCounter();
    delete[] buffer;
}

//----------------------------------------------------------------------
// PrintCharHandler
// 	Function to print character to console.
//----------------------------------------------------------------------

void
PrintCharHandler()
{
    char character = (char)machine->ReadRegister(4);
    gSynchConsole->Write(&character, 1);
    IncreaseProgramCounter();
}

//----------------------------------------------------------------------
// ReadStringHandler
// 	Function to read string input from user into buffer, the string ends
//      when user hits enter or the length is already maxed out.
//----------------------------------------------------------------------

void
ReadStringHandler()
{
    // get buffer address from r4 and max length from r5
    int virtAddr = machine->ReadRegister(4), len = machine->ReadRegister(5);
    char* buffer = machine->User2System(virtAddr, len); // get the buffer in system space
    gSynchConsole->Read(buffer, len); // read the inputted string
    machine->System2User(virtAddr, len, buffer); // copy string to user space
    IncreaseProgramCounter();
    delete[] buffer;
}

//----------------------------------------------------------------------
// PrintStringHandler
// 	Function to print string to console.
//----------------------------------------------------------------------

void
PrintStringHandler()
{
    // get buffer address from r4
    int virtAddr = machine->ReadRegister(4), MAX_BUFFER = 255;
    char* buffer = machine->User2System(virtAddr, MAX_BUFFER);

    int i = 0; // print one character at a time until we encounter
    while (buffer[i] != 0 && buffer[i] != '\n') { // terminating char or endline
        gSynchConsole->Write(buffer + i, 1);
        i++;
    }
    IncreaseProgramCounter();
    delete[] buffer;
}

//----------------------------------------------------------------------
// ExceptionHandler
// 	Function that handles exceptions, mostly runtime errors.
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
        printf("\n\n No valid translation found.");
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
            printf("\n\n Shutdown, initiated by user program.");
            interrupt->Halt();
            break;
        case SC_ReadInt:
            ReadIntHandler();
            break;
        case SC_PrintInt:
            PrintIntHandler();
            break;
        case SC_ReadChar:
            ReadCharHandler();
            break;
        case SC_PrintChar:
            PrintCharHandler();
            break;
        case SC_ReadString:
            ReadStringHandler();
            break;
        case SC_PrintString:
            PrintStringHandler();
            break;
        default:
            printf("\n Unexpected user mode exception %d %d\n", which, type);
            interrupt->Halt();
            // ASSERT(FALSE);
        }
    }
}
