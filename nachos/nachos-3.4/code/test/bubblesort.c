/* bubblesort.c
 *	Program that allows user to input an array of n integers (n <= 100)
 *      then sort said array ascendingly.
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
    int n = 0, i = 0, j = 0, temp = 0;
    int arr[100];

    PrintChar('\n');
    PrintString("Input array's size (<= 100): ");
    n = ReadInt();

    PrintString("-- Input integers (MUST hit enter after every integer or it WILL break): ");
    PrintChar('\n');
    for (i = 0; i < n; i++) arr[i] = ReadInt();

    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[i]) {
                temp = arr[j];
                arr[j] = arr[i];
                arr[i] = temp;
            }
        }

    PrintString("Sorted array: ");
    for (i = 0; i < n; i++) {
        PrintInt(arr[i]);
        PrintChar(' ');
    }

    PrintChar('\n');
    Halt();
}
