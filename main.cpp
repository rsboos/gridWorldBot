#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>

using namespace std;


#define GAMMA 0.9 // taxa de desconto
#define ALPHA 0.1
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define INF 9999999999

std::vector< std::vector<float> > q;
std::vector< std::vector<float> > r;
std::vector< float > reward;
vector<bool> isFinal;


int nColumns,nLines;
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
				return NULL;
				break;
		}	
	}
	float calculateQ(int pos, int action) {
		float ret = 0;
		ret = (1-ALPHA) * r[pos][action];
		int nextState = getNeighbour(pos,action);
		float maxFactor = -INF;
		float val;
		//cout << "Current Position: " << pos << " Next position: " << nextState << " by Action: " << actionPrint(action) << endl;
		for (int i = 0; i < 4; i++) {
			val = q[nextState][i];
			if (val > maxFactor)
				maxFactor = val;
		}
		ret += ALPHA*GAMMA*maxFactor;
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

	bool hasEquals(vector<float> &v,float bestValue, vector<int> &ret) {
		for (int i=0; i < v.size(); i++)
			if (v[i] == bestValue)
				ret.push_back(i);
		if (ret.size() > 1)
			return true;
		else
			return false;
	}

	void performEpisode(int pos) {	
		bool isFinalState = false;
		vector<float> valuesTmp;
		vector<float> values;
		values.resize(4);
		valuesTmp.resize(4);
		int curPos = pos;
		do {
			for (int i=0; i < valuesTmp.size(); i++)
				valuesTmp[i] = calculateQ(curPos,i);	
			for (int i=0; i < values.size(); i++) {
				values[i] = normalizeQ(i,valuesTmp);
				//cout << values[i] << endl;
			}
			srand (time(NULL));
			float bestValue = *max_element(values.begin(),values.end());
			int bestMove;
			vector<int> bestPositions; 
			if (hasEquals(values,bestValue,bestPositions)) {
				int randVal = rand() % bestPositions.size();
				bestMove = bestPositions[randVal];
			}
			else {
				size_t tmp = max_element(values.begin(),values.end()) - values.begin();
				bestMove = static_cast<int>(tmp);
			}
			if (count (values.begin(), values.end(), bestValue) == 4) {
				bestMove = rand() % 4;
			}
			else {
				
			}
			q[pos][bestMove] = bestValue;
		//int bestMove = find(values.begin(),values.end(),bestValue) - values.begin(); // UP, DOWN, LEFT, RIGHT
		//cout << bestMove << endl;
		//cout << bestValue << endl;
		
		int nextPos = getNeighbour(curPos,bestMove);
		if (curPos != nextPos) {
			cout << "Current position: " << curPos << " Next one: " << nextPos << endl;
		}
		//cout << "Current position: " << curPos << " Next one: " << nextPos << " Move made: " << actionPrint(bestMove) << endl;
		if (isFinal[nextPos])
			isFinalState=true;
		curPos = nextPos;
	
	} while (!isFinalState);

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

int main() {
	// Start reading input
	std::cin >> nLines >> nColumns >> defaultValue;
	q.resize(nLines*nColumns);
	r.resize(nLines*nColumns);
	for (int i=0; i < nLines*nColumns; i++) {
		isFinal.push_back(false);
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
		else if (content == "D") {
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
	performEpisode(initState);

	return 0;
}
