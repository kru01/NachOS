/* tapwater.c
 *	Program that solves the tapwater problem by using multi-threading.
 *
 *	The problem is as follows, there are 2 taps, each can fill a bottle
 *	of 1 liter within 10 seconds. At one point in time, only 2 people
 *	can get water while the others have to wait for their turns.
 *
 *	The solution method requires 3 programs: tapwater, people, taps. The
 *	tapwater will read the inputs from input.txt then call Exec on people
 *	and taps, then wait for all threads to finish. people will write which
 *	person took water and taps will write which tap distributed water to
 *	the file. The result will be in output.txt.
 *	
 *	input.txt: First line has one integer N showing the number of points
 *	in time. For the next N lines, each contains n integers. Each integer
 *	represents how many liters a person's bottle can hold.
 *	output.txt: There are N lines, each contains n pairs of integers. The
 *	first number of each pair is the same as the number in input.txt. The
 *	second number shows which tap the person used.
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
    SpaceId si_input, si_people;
    int timePoints = 0;
    char charRead = '0';

    if (CreateSemaphore("tapwater", 0) == -1) return 1;
    if (CreateSemaphore("people", 0) == -1) return 1;
    if (CreateSemaphore("taps", 0) == -1) return 1;

    if (CreateFile("output.txt") == -1) return 1;

    si_input = Open("input.txt", 1);		// Open as read-only.
    if (si_input == -1) return 1;

    while (1) {					// Read the first line to get
        Read(&charRead, 1, si_input);		// the number of points in time.
        if (charRead == '\n') break;
        if (charRead >= '0' && charRead <= '9')
            timePoints = timePoints * 10 + (charRead - 48);
    }

    if (Exec("./test/people") == -1) {		// Start the execution of people.c.
        Close(si_input);
        return 1;
    }

    if (Exec("./test/taps") == -1) {		// Start the execution of taps.c.
        Close(si_input);
        return 1;
    }

    while (timePoints--) {
        if (CreateFile("people.txt") == -1) {	// Clearing the content of the file
            Close(si_input);			// each time so we can move on to
            return 1;				// the next point in time.
        }

        si_people = Open("people.txt", 0);	// Open as read and write.
        if (si_people == -1) {
            Close(si_input);
            return 1;
        }
		// We consider each point in time at a time so we read one
		// line into people.txt to consider each loop.
        while (1) {
            if (Read(&charRead, 1, si_input) < 1 || charRead == '\n')
                break;
            Write(&charRead, 1, si_people);
        }        

        Close(si_people);			// Avoid conflicts with people.c.
        Signal("people");			// Tell people.c to continue.
        Wait("tapwater");			// Wait for signal from people.c.
    }

    Close(si_input);
    PrintString("\n--> Solution has been written to output.txt!\n");
    Halt();
    return 0;
}
