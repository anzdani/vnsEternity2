
# vnsEternity2
============
## About
This project deals with an implementation of a [Variable Neighborhood Search](http://en.wikipedia.org/wiki/Variable_Neighborhood_Search) approach for solving [Eternity II puzzle](http://en.wikipedia.org/wiki/Eternity_II_puzzle).
It is a assignment for Optimization methods and algorithms course of MS Computer Engineering at Polytechnic of Turin.  
Please note that not every C files currently included is necessary for the latest version of the program, but they are provided in order to allow successive improvements of the program, (for instance speed starting, shaking for very large neighborhood) .

The files are:
* **main.c:** the main of the program.  
* **solver.c:** various functions to solve some parts of the algorithm.
* **support.c:** supporting functions, like parser and printer.
* **hungarian.c, hun_methods.c:** a library for using the hungarian algorithm, no more used by the current version of the program.
* **optimization.h:** the header containing all constants used throughout the program.  

## Installation 
First clone the repository:
```
git clone https://github.com/danjok/vnsEternity2.git
```
Change into the repository directory and compile sources:
```
$ cd vnsEternity2/
$ make
```
## Usage  
The executable file will be located in bin folder. Some input files are provided in files folder.  

The program has a command-line interface with this syntax:
```
$ ./vns [-t <time in seconds>] [-p <maximum score>] <input file> <output file>
```
For instance:
```
$ ./vns -t 100 -p 100 ../files/pieces_10x10.txt ../files/sol.txt
```

Input file and output file must be in .txt format. Input file must be ordered according to the sequence Corners, Borders and Centers and 0 (border color) must be placed at the beginning of the sequence of colors.
It's necessary to provide at least one of the two stop condition.

The output is put in the output file according to the standard.
The output on the console is in the form of:
```
<Global Time> - <Time since last Restart> <Number of Restarts> <Best Score among all the Restarts>
```
During a restart an additional output is visualized
```
<Global Time> <Number of Restarts> <Best Score among all the Restarts>
```
At the end the colored table of the best score is provided.

### Visualization
An additional tool can be used to have a better visualization of the output solution. To run it:
```
$ java -jar visualizer.jar <inputfile> <outputfile> 0
```
