#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>

using namespace std;


#define GAMMA 0.8 // taxa de desconto
#define ALPHA 0.2
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define INF 999999

std::vector< std::vector<float> > q;
std::vector< std::vector<float> > r;
std::vector< float > reward;
vector<bool> isFinal;
vector<bool> visited;


int nColumns,nLines, totalValidCells;
float defaultValue;

bool isWall(int pos) {
		if (q[pos].size() == 0)
			return true;
		else
			return false;
	}

int getIndex(int line, int column) {
	return nColumns * line + column;
}

int getNeighbour(int pos, int action) {
	int line = (int) floor( (double ) pos / nColumns);
	int column = pos - (line * nColumns);
	int newPos;
	//cout << "Cur position:: " << pos << " Action: " << action << endl;
	if (action == UP) {
		newPos = getIndex(line-1,column);
		if ((line == 0) || isWall(newPos)) // wall or out of maze
			return pos;
		else
			return newPos;
	}
	else if (action == DOWN) {
		newPos = getIndex(line+1,column);
			if ((line == nLines - 1) || isWall(newPos)) // wall or out of maze
				return pos;
			else
				return newPos;
		}
		else if (action == LEFT) {
			newPos = getIndex(line,column-1);
			if ((column == 0) || isWall(newPos)) // wall or out of maze
				return pos;
			else
				return newPos;
		}
		else if (action == RIGHT) {
			newPos = getIndex(line,column+1);
			if ((column == nColumns-1) || isWall(newPos)) // wall or out of maze
				return pos;
			else
				return newPos;

	}

}
	string actionPrint(int action) {
		switch(action) {
			case 0:
				return "UP";
				break;
			case 1:
				return "DOWN";
				break;
			case 2:
				return "LEFT";
				break;
			case 3:
				return "RIGHT";
				break;
			default:
				return "N";
				break;
		}	
	}
	float calculateQ(int pos, int action) {
		float ret = 0;
		//ret = q[pos][action];
		ret = (1-ALPHA) * q[pos][action];
		int nextState = getNeighbour(pos,action);
		float maxFactor = -INF;
		float val;
		//cout << "Current Position: " << pos << " Next position: " << nextState << " by Action: " << actionPrint(action) << endl;
		for (int i = 0; i < 4; i++) {
			val = q[nextState][i];
			if (val > maxFactor)
				maxFactor = val;
		}
		ret += ALPHA * (r[pos][action] + (GAMMA*maxFactor));
		return ret;
	}

	float normalizeQ(int index, vector<float> &v) {
		if (index == UP) {
			return (0.8 *v[index]) + (0.1 * v[LEFT]) + (0.1 * v[RIGHT]);
		}
		else if (index == DOWN) {
			return (0.8 *v[index]) + (0.1 * v[LEFT]) + (0.1 * v[RIGHT]);
		}
		else if (index == LEFT) {
			return (0.8 *v[index]) + (0.1 * v[UP]) + (0.1 * v[DOWN]);
		}
		else if (index == RIGHT) {
			return (0.8 *v[index]) + (0.1 * v[UP]) + (0.1 * v[DOWN]);
		}
		return -INF;
	}

	bool hasEquals(vector<float> &v,float bestValue, int pos, vector<int> &ret) {
		int totEqual = 0;
		vector<int> retSub;
		for (int i=0; i < v.size(); i++)
			if (v[i] == bestValue) {
				totEqual++;
				if (!(getNeighbour(pos,i) == pos)) { // not going to the same place
					retSub.push_back(i);
					if (!visited[getNeighbour(pos,i)]) // better if not visited
						ret.push_back(i);
				}
			}
		if (ret.size() == 0) {
			ret.clear();
			for (int i=0; i < retSub.size(); i++) {
				ret.push_back(retSub[i]);
			}
		}


		if (totEqual > 1)
			return true;
		else
			return false;
	}

	bool performRandomAction() {
		int probConst = 50;
		int eta = count(visited.begin(),visited.end(),true) / totalValidCells;
		eta = eta / 2; // amortize constant
		int probRes = probConst;
		srand (time(NULL));
		int randNum = rand() % 100;
		if (randNum < probConst)
			return true;
		else
			return false;
	}

	void selectActions(int pos, vector<int> &v) {
		vector<int> retSub;
		for (int i=0; i < 4; i++)
			if (!(getNeighbour(pos,i) == pos)) { // not going to the same place
				retSub.push_back(i);
					if (!visited[getNeighbour(pos,i)]) // better if not visited
						v.push_back(i);
			}
		if (v.size() == 0) {
			v.clear();
			for (int i=0; i < retSub.size(); i++) {
				v.push_back(retSub[i]);
			}
		}
	}

	int performEpisode(int pos) {
		for (int i=0; i < nLines*nColumns; i++)
			visited[i]= false;

		int totalSteps = 0;
		bool isFinalState = false;
		vector<float> valuesTmp; // 
		vector<float> values; // 
		values.resize(4); // 
		valuesTmp.resize(4); 
		int curPos = pos;
		do {			
			visited[curPos] == true;
			int bestMove;
			float bestValue; 
			if (performRandomAction()) {
				vector<int> actions;
				selectActions(curPos,actions);
				if (actions.size() == 0) {
					bestMove = rand() % 4;
				}
				else {
					int bestMoveIndex = rand() % actions.size();
					bestMove = actions[bestMoveIndex];
				}
				bestValue = calculateQ(curPos,bestMove);
				vector<float> v; // float
				v.push_back(bestValue);
				bestValue = normalizeQ(0,v);
				if (q[curPos][bestMove] < bestValue)
					q[curPos][bestMove] = bestValue;
			}
			else {
				srand (time(NULL));			
				for (int i=0; i < valuesTmp.size(); i++)
					valuesTmp[i] = calculateQ(curPos,i); 	
				for (int i=0; i < values.size(); i++) {
					values[i] = normalizeQ(i,valuesTmp); 
					//cout << values[i] << endl;
				}
				srand (time(NULL));
				bestValue = *max_element(values.begin(),values.end());
				
				vector<int> bestPositions;
				if (hasEquals(values,bestValue,curPos,bestPositions)) { // float
					if (bestPositions.size() == 0) {
						size_t tmp = max_element(values.begin(),values.end()) - values.begin();
						bestMove = static_cast<int>(tmp);
					}
					else {
					int randVal = rand() % bestPositions.size();
					bestMove = bestPositions[randVal];
					}
				}
				else {
					size_t tmp = max_element(values.begin(),values.end()) - values.begin();
					bestMove = static_cast<int>(tmp);
				}
				q[curPos][bestMove] = bestValue;
			}
			if (curPos == 10) {
				//cout << "10 will execute: " << actionPrint(bestMove) << endl;
			}

			
		//int bestMove = find(values.begin(),values.end(),bestValue) - values.begin(); // UP, DOWN, LEFT, RIGHT
		//cout << bestMove << endl;
		//cout << bestValue << endl;
		
		int nextPos = getNeighbour(curPos,bestMove);
		if (curPos != nextPos) {
			//cout << "Current position: " << curPos << " Next one: " << nextPos << endl;
		}
		//cout << "Current position: " << curPos << " Next one: " << nextPos << " Move made: " << actionPrint(bestMove) << endl;
		if (isFinal[nextPos])
			isFinalState=true;
		curPos = nextPos;
		totalSteps++;
	} while (!isFinalState);
	cout << "Perform made - > Current position: " << curPos;
	return totalSteps;
	

}


void initVars() {

	// init r(state,action)
	for (int i=0; i < nLines*nColumns; i++) {
		if (!isWall(i)) {
			int upNode = getNeighbour(i,UP);
			int downNode = getNeighbour(i,DOWN);
			int leftNode = getNeighbour(i,LEFT);
			int rightNode = getNeighbour(i,RIGHT);
			r[i][UP] = (0.8*reward[upNode]) + (0.1 * reward[leftNode]) + (0.1 * reward[rightNode]);
			r[i][DOWN] = (0.8*reward[downNode]) + (0.1 * reward[leftNode]) + (0.1 * reward[rightNode]);
			r[i][LEFT] = (0.8*reward[leftNode]) + (0.1 * reward[upNode]) + (0.1 * reward[downNode]);
			r[i][RIGHT] = (0.8*reward[rightNode]) + (0.1 * reward[upNode]) + (0.1 * reward[downNode]);
		}
	}
}

void printQ() {
	for (int i=0; i < nLines; i ++) {
		for (int j=0; j < nColumns; j++) {
			int pos = getIndex(i,j);
			int bestMove = -1;
			if (!isWall(pos)) {
				float bestVal = -INF;
				for (int index = 0; index < 4; index++) {
					if ((q[pos][index] != 0) && (q[pos][index] > bestVal)) {
						bestVal = q[pos][index];
						bestMove = index;
					}
				}
				cout << actionPrint(bestMove) << "\t";	
			}
			else
				cout << "X\t";
						
		}
		cout << endl;
	}
	int i=0;
	cout << q[i][0] << " " << q[i][1] << " " << q[i][2] << " " << q[i][3] << endl;
}

int main() {
	totalValidCells = 0;
	// Start reading input
	std::cin >> nLines >> nColumns >> defaultValue;
	q.resize(nLines*nColumns);
	r.resize(nLines*nColumns);
	for (int i=0; i < nLines*nColumns; i++) {
		isFinal.push_back(false);
		visited.push_back(false);
	}
	reward.resize(nLines*nColumns);
	string trash;
	for (int i=0; i < nLines * nColumns; i++) {
		cin >> trash;
	}
	string content;
	for (int i=0; i < nLines * nColumns; i++) {
		cin >> content;

		if (content == "X") {
			//cout << q[i].size();
		}
		else if ((content == "D") || (content == ".")) {
			totalValidCells++;
			reward[i] = defaultValue;
			vector< float > qElement, rElement;
			//cout << q[i].size();
			q[i].resize(4);
			r[i].resize(4);
			for (int ind = 0; ind < 4; ind++) {
				q[i][ind] = 0;
				r[i][ind] = 0;
			}
		}
		else {
			totalValidCells++;
			double tmp = atof(content.c_str());
			reward[i] = (float) tmp;
			std::vector< float > qElement, rElement;
			for (int ind = 0; ind < 4; ind++) {
				qElement.push_back(0);
				rElement.push_back(0);
			}
			isFinal[i] = true;
			q[i] = qElement;
			r[i] = rElement;
		}
	}

	// end reading input

	//start initing vars
		initVars();
	// end of initing vars
	int initState = 0;
	int bestFound = INF;
	for (int i=0; i < 200; i++) {
		int totSteps = performEpisode(initState);
		if (totSteps < bestFound)
			bestFound = totSteps;
		cout << "Episode " << i+1 << " finished with " << totSteps << " steps." << endl;
		printQ();
	}
	cout << "best: " << bestFound << endl;

	return 0;
}
