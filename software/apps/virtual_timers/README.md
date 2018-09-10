Virtual Timers
==============

The goal of this lab is to write a virtual timer library. You will use
one hardware timer to create a timer library that handles any number of timers.

The lab in order:
0) Read the timer and print it

1) Set up a single hardware timer interrupt and handle that interrupt

2) Create a timer library that can handle one timer at a time (with linked list)
    - Calculate time required for a time in ms and set up the timer
    - Insert into linked list
    - Call the callback function using function pointers

3) Add repeated mode to the timer library

4) Make two timers
    - Handle timers created out of earliest deadline order

5) Make N timers

6) Check for correctness in all cases
    - N timers firing very close to the same time (no timers should be dropped)
    - Add a very short timer to the list (does it keep firing?)
    - Remove timers from the list (do other timers keep firing?)

