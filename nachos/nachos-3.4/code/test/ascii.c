/* ascii.c
 *	Simple program to print out ascii table.
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
{   // readable asciis are only from 32 to 126, I won't bother printing the others
    int i = 32;
    char c = 32; // very funny, these have to be define at the top
    // don't put \n inside PrintString(), it'll stop printing exactly there
    PrintChar('\n');
    PrintString("-- Readable asciis are only from 32 to 126");
    PrintChar('\n');

    for (; i <= 126; i++, c++) {
        if (i < 10) PrintChar('0');
        if (i < 100) PrintChar('0');
        PrintInt(i);
        PrintString(" | ");
        PrintChar(c);
        PrintChar('\n');
    }
    // output should look like this, xxx | ascii\n
    PrintString("-- Readable asciis are only from 32 to 126");
    PrintChar('\n');

    Halt();
}
