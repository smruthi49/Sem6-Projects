void sequential() {
	//temporarily store path from start to goal
	std::vector<State*> tempPath;

	//hash set to store states - to avoid duplicates
	std::unordered_set<State*, stateHash, stateEqual> hash;

	//priority queue to store states
	PriorityQueue<State*> open;

	//initialize the start state
	hash.insert(start);
	//start state marked open to be expanded
	start->addOpen();
	//push start state to the priority queue
	open.push(start);

	//number of states expanded
	int expanded = 0;

	//loop until the priority queue is empty
	while (!open.empty()) {
		//for every 100,000 states expanded, print a message
		if (expanded%100000 == 0) {
			printf("Finding optimal solution...\n");
		}

		expanded++;

		//get the state with the lowest f value
		//assign it to cur for expansion
		State* cur = open.pop();

		//check if the current state is the goal state
		if (*cur == *goal) {

			//reconstruct the path from goal to start
			// by following the prev pointers of each state
			while (cur != start) {
				tempPath.push_back(cur);
				cur = cur->getPrev();
			}
			//add it to path
			tempPath.push_back(start);
			
			//path length and final path
			int pathLength = tempPath.size();
			for (int i = 0; i < pathLength; i++) {
				path.push_back(tempPath[pathLength-1-i]);
			}

			return;
		}
		
		//current state is no longer open
		cur->removeOpen();

		//get the neighbors of the current state
		std::vector<State*> neighbors = cur->getNeighbors();
		
		//tentaive g value for the neighbors from start
		int altG = cur->getG() + 1;

		for (int i = 0; i < neighbors.size(); i++) {
			State* neighbor = neighbors[i];

			std::unordered_set<State*, stateHash, stateEqual>::iterator it = hash.find(neighbor);
			
			//check if the neighbor is already in the hash set
			if (it != hash.end()) {
				// found existing state
				delete neighbor;
				//we dont need same neighbor again
				neighbor = *it;

				//check if the new g value is less than the existing g value
				//check if we got a shorter path to the neighbor
				if (altG < neighbor->getG()) {
					neighbor->setPrev(cur);
					neighbor->setG(altG);
					//if neighbor is in open list, update it
					if (neighbor->checkOpen()) {
						open.update(neighbor);            
					} else {
						open.push(neighbor);
					}
				}
			} else { //if neighbor is not in hash set
				// new state
				//insert it in hash set
				hash.insert(neighbor);
				//mark it open
				neighbor->addOpen();
				//set the previous state
				neighbor->setPrev(cur);
				//set the g value
				neighbor->setG(altG);
				//push it to the priority queue
				open.push(neighbor);
			}
		}
	}

	return;
}