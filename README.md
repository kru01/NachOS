<a name="readme-top"></a>

# NachOS - Not Another Completely Heuristic Operating System

- Group project from HCMUS's 2023 Operating System course.

<details open>
  <summary>Table of Contents</summary>
  <ul>
    <li><a href="#content">Content</a></li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li>
      <a href="#usage">Usage</a>
      <ul>
        <li><a href="#to-compile-nachos">To compile NachOS</a></li>
        <li><a href="#to-run-a-program">To run a program</a></li>
	<li><a href="#to-make-a-program">To make a program</a></li>
	<li><a href="#to-make-a-new-system-call">To make a new System call</a></li>
	<li><a href="#to-make-a-new-class">To make a new class</a></li>
	<li><a href="#to-make-a-new-global-pointer">To make a new global pointer</a></li>
      </ul>
    </li>
    <li><a href="#built-with">Built With</a></li>
    <li><a href="#meet-the-team">Meet The Team</a></li>
  </ul>
</details>

## Content

- ***Project 1: NachOS system calls.*** Details are in `Guides (vietnamese)/ProjectInfo`.
- ***Project 2: Multi-threading and synchronization.*** Details are in `Guides (vietnamese)/ProjectInfo2`.
- Read the guides, especially ProjectInfo, for full details.
   - They are in vietnamese, ***maybe*** I'll do some translating once I find the time.
- Alternatively, the commit history serves as a chronological documentation of which task was implemented first.

## Getting Started

### Prerequisites

* Ubuntu 14.04 32-bit (i386)
* gcc / g++
* binutils-2.11.2.tar.gz (cross-compiler-mips)

### Installation

1. Clone the repo.
1. Grant permission to execute file as program.
   * Go into `nachos/gnu-decstation-ultrix/decstation-ultrix/2.95.3/` folder.
     * Select all the files in there `(as, cc1, cpp0, gcc, ld)`.
     * Right-click, open `Properties`, go into `Permissions` tab and allow them to execute as program.
   * Go into `nachos/nachos-3.4/code/userprog/` folder.
     * Select `nachos`.
     * Right-click, open `Properties`, go into `Permissions` tab and allow it to execute as program.
1. Open up terminal and cd into `nachos/nachos-3.4/code/`.
1. Run `make` command.
1. cd into `threads/`.
1. Run `./nachos` command.
1. If the output is similar to this, you're all set!
   ```text
   No threads ready or runnable, and no pending interrupts.
   Assuming the program completed.
   Machine halting!
   
   Ticks: total 10, idle 0, system 10, user 0
   Disk I/O: reads 0, writes 0
   Console I/O: reads 0, writes 0
   Paging: faults 0
   Network I/O: packets received 0, sent 0
   
   Cleaning up...
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Usage

### To compile NachOS

1. Open up terminal and cd into `nachos/nachos-3.4/code/`.
1. Run `make` command.

### To run a program

All of the user-level programs are stored in `nachos/nachos-3.4/code/test/`.

1. Open up terminal and cd into `nachos/nachos-3.4/code/`.
1. Run `./userprog/nachos –rs 1023 –x ./test/programName`.

### To make a program

1. Go into `nachos/nachos-3.4/code/test/`.
1. Create `programName.c` file and write your program in there.
1. Open `Makefile` in `test/` folder.
1. Add your program's name on line 38.
   ```make
   all: halt shell ... people taps programName
   ```
1. Scroll down and add the below lines of code to the file.
   ```make
   programName.o: programName.c
	   $(CC) $(CFLAGS) -c programName.c
   programName: programName.o start.o
	   $(LD) $(LDFLAGS) start.o programName.o -o programName.coff
	   ../bin/coff2noff programName.coff programName
   ```
1. Recompile NachOS.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### To make a new System call

- Let's use an already written system call as an example.
   ```c
   int ReadInt();
   ```
   - Implementing this system call means we're handling exception with `which = SyscallException` and `type = SC_ReadInt` in `userprog/exception.cc`.

1. Navigate to `nachos/nachos-3.4/code/userprog/` folder and open `syscall.h`.
   - Define a new macro.
      ```c
      #define SC_ReadInt	11
      ```
   - Declare the system call.
      ```c
      /* Read a single integer input from user (using SynchConsole class) and return it.
       * Return 0 if the input is not an integer.
       */
      int ReadInt();
      ```
1. Navigate to `nachos/nachos-3.4/code/test/` folder and open both `start.c` and `start.s`.
   - Implement how the assembler should handle the system call.
      ```gas
        .globl ReadInt
        .ent	ReadInt
      ReadInt:
        addiu $2,$0,SC_ReadInt
        syscall
        j	$31
        .end ReadInt
      ```
   - This should be the same for both of the files.
1. Navigate back to `nachos/nachos-3.4/code/userprog/` folder and open `exception.cc`.
   - Give everything a read, especially the block comment on line 28.
   - Implement the code to handle the system call.
      ```c
      void
      ExceptionHandler(ExceptionType which)
      {
         ...
            case SC_ReadInt:
            ...
            break;
         ...
      }
      ```
1. Recompile NachOS.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### To make a new class

- Let's use an already written class as an example.
   ```cpp
   class PTable { ... };
   ```

1. Navigate to the appropriate folder for the class.
   - Since `PTable` is a process-managing table that assists in the implementation of system calls that enable multi-threading ability. It is only proper that we put its files in `nachos/nachos-3.4/code/userprog/`, the folder where we provide tools for user programs.
1. Create `.h` and `.cc` files then implement the class in them.
   - **It is ***imperative*** that you follow the formats and conventions as closely as possible.** Not only does this help create a sense of uniformity between the files, it also mitigates *your brain splitting* and *you wanting to gouge your eyes out* while debugging.
   - **I would implore you to just copy one of the existing `.h` and `.cc` files then modify them into the new class**.
   - After finishing, we have ourselves `ptable.h` and `ptable.cc`.
1. Navigate to `nachos/nachos-3.4/code/` and open `Makefile.common`.
   - Declare all your files, i.e., `.h` and `.cc`, at the section corresponding to the folder you implemented your class in. For our current `PTable`, as we put its files in `nachos/nachos-3.4/code/userprog/`, we will walk through the process of declaring the files in the `userprog` section as an example.
   - Add `../folder/filename.h` to `USERPROG_H`. If you don't want to put your file on the last line, use `../folder/filename.h\`.
   ```gas
   USERPROG_H = ../userprog/addrspace.h\
            ...
            ../userprog/ptable.h\
            ../userprog/stable.h
   ```
   - Add `../folder/filename.cc` to `USERPROG_C`. If you don't want to put your file on the last line, use `../folder/filename.cc\`.
   ```gas
   USERPROG_C = ../userprog/addrspace.cc\
            ...
            ../userprog/ptable.cc\
            ../userprog/stable.cc
   ```
   - Add `filename.o` to `USERPROG_O`.
   ```gas
   USERPROG_O = addrspace.o bitmap.o exception.o progtest.o console.o machine.o \
            mipssim.o translate.o synchcons.o pcb.o ptable.o stable.o
   ```
1. Recompile NachOS.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### To make a new global pointer

- Let's use an already written global pointer as an example.
   ```cpp
   class PTable* pTab;
   ```

1. Navigate to `nachos/nachos-3.4/threads/` and open `system.h`.
   1. Include `filename.h` file that contains the datatype of the pointer.
      ```cpp
      #include "copyright.h"
      ...
      #include "ptable.h"
      #include "stable.h"
      ```
   1. Declare the pointer as global by `extern`ing it.
      ```cpp
      #ifdef USER_PROGRAM
      #include "machine.h"
      ...
      extern PTable* pTab;		// process-managing table
      extern STable* semTab;		// semaphore-managing table
      #endif
      ```
1. In the same folder, open `system.cc`.
   1. Declare the pointer.
      ```cpp
      #ifdef USER_PROGRAM	// requires either FILESYS or FILESYS_STUB
      Machine *machine;	// user program memory and registers
      ...
      PTable* pTab;		// process-managing table
      STable* semTab;		// semaphore-managing table
      #endif
      ```
   1. Define the pointer (Initialize it).
      ```cpp
      #ifdef USER_PROGRAM
      machine = new Machine(debugUserProg);	// this must come first
      ...
      pTab = new PTable(10);
      semTab = new STable();
      #endif
      ```
   1. Delete the pointer (Free the memory allocated to it).
      ```cpp
      #ifdef USER_PROGRAM
      delete machine;
      ...
      delete pTab;
      delete semTab;
      #endif
      ```
1. Recompile NachOS.

## Built With

[gediticon]: https://upload.wikimedia.org/wikipedia/commons/thumb/c/ca/Gedit-logo-clean.svg/90px-Gedit-logo-clean.svg.png?20220324134749
[gediturl]: https://github.com/GNOME/gedit

[gccicon]: https://cdn.jsdelivr.net/gh/devicons/devicon/icons/gcc/gcc-original.svg
[gccurl]: https://ftp.gnu.org/gnu/gcc/gcc-4.8.4/

[ubuntuicon]: https://cdn.jsdelivr.net/gh/devicons/devicon/icons/ubuntu/ubuntu-plain.svg
[ubuntuurl]: https://releases.ubuntu.com/14.04/

| [![gedit][gediticon]][gediturl] | [![gcc][gccicon]][gccurl] | [![Ubuntu][ubuntuicon]][ubuntuurl] |
| :-: | :-: | :-: |
| 3.10.4 | &nbsp;&nbsp;&nbsp; 4.8.4 &nbsp;&nbsp;&nbsp; | 14.04 i386 |

## Meet The Team

<div align="center">
  <a href="https://github.com/phongan1x5"><img alt="phongan1x5" src="https://github.com/phongan1x5.png" width="60px" height="auto"></a>&nbsp;&nbsp;&nbsp;
  <a href="https://github.com/nminhkhoi0818"><img alt="nminhkhoi0818" src="https://github.com/nminhkhoi0818.png" width="60px" height="auto"></a>&nbsp;&nbsp;&nbsp;
  <a href="https://github.com/kru01"><img alt="kru01" src="https://github.com/kru01.png" width="60px" height="auto"></a>&nbsp;&nbsp;&nbsp;
  <a href="https://github.com/SennHCMUS"><img alt="SennHCMUS" src="https://github.com/SennHCMUS.png" width="60px" height="auto"></a>&nbsp;&nbsp;&nbsp;
  <a href="https://github.com/vmathu"><img alt="vmathu" src="https://github.com/vmathu.png" width="60px" height="auto"></a>
</div>

<p align="right">(<a href="#readme-top">back to top</a>)</p>
