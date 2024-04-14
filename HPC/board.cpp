class Board : public State {

public:
    int** board;
    int size;
    int emptyRow;
    int emptyCol;

    // make moves from goal board
    Board(int n, int moves) {
        size = n; //setting size of board

        board = new int*[size]; //allocating memory for board

        //filling board with numbers
        // GOAL BOARD
        for (int i = 0; i < size; i++) {
            board[i] = new int[size];
            int row = 1+i*size;
            for (int j = 0; j < size; j++) {
                board[i][j] = row+j;
            }
        }
        //last element is 0
        board[size-1][size-1] = 0;

        //the index of the empty space
        emptyRow = size-1;
        emptyCol = size-1;

        //make random moves
        srand(time(NULL)); //seed for random number generator

        while(moves--) {
            //change in x and y direction {up, down, left, right}
            int dx[4] = {-1, 1, 0, 0};
            int dy[4] = {0, 0, -1, 1};

            // array to keep track of valid move directions.
            int valid[4] = {0, 0, 0, 0};

            int numvalid = 0;
            if (emptyRow > 0) { valid[0] = 1; numvalid++;}
            if (emptyRow < size-1) { valid[1] = 1; numvalid++;}
            if (emptyCol > 0) { valid[2] = 1; numvalid++;}
            if (emptyCol < size-1) { valid[3] = 1; numvalid++;}

            //if no valid moves, initialize the state
            if (numvalid == 0) { initNew(0); return; }

            int idx = 0;
            int swapidx = rand() % numvalid;

            //Randomly selects a valid move direction.
            for (int i = 0; i <= swapidx; i++) {
                if (i > 0) idx++;
                while (valid[idx] == 0) idx++;
            }

            //tracking empty space
            int newEmptyRow = emptyRow + dx[idx];
            int newEmptyCol = emptyCol + dy[idx];
            
            //swapping the empty space with the number
            board[emptyRow][emptyCol] = board[newEmptyRow][newEmptyCol];
            board[newEmptyRow][newEmptyCol] = 0;

            //updating the empty space
            emptyRow = newEmptyRow;
            emptyCol = newEmptyCol;
        }

        initNew(0);
    }

    // initialize board based on file
    Board(std::string s) {
        FILE *fp = fopen(s.c_str(), "r");
        fscanf(fp, "%d", &size);

        //allocate memory for board
        board = new int*[size];
        for (int i = 0; i < size; i++) {

            board[i] = new int[size];

            //read the size of the board
            int row = i*size;
            for (int j = 0; j < size; j++) {
                //read the value and store it in the board
                fscanf(fp, "%d", &board[i][j]);
                if (board[i][j] == 0) {
                    emptyRow = i;
                    emptyCol = j;
                }
            }
        }
        fclose(fp);

        initNew(0);
    }

    // return the neighbors of the board
    std::vector<State*> getNeighbors() {

        //array to store the neighbors
        std::vector<State*> neighbors;

        if (emptyRow > 0) {
            //move up
            Board* nbr = new Board(this, -1, 0);
            neighbors.push_back((State*)nbr);
        }
        if (emptyRow < size-1) {
            //move down
            Board* nbr = new Board(this, 1, 0);
            neighbors.push_back((State*)nbr);
        }
        if (emptyCol > 0) {
            //move left
            Board* nbr = new Board(this, 0, -1);
            neighbors.push_back((State*)nbr);
        }
        if (emptyCol < size-1) {
            //move right
            Board* nbr = new Board(this, 0, 1);
            neighbors.push_back((State*)nbr);
        }
        return neighbors;
    }

    //compare whether two boards are equal
    //to keep track of states tha has already been visited
    bool operator==(const State& other) {
        Board* b = (Board*)(&other);
        if (size != b->size) return false;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] != b->board[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    //change to string  
    std::string toString() {
        std::string s;
        s += "empty space Row = " + std::to_string(emptyRow) + " ";
        s += "empty space Col = " + std::to_string(emptyCol) + "\n";
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                s += std::to_string(board[i][j]);
                s += " ";
            }
            s += "\n";
        }
        return s;
    }

private:

    // copy board and make a move
    // dx and dy are the change in x and y direction
    Board(Board* b, int dx, int dy) {
        //size of the board
        size = b->size;

        //update the empty space position
        emptyRow = b->emptyRow+dx;
        emptyCol = b->emptyCol+dy;

        board = new int*[size];

        //copy the board b to the new board
        for (int i = 0; i < size; i++) {
            if (i == emptyRow || i == b->emptyRow) {
                board[i] = new int[size];
                for (int j = 0; j < size; j++) {
                    board[i][j] = b->board[i][j];
                }
            } else {
                board[i] = b->board[i];
            }
        }

        //swap the empty space with the number
        board[emptyRow][emptyCol] = b->board[b->emptyRow][b->emptyCol];
        board[b->emptyRow][b->emptyCol] = b->board[emptyRow][emptyCol];

        //initialize the board with max possible g value
        //to be updated later during the search
        initNew(INT_MAX);
    }

    //compute the heuristic value
    int computeH() {
        //to store the manhattan distance
        int total = 0;

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {

                int element = board[i][j];
                //if the element is 0, skip
                if (i == emptyRow && j == emptyCol) continue;

                //calculate the manhattan distance between the current position and the goal position
                int row = (element-1)/size;
                int col = (element-1)%size;
                int distance = abs(row-i) + abs(col-j);
                total += distance;
            }
        }
        return total;
    }

    size_t computeHash() {
        //hash value
        size_t h = 0;

        //size of the board
        int size2 = size * size;

        //set the mod value
        size_t mod = (1LL<<56)-5;

        //calculate the hash value
        // h * size of board + element
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                h = (h * size + board[i][j]) % mod;
            }
        }

        //bitwise XORs
        size_t v = h;
        v = v ^ (v >> 21);
        v = v ^ (v << 37);
        v = v ^ (v >> 4);
        v = v * 4768777513237032717;
        v = v ^ (v << 20);
        v = v ^ (v >> 41);
        v = v ^ (v <<  5);
        return v;
    }
};