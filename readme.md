# Knight Tour using Multithreading
As a group assignment, we made a C program (using C libraries for pthread & synchronisation) to solve the knight’s tour problem given an NxN board and the starting position of the knight. Given the starting position of the knight on an NxN board the starting position will be a coordinate (X, Y) with X and Y between (N=0…N-1). Print out a possible path the knight can take such that it covers all the squares (cells) in that board exactly once. If no such path is possible print “No Possible Tour”. The knight may move two cells vertically and one cell horizontally or two cells horizontally and one cell vertically. Please note the following details:
- N is between [1,50]. The value of N will be supplied as a command line argument. 
- 0 <= X,Y < N the starting position of the knight on the grid, supplied as command line arguments, the positions are 0 indexed.
So, the program should run as : 
```
./knight.out N X Y
```
The program should print a solved path of the tour. If no tour is possible from that start point print ```No Possible Tour```. Otherwise print the (x,y) 0 indexed position of the Knight at after each move in the tour till the entire tour is printed out.
There can be multiple solutions for a given N X Y triplet. Printing one possible solution is sufficient.
Examples:
```
N = 5, X = 1, Y = 1
Output:
1,1|3,0|2,2|0,3|2,4|4,3|3,1|1,0|0,2|1,4|3,3|4,1|2,0|0,1|1,3|3,4|4,2|2,1|4,0|3,2|4,4|2,3|0,4|1,2|0,0|
```
```
N = 8, X = 3, Y = 2
Output:
3,2|5,3|7,4|6,6|4,7|2,6|0,7|1,5|3,6|5,7|4,5|3,7|1,6|0,4|2,5|4,6|6,7|5,5|7,6|6,4|5,6|7,7|6,5|4,4|2,3|3,5|2,7|0,6|1,4|3,3|5,2|4,0|6,1|7,3|5,4|7,5|6,3|7,1|5,0|3,1|1,0|0,2|2,1|0,0|1,2|2,0|0,1|1,3|3,4|4,2|3,0|1,1|0,3|2,2|4,1|6,0|7,2|5,1|7,0|6,2|4,3|2,4|0,5|1,7|
```
```
N = 3, X = 1, Y = 1
Output:
No Possible Tour
```
