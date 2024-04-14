//If bucketMultiplier is -1, numBuckets is set to numThreads. 
//Otherwise, numBuckets is set to bucketMultiplier * numThreads
//The open priority queue is initialized with numBuckets
// each bucket can be processed by a separate thread


TSPriorityQueue<State*, stateHash, stateEqual> open;
int lock = 0;
int numBuckets;

//searches for goal state in open
bool handle_tick() {
    State* element;
    //if goal state is not in open, return false
    if ((element = open.find(goal)) == NULL) {
        return false;
    }

    //if goal state is in open, retrieve f value of goal state
    int fval = element->getF();

    //for each bucket, 
    //if any bucket has a state with f value less than f value of goal state, 
    // return false
    //because this goal state is not optimal
    for (int i = 0; i < numBuckets; i++) {
        if (open.getMinKey(i) < fval) {
            return false;
        }
    }
    return true;
}

//parallel thread function
void* parallelThread(void* arg) {
    //extract thread id from argument
    int thread_id = (int)(long long)(arg);
    int expanded = 0;

    while (1) {

        if (thread_id == 0 && expanded%10000 == 0) {
            if (expanded % 100000 == 0) {
                printf("Finding optimal solution...\n");
            }
            //if optimal solution is found, return
            if (handle_tick()) {
                //set lock to 1 to signal other threads to return
                lock = 1;
                return NULL;
            }
        }

        expanded++;

        State* cur = NULL;
        //fetch state from open
        while (cur == NULL) {
            //if lock is set, return
            if (lock == 1) {
                return NULL;
            }
            //if bucketMultiplier is -1, pop from thread_id bucket
            //otherwise, pop from random bucket
            if (bucketMultiplier == -1) {
                cur = open.pop(thread_id);
            } else {
                cur = open.pop(rand()%numBuckets);
            }
        }
        
        cur->removeOpen();

        //if goal state is found, return
        std::vector<State*> neighbors = cur->getNeighbors();
        
        //for each neighbor of current state,
        //push neighbor into open
        for (int i = 0; i < neighbors.size(); i++) {
            State* neighbor = neighbors[i];
            open.push(neighbor, cur);
        }
    }

    return NULL;
}

//create path from start to goal
void createPath() {
    std::vector<State*> tempPath;

    //start from goal state
    State* cur = open.find(goal);

    //while current state is not NULL and current state is not start state,
    //add current state to tempPath
    while (cur != NULL && cur != start) {
        tempPath.push_back(cur);
        cur = cur->getPrev();
    }
            
    tempPath.push_back(start);
    
    //determine path length
    int pathLength = tempPath.size();
    for (int i = 0; i < pathLength; i++) {
        path.push_back(tempPath[pathLength-1-i]);
    }

    return;
}

//parallel function
void parallel(int numThreads) {

    //if bucketMultiplier is -1, set numBuckets to numThreads
    //otherwise, set numBuckets to bucketMultiplier*numThreads
    if (bucketMultiplier == -1) {
        numBuckets = numThreads;
    } else {
        numBuckets = bucketMultiplier*numThreads;
    }

    //initialize random seed
    srand(time(NULL));

    //initialize open priority queue
    open.init(numBuckets);

    //initialize threads
    pthread_t threads[numThreads];

    //push start state into open
    open.push(start, NULL);

    //create threads
    for (int i = 0; i < numThreads; i++) {
        pthread_create(&threads[i], NULL, &parallelThread, (void*)(long long)i);
    }

    //join threads
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    createPath();
}