Damien C. Sardinha
Operating System - README
December 19th, 2018

	This project uses the C programming language to build the concept of an operating system using a linux source code library. Over the course of my fall semester, I was assigned several projects that eventually build to a bootable floopy disc that could load and execute a shell program. Total power of this featured operating system is about the equivalent to CP/M back in the 1980's or better yet, MS-DOS version 1. 

	Kernel.c and Shell.c are the two main program files that need to be compiled and loaded to a floppy disk image file before executing in a simulator or real computer.

To complile kernel.c and shell.c, run ./compileOS.sh in a terminal window. Each additional file must be loaded separately using ./loadFile. ex. ./loadFile shell

Availible files are message.txt, tstprg, tstpr2, and shell

How to verify?

	Double click simulator.jar and make sure that floppya.img is
selected on the first bootable image. Once running, the floppy should prompt
"Shell> " and to verify shell has loaded properly.

Type "dir " to list all the files on the floppy and create your own text file with "create" and (filename) proceding. 
ex. create messa2
