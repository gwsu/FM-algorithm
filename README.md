# Fiduccia and Mattheyses algorithm (FM algorithm)

This is the implementation of the Fiduccia and Mattheyses algorithm (FM algorithm) in C++.
But It is a little bit different from standard version of FM algorithm, It allowed one cell has different cell weight at different group and the goal is to balance the weight of each group. 

This project is a course project for Physical-Design-Automation.

## Get started.

```bash
$ mkdir build && cd build
$ cmake ..
$ ./bin/bw_partitioner <cells file>.cells <nets file>.nets <output file>.out 
```

## Library
I also provide the library version with CMake
