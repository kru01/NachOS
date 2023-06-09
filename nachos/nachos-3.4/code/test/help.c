/* help.c
 *	Simple program to print out introduction.
 *
 * 	NOTE: for some reason, user programs with global data structures 
 *	sometimes haven't worked in the Nachos environment.  So be careful
 *	out there!  One option is to allocate data structures as 
 * 	automatics within a procedure, but if you do this, you have to
 *	be careful to allocate a big enough stack to hold the automatics!
 */

#include "syscall.h"
#include "copyright.h"

int
main()
{
    PrintChar('\n');
    PrintString("|-- Operating System - 21CLC05 - HCMUS");
    PrintChar('\n');
    PrintString(" \\-- Lecturers: Le Viet Long, Pham Tuan Son");
    PrintChar('\n');
    PrintString("  |- 21127004 - Tran Nguyen An Phong");
    PrintChar('\n');
    PrintString("  |- 21127081 - Nguyen Minh Khoi");
    PrintChar('\n');
    PrintString("  |- 21127135 - Diep Huu Phuc");
    PrintChar('\n');
    PrintString("  |- 21127325 - Phan Dang Anh Khoi");
    PrintChar('\n');
    PrintString("  |- 21127446 - Vo Minh Anh Thu");
    PrintChar('\n');

    PrintString("|-- Programs in code/test");
    PrintChar('\n');
    PrintString(" \\-- ascii");
    PrintChar('\n');
    PrintString("  |- Print ascii table from 32 to 126 since those are");
    PrintChar('\n');
    PrintString("  |- the only readable asciis.");
    PrintChar('\n');
    PrintString(" \\-- bubblesort");
    PrintChar('\n');
    PrintString("  |- Let user input an array of n integers (n <= 100)");
    PrintChar('\n');
    PrintString("  |- then sort said array ascendingly. When inputing");
    PrintChar('\n');
    PrintString("  |- integers, please hit enter after every input!");
    PrintChar('\n');

    Halt();
}
