# 15 Puzzle with A*

## Problem Statement
The 15-Puzzle, a classic sliding puzzle game, challenges players to arrange a set of
numbered tiles in ascending order within a 4x4 grid, with one tile missing to allow
sliding movements. While simple in concept, solving the 15-Puzzle optimally
presents a computational challenge due to its combinatorial complexity.
The aim of our project is to develop and implement a parallelized version of the A*
algorithm to solve the 15-Puzzle efficiently, with minimal moves required.

## Approach
We've devised both **sequential** and **parallel** versions of the 15-Puzzle solver,
employing Manhattan distance as the heuristic measure.
where 
`Manhattan Distance = abs(goal - current)`

## Sequential Approach

The sequential approach starts by initializing a temporary vector to store the path
from the initial state to the goal state, a hash set to keep track of visited states, and a
priority queue to manage states available for exploration.

During the exploration process, states are expanded iteratively from the priority
queue. If the current state matches the goal state, the path is reconstructed by
tracing back from the goal state to the initial state using the temporary vector.

Throughout the process, states are added to the hash set once visited, ensuring
efficient exploration by avoiding revisiting previously explored states. Additionally,
Manhattan distance serves as the heuristic measure, guiding the search towards the
goal state.

## Parallel Approach

This `TSPriorityQueue` class provides a thread-safe priority queue implementation with
multiple buckets for parallel processing.
It ensures synchronized access to the priority queues and hash sets using mutex locks,
allowing concurrent operations on different buckets.
When `bucketMultiplier` is set to -1, each thread manages its own bucket in the priority
queue.
If `bucketMultiplier` is n, the number of buckets is n times the number of threads.
The priority queue is structured with multiple buckets, where each bucket is assigned to
a single thread for parallel processing.

We have utilized a multi-threaded implementation to concurrently explore the search
space and find the optimal solution for the 15-Puzzle.

We initialized a thread-safe priority queue open with a specified number of buckets to
distribute the workload among threads effectively. Each thread runs the parallelThread
function, which continuously expands states from the priority queue until the optimal
solution is found or a termination condition is met.

To ensure efficiency and avoid redundant work, we employ a handle_tick function to
check if the goal state has been reached by any thread. If so, the search is terminated,
and the optimal solution is obtained.


## Performance Metrics
![image](https://github.com/smruthi49/Sem6-Projects/assets/94833021/5e58e6bd-e577-40a2-96eb-075dd2a51991)
![image](https://github.com/smruthi49/Sem6-Projects/assets/94833021/4d5d9f21-8fce-43da-a170-1fc5bbf8dc97)

## Conclusion

When considering Amdahl's Law in this context, it becomes evident that while
parallelization offers significant improvements in computational efficiency, there
are practical limits to the extent of these enhancements. Despite employing
parallel algorithms such as parallel A* or employing multi-threaded
implementations, the speedup achieved may not scale linearly with the number
of processing units.

