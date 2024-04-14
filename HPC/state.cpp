#include <new> //new-header for features of dynamic memory management

// Public Variables :
// 1. heapIdx - index of the state in the heap
// Public Function :
// 1. getF - f -> total estimated cost of reaching goal state from current state
// 2. setG - g -> cost of reaching current state from start state
// 3. getG - g
// 4. getH - h -> heuristic cost of reaching goal state from current state
// 5. setPrev - set previous state
// 6. getPrev - get previous state
// 7. addOpen - add state to open list - states that can be further explored
// 8. removeOpen - remove state from open list
// 9. checkOpen - check if state is in open list
// 10. getHash - get hash value of state
// 11. operator< - compare two states based on f and g values - to sort in priority queue
// 12. getNeighbors - . It returns a vector of pointers to neighboring states.
// 13. operator== - compare two states based on their hash values
// 14. toString - return string representation of state
// 15. ~State - destructor 

// Protected Variables :
// 1. f - total estimated cost of reaching goal state from current state
// 2. g - cost of reaching current state from start state
// 3. h - heuristic cost of reaching goal state from current state
// 4. prev - pointer to previous state
// 5. inOpen - boolean to check if state is in open list
// 6. hashval - hash value of state
// Protected Functions :
// 1. initNew - initialize state with g value
// 2. setF - set f value
// 3. computeH - compute heuristic cost
// 4. computeHash - compute hash value


class State {

public: //public variables and functions
    int heapIdx;
    
    int getF() {
        return f;
    }

    void setG(int gValue) {
        g = gValue;
        setF();
    }

    int getG() {
        return g;
    }

    int getH() {
        return h;
    }

    void setPrev(State* s) {
        prev = s;
    }

    State* getPrev() {
        return prev;
    }

    void addOpen() {
        inOpen = true;
    }

    void removeOpen() {
        inOpen = false;
    }

    bool checkOpen() {
        return inOpen;
    }

    size_t getHash() {
        return hashval;
    }
    
    bool operator<(State& other) {
        if (getF() < other.getF()) return true;
        if ((getF() == other.getF()) && (getG() > other.getG())) return true;
        return false;
    }

    virtual std::vector<State*> getNeighbors() = 0;

    virtual bool operator==(const State& other) = 0;

    virtual std::string toString() = 0;

    virtual ~State() {}

protected:
    int f;
    int g;
    int h;
    State* prev;
    bool inOpen;
    size_t hashval;

    void initNew(int gval) {
        g = gval;
        h = computeH();
        setF();
        prev = NULL;
        inOpen = false;
        hashval = computeHash();
    }

    void setF() {
        f = g+h;
    }

    virtual int computeH() = 0;

    virtual size_t computeHash() = 0;

};

struct stateHash {
public:
	size_t operator()(State* s) const {
		return s->getHash();
	}
};

struct stateEqual {
public:
	bool operator()(State* s1, State* s2) const {
		return *s1 == *s2;
	}
};