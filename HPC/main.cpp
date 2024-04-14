#include <iostream>
#include <vector>
#include <unordered_set>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <getopt.h>
#include <unistd.h>
#include <chrono>

#include "state.cpp"
#include "board.cpp"

State* start;
State* goal;
std::vector<State*> path;
int numThreads = 0;
int bucketMultiplier = -1;

#include "priorityqueue.cpp"
#include "tspriorityqueue.cpp"
#include "sequential.cpp"
#include "parallel.cpp"

int main(int argc, char *argv[]) {
    int size = 4;
    int moves = -1;
    std::string inputFile = "";
    int opt;
    while ((opt = getopt(argc, argv, "t:b:s:m:f:")) != -1) {
        switch (opt) {
            case 't':
                numThreads = atoi(optarg);
                break;
            case 'b':
                bucketMultiplier = atoi(optarg);
                break;
            case 's':
                size = atoi(optarg);
                break;
            case 'm':
                moves = atoi(optarg);
                break;
            case 'f':
                inputFile = optarg;
                break;
        }
    }

    //file not empty
    if (inputFile.empty() == false) {
        start = (State*)(new Board(inputFile));
    }
    std::cout << "Start board : " << std::endl;
    std::cout << start->toString() << std::endl;

    if (numThreads == 0) {
        std::cout << "Running sequential baseline..." << std::endl;
    } else {
        std::cout << "Running parallel version with " << numThreads << " threads..." << std::endl;
    }
    if (bucketMultiplier != -1) {
        int numBuckets = bucketMultiplier * numThreads;
        std::cout << "Using " << numBuckets << " buckets..." << std::endl;
    }

    goal = (State*)(new Board(size, 0));

    auto start_t = std::chrono::high_resolution_clock::now();

    if (numThreads == 0) {
        sequential();
    } else {
        parallel(numThreads);
    }

    auto end_t = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> time = end_t - start_t;

    double time_ms = time.count();


    std::cout << "Optimal solution found!" << std::endl << std::endl;
    int length = path.size();
    for (int i = 0; i < length; i++) {
        std::cout << "Step " << i << ":" << std::endl;
        std::cout << path[i]->toString() << std::endl;
    }
    std::cout << "Length of path: " << length-1 << std::endl;
    std::cout << "Total time: " << time_ms << "ms" << std::endl;

    return 0;
}