# Fun C++ Stuff

## Belady's Anomaly Test

This program demonstrates the frequency of Belady's Anomaly. It creates 100 random sequences of pages to be requested. The length of each sequence is 1000, and there are 250 pages to choose from. Each sequence is then ran through a FIFO page replacement algorithm with frames from 1 to 100. The number of page faults is counted for each run-through of the algorithm. If the number of page faults increases when the number of frames also increases, then an anomaly is detected and is printed to the console.

To learn more about Belady's Anomaly check out the [Wikipedia article](https://en.wikipedia.org/wiki/B%C3%A9l%C3%A1dy%27s_anomaly).

This code was originally written in Fall 2017 with help from Dean Mathias.

## Big Integer

The BigInteger class allows the handling of astronomical numbers. It includes the following:

* Various constructors
* Copy constructor
* Move constructor
* Overloaded assignment operator
* Overloaded move assignment operator
* Overloaded `+` operator
* Overloaded `+=` operator
* Overloaded `*` operator
* Overloaded `+=` operator
* Overloaded `==` operator
* Overloaded `<=` operator
* Overloaded postfix increment operator `++`
* Overloaded extraction operator `<<`
* Overloaded double conversion operator
* Destructor

The main.cpp does some basic testing of the BigInteger class. It utilizes a factorial result from [http://www.dcode.fr/factorial](http://www.dcode.fr/factorial) to verify some calculations. Rigorous testing for the class has yet to be written.

This code was originally written in Fall 2016 with help from Dean Mathias.

## Multithreaded PiComputer

This program utilizes an implementation of Simon Plouffe's pi computing algorithm written by Fabrice Bellard. See [this Stack Overflow post](http://stackoverflow.com/questions/5905822/function-to-find-the-nth-digit-of-pi) as well as [Bellard's website here](http://bellard.org/pi/pi_n2/pi_n2.html) for the original code.

The main purpose of writing this program was to demonstrate familiarity with writing multithreaded code in C++.

This code was originally written in Fall 2017 with help from Dean Mathias.

## Shell Program

This program launches a shell program which accepts normal commands that may be run in a shell. It has four built-in functions: `cd`, `history`, `^ k`, and `ptime`. All other commands are attempted to be run as child processes. Invalid input is handled gracefully.

* `ptime` gives the total time that child processes have spent running
* `history` gives a list of all the commands that have been run
* `^ k` runs the kth command. `k` must be a valid integer
* `cd` changes the current directory as expected

Type `exit` to exit the shell program. Interrupt signals will be caught; therefore this is the preferred way to exit the program.

Piping between commands is supported, including piping between built-in and non-built-in commands.

*This program is only designed to run on Linux.*

This code was originally written in Fall 2017 with help from Dean Mathias.