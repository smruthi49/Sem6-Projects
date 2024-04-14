//Thread Safe Priority Queue
//partitions the priority queue into multiple buckets
//each bucket has its own lock
//concurrent access to diffrent parts of priority queue

template <class T, class stateHash, class stateEqual>
//takes three parameters -
// T (the type of elements in the priority queue)
// stateHash (a hash function for the elements)
// stateEqual (a function to compare two elements)
class TSPriorityQueue {

public:

    typedef typename std::unordered_set<T, stateHash, stateEqual>::iterator iterator;

    TSPriorityQueue() {}

    void init(int n) {
        numBuckets = n;
        //initialize locks - one for each bucket
        locks = (pthread_mutex_t*)malloc(n * sizeof(pthread_mutex_t));

        //initialize priority queues and hash sets
        for (int k = 0; k < n; k++) {
            pthread_mutex_init(&locks[k], NULL);
            pqs.push_back(PriorityQueue<T>());
            hashes.push_back(std::unordered_set<T, stateHash, stateEqual>());
        }
    }

    //if priority queue in bucket k is empty
    //lcoks mutex associated with bucket k
    //checks if priority queue in bucket k is empty
    //unlocks mutex associated with bucket k
    bool empty(int k) {
        pthread_mutex_lock(&locks[k]);
        bool b = pqs[k].empty();
        pthread_mutex_unlock(&locks[k]);
        return b;
    }

    //find element in hash set
    //locks mutex associated with bucket k determined by hash value of element
    //find element in hash set
    //unlock mutex associated with bucket k
    T find(T element) {
        int k = bucketNum(element);
        T e = NULL;
        pthread_mutex_lock(&locks[k]);
        iterator it = hashes[k].find(element);
        if (it != hashes[k].end()) {
            e = *it;
        }
        pthread_mutex_unlock(&locks[k]);
        return e;
    }
    
    //size of priority queue in bucket k
    int size(int k) {
        pthread_mutex_lock(&locks[k]);
        int size = pqs[k].size();
        pthread_mutex_unlock(&locks[k]);
        return size;
    }

    //size of hash set in bucket k
    int hashSize(int k) {
        pthread_mutex_lock(&locks[k]);
        int size = hashes[k].size();
        pthread_mutex_unlock(&locks[k]);
        return size;
    }

    //push element into priority queue
    void push(T element, State* cur) {
        int k = bucketNum(element);
        pthread_mutex_lock(&locks[k]);
        int altG;
        if (cur == NULL) {
            altG = 0;
        } else { 
            altG = cur->getG() + 1;
        } 

        iterator it = hashes[k].find(element);
        if (it != hashes[k].end()) {
            // found existing state
            delete element;
            element = *it;

            if (altG < element->getG()) {
                element->setPrev(cur);
                element->setG(altG);
                if (element->checkOpen()) {
                    pqs[k].update(element);            
                } else {
                    pqs[k].push(element);
                }
            }
        } else {
            // new state
            hashes[k].insert(element);
            element->addOpen();
            
            element->setPrev(cur);
            element->setG(altG);
            pqs[k].push(element);
        }
        pthread_mutex_unlock(&locks[k]);
    }
    
    //pop element from priority queue
    T pop(int k) {
        pthread_mutex_lock(&locks[k]);
        T element = pqs[k].pop();
        pthread_mutex_unlock(&locks[k]);
        return element;
    }

    //remove element from priority queue 
    void remove(T element) {
        int k = bucketNum(element);
        pthread_mutex_lock(&locks[k]);
        pqs[k].remove(element);
        pthread_mutex_unlock(&locks[k]);
    }

    //update element in priority queue
    void update(T element) {
        int k = bucketNum(element);
        pthread_mutex_lock(&locks[k]);
        pqs[k].update(element);
        pthread_mutex_unlock(&locks[k]);
    }

    //get minimum key in priority queue
    int getMinKey(int k) {
        pthread_mutex_lock(&locks[k]);
        int key = pqs[k].getMinKey();
        pthread_mutex_unlock(&locks[k]);
        return key;
    }

private:

    pthread_mutex_t* locks;
    std::vector< PriorityQueue<T> > pqs;
    std::vector< std::unordered_set<T, stateHash, stateEqual> > hashes;
    int numBuckets;

    int bucketNum(T element) {
        //compute hash value of element
        //it is used to determine which bucket to use
        size_t hashval = stateHash()(element);
        //return hash value modulo number of buckets
        return hashval % numBuckets;
    }
    
};