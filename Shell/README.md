## Shell Program

This program launches a shell program which accepts normal commands that may be run in a shell. It has four built-in functions: `cd`, `history`, `^ k`, and `ptime`. All other commands are attempted to be run as child processes. Invalid input is handled gracefully.

* `ptime` gives the total time that child processes have spent running
* `history` gives a list of all the commands that have been run
* `^ k` runs the kth command. `k` must be a valid integer
* `cd` changes the current directory as expected

Piping between commands is supported including piping between built-in and non-built-in commands.

*This program is only designed to run on Linux.*

This code was originally written in Fall 2017 with help from Dean Mathias.