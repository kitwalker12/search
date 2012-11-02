search
======

Breadth First Search / Uniform Cost Search

To Install:
make assign1

To Run:
./assign1

Info:
The program has a class Graph with functions to create the graph, add edges and perform BFS/UCS. A priority queue is implemented for expanding and marking nodes while traversing the graph. UCS and BFS is performed using the same function as BFS is nothing but UCS with edges having the same weight. The function call of BFS is made by sending 1,1,1,1 as costs of operations to UCS function. Normal values read from the input.txt are passed to perform UCS. The program takes no arguments and assumes input.txt is present in the same directory.