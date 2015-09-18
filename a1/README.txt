Hayden Schmackpfeffer (hbs25)
EECS 338 Assignment 1
Due 9/17/15

To build this file run make all in directory with Makefile

Then ./main

Included is a typescript in which the file is made, ran, and outputted on the command line.

ANSWERS TO HW QUESTIONS:

Q1) Briefly Explain the Purpose of the Parent and Child Processes' Real and Effective User IDs and Their Group IDs.

A1) The real User id is always the user id of the user who ran the file. The effective user id is usually the same, but if the program is ran with the set-uid bit on, then the euid is the id of the user who owns the file, and the uid is the id of the user who ran the file.

  In a similar vein, the group id is the ID of the group that the user who ran the program belongs to. And the effective group id is the ID of the group that the user who owns the file belongs to.

Q2) Why might we observe zero time values if we dont loop our processes for a while?

A2) Present day CPUs can execute processes incredibly quickly, so quickly that the time it takes to finish a process is effectively 0. I had to loop one million times just to observe a process taking 4000ns to complete. 
