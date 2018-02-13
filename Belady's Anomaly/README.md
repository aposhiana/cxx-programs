## Belady's Anomaly Test

This program demonstrates the frequency of Belady's Anomaly. It creates 100 random sequences of pages to be requested. The length of each sequence is 1000, and there are 250 pages to choose from. Each sequence is then ran through a FIFO page replacement algorithm with frames from 1 to 100. The number of page faults is counted for each run-through of the algorithm. If the number of page faults increases when the number of frames also increases, then an anomaly is detected and is printed to the console.

To learn more about Belady's Anomaly check out the [Wikipedia article](https://en.wikipedia.org/wiki/B%C3%A9l%C3%A1dy%27s_anomaly).

This code was originally written in Fall 2017 with help from Dean Mathias.