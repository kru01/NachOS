/* taps.c
 *	Program that helps in solving the tapwater problem in test/tapwater.c.
 *
 *	taps will write which tap distributed water to output.txt.
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
    SpaceId si_taps, si_output;
    int tap1 = 0, tap2 = 0, tapTime = 0;
    int problemSolved = 0;
    char charRead = '0';

    while (1) {
        while (1) {
            Wait("taps");			// Wait for signal from people.c
            charRead = 0;
            si_taps = Open("taps.txt", 1);	// Open as read-only.
            if (si_taps == -1) {
                Signal("people");
                return 1;
            }

            tapTime = 0;
            problemSolved = 0;

            while (1) {
                if (Read(&charRead, 1, si_taps) == -2) {	// If we reach EOF.
                    Close(si_taps);
                    break;
                }

                if (charRead == '*') {		// Reaching the asterisk means everyone
                    problemSolved = 1;		// has got their water so the problem
                    Close(si_taps);		// of the current point in time is solved.
                    break;
                }

                tapTime = tapTime * 10 + (charRead - 48);
            }

            si_output = Open("output.txt", 0);	// Open as read and write.
            if (si_output == -1) {
                Close(si_taps);
                Signal("people");
                return 1;
            }

            Seek(-1, si_output);		// Always start writing at the end of the file.

            if (tapTime != 0) {
                if (tap1 <= tap2) {
                    tap1 += tapTime;
                    Write(" 1", 2, si_output);	// Write tap 1 to output.txt.
                } else {
                    tap2 += tapTime;
                    Write(" 2", 2, si_output);	// Write tap 2 to output.txt.
                }
            }

            if (problemSolved == 1) {
                tap1 = tap2 = 0;
                Write("\r\n", 2, si_output);	// Write a newline so we can move on to the
						// next point in time.
                Close(si_output);		// Avoid conflicts with people.c.
                Signal("people");
                break;
            } else Write("   ", 3, si_output);

            Close(si_output);			// Avoid conflicts with people.c.
            Signal("people");
        }
    }

    return 0;
}
