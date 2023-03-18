<a name="readme-top"></a>

# NachOS - Not Another Completely Heuristic Operating System

- Group project from HCMUS's 2023 Operating System course.

## Content

- Read the guides, especially ProjectInfo, for full details.
  - They are in vietnamese, ***maybe*** I'll do some translating once I find the time.

## Getting Started

### Prerequisites

* Ubuntu 14.04 32-bit (i386)
* gcc / g++
* binutils-2.11.2.tar.gz

### Installation

1. Clone the repo.
2. Open up terminal and cd into `nachos/nachos-3.4/code/`.
3. Run `make` command.
4. cd into `threads/`.
5. Run `./nachos` command.
6. If the output is similar to this, you're all set!

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

## Usage

### To compile NachOS

1. Open up terminal and cd into `nachos/nachos-3.4/code/`.
2. Run `make` command.

### To run a program

All of the user-level programs are stored in `nachos/nachos-3.4/code/test/`.

1. Open up terminal and cd into `nachos/nachos-3.4/code/`.
2. Run `./userprog/nachos –rs 1023 –x ./test/programName`.

### To make a program

1. Go into `nachos/nachos-3.4/code/test/`.
2. Create `programName.c` file and write your program in there.
3. Open `Makefile` in `test/` folder.
4. Add your program's name on line 38.

```make
all: halt shell matmult sort readprint programName
```

5. Scroll down and add the below lines of code to the file.

```make
programName.o: programName.c
	$(CC) $(CFLAGS) -c programName.c
programName: programName.o start.o
	$(LD) $(LDFLAGS) start.o programName.o -o programName.coff
	../bin/coff2noff programName.coff programName
 ```

6. Recompile NachOS.

## Built With

[ubuntuicon]: https://cdn.jsdelivr.net/gh/devicons/devicon/icons/ubuntu/ubuntu-plain.svg
[ubuntuurl]: https://releases.ubuntu.com/14.04/

| [![Ubuntu][ubuntuicon]][ubuntuurl] |
| :-: |
| 14.04 i386 |

## Meet The Team

<div align="center">
  <a href="https://github.com/phongan1x5"><img alt="phongan1x5" src="https://github.com/phongan1x5.png" width="60px" height="auto"></a>&nbsp;&nbsp;&nbsp;
  <a href="https://github.com/nminhkhoi0818"><img alt="nminhkhoi0818" src="https://github.com/nminhkhoi0818.png" width="60px" height="auto"></a>&nbsp;&nbsp;&nbsp;
  <a href="https://github.com/kru01"><img alt="kru01" src="https://github.com/kru01.png" width="60px" height="auto"></a>&nbsp;&nbsp;&nbsp;
  <a href="https://github.com/SennHCMUS"><img alt="SennHCMUS" src="https://github.com/SennHCMUS.png" width="60px" height="auto"></a>&nbsp;&nbsp;&nbsp;
  <a href="https://github.com/vmathu"><img alt="vmathu" src="https://github.com/vmathu.png" width="60px" height="auto"></a>
</div>

<p align="right">(<a href="#readme-top">back to top</a>)</p>
