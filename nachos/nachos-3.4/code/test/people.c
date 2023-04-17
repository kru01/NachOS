/* people.c
 *	Program that helps in solving the tapwater problem in test/tapwater.c.
 *
 *	people will write which person needs water to taps.txt and output.txt.
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
    SpaceId si_people, si_taps, si_output;
    int getWater = 0;
    int fileLen = 0, filePointer = 0;
    char charRead = '0';

    while (1) {
        fileLen = 0;				// Reset since the number of people is
						// different at each point in time.
        Wait("people");				// Wait for signal from tapwater.

        si_people = Open("people.txt", 1);	// Open as read-only.
        if (si_people == -1) {
            Signal("tapwater");
            return 1;
        }

        fileLen = Seek(-1, si_people);		// The number of people is different
						// at each point in time.
        Seek(0, si_people);			// Move the pointer back to the start.
        filePointer = 0;			// Reset the file pointer.

        if (CreateFile("taps.txt") == -1) {
            Close(si_people);
            Signal("tapwater");
            return 1;
        }

        si_taps = Open("taps.txt", 0);		// Open as read and write.
        if (si_taps == -1) {
            Close(si_people);
            Signal("tapwater");
            return 1;
        }

        while (filePointer < fileLen) {
            getWater = 0;
            Read(&charRead, 1, si_people);
		// Read a character and if it's a number we will write it to taps.txt.
                // A space means we have read all the digits of the number so we will
		// begin to get water from the tap before reading the next number.
            if (charRead != ' ') {
                Write(&charRead, 1, si_taps);

                si_output = Open("output.txt", 0);	// Open as read and write.
                if (si_output == -1) {
                    Close(si_people);
                    Signal("tapwater");
                    return 1;
                }

                Seek(-1, si_output);		// Always start writing at the end of the file.
                Write(&charRead, 1, si_output);	// We write whatever we read to output.
                Close(si_output);		// Avoid conflicts with taps.c.
            } else getWater = 1;

            if (filePointer == fileLen - 1) {	// We use an asterisk to mark the end
                Write("*", 1, si_taps);		// of the current file (the current
                getWater = 1;			// point in time).
            }

            if (getWater == 1) {
                Close(si_taps);			// Avoid conflicts with taps.c.
                Signal("taps");
                Wait("people");			// Wait for signal from taps.c.

                if (CreateFile("taps.txt") == -1) {	// This is just to delete all the
                    Close(si_people);			// content in the file, i.e., the
                    Signal("tapwater");			// person that has already got
                    return 1;				// water from the tap.
                }

                si_taps = Open("taps.txt", 0);		// Open as read and write.
                if (si_taps == -1) {
                    Close(si_people);
                    Signal("tapwater");
                    return 1;
                }
            }

            filePointer++;			// Move the pointer to the next character.
        }

        Signal("tapwater");
    }

    return 0;
}
