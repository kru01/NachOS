/* readprint.c
 *	Simple program to test syscalls dealing with reading and printing
 *      user input.
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
/*
    int num = 0;
    PrintString("Input an integer: ");
    input = ReadInt();
    PrintString("Integer: ");
    PrintInt(num);
*/
    char str[100];
    PrintString("Input a string: ");
    ReadString(str, 100);
    PrintString(str);

    Halt();
}
