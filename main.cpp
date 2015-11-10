#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>

using namespace std;


#define GAMMA 0.8 // taxa de desconto
#define ALPHA 0.1
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define INF 9999999999

std::vector< std::vector<float> > q;
std::vector< std::vector<float> > r;
std::vector< std::vector<int> > adjacencyList;
std::vector< float > reward;
vector<bool> isFinal;


int nColumns,nLines;

float calculateQ(int pos, int action) {
	float ret = 0;
	ret = (1-ALPHA) * r[pos][action];
	int nextState = adjacencyList[pos][action];
	float maxFactor = -INF;
	float val;
	for (int i = 0; i < 4; i++) {
		val = q[nextState][i];
		if (val > maxFactor)
			maxFactor = val;
	}
	ret += ALPHA*GAMMA*maxFactor;
	return ret;
}



bool isWall(int pos) {
	if (q[pos].size() == 0)
		return true;
	else
		return false;
}

int getIndex(int line, int column) {
	return nColumns * line + column;
}


void performEpisode(int pos) {	
	bool isFinalState = false;
	vector<float> values;
	values.resize(4);
	do {
		for (int i=0; i < values.size(); i++)
			values[i] = calculateQ(pos,i);
		float bestValue = *max_element(values.begin(),values.end());
		int bestMove = find(values.begin(),values.end(),bestValue) - values.begin(); // UP, DOWN, LEFT, RIGHT
		int nextPos;
		nextPos = adjacencyList[pos][bestMove];
		if (isFinal[nextPos])
			isFinalState=true;

	} while (!isFinalState);

}

int getNeighbour(int pos, int action) {
	int line = floor(nColumns / pos);
	int column = pos - (line * nColumns);
	int newPos;
	if (action == UP) {
		newPos = getIndex(line+1,column);
		if ((line == nLines - 1) || (adjacencyList[newPos].size() == 0)) // wall or out of maze
			return pos;
		else
			return newPos;
	}
	else if (action == DOWN) {
		newPos = getIndex(line-1,column);
			if ((line == 0) || (adjacencyList[newPos].size() == 0)) // wall or out of maze
				return pos;
			else
				return newPos;
		}
		else if (action == LEFT) {
			newPos = getIndex(line,column-1);
			if ((column == 0) || (adjacencyList[newPos].size() == 0)) // wall or out of maze
				return pos;
			else
				return newPos;
		}
		else if (action == RIGHT) {
			newPos = getIndex(line,column+1);
			if ((column == nColumns-1) || (adjacencyList[newPos].size() == 0)) // wall or out of maze
				return pos;
			else
				return newPos;

		}

	}

	int main() {
		float defaultValue;
	// Start reading input
		std::cin >> nLines >> nColumns >> defaultValue;
		q.resize(nLines*nColumns);
		r.resize(nLines*nColumns);
		for (int i=0; i < nLines*nColumns; i++) {
			isFinal.push_back(false);
		}
		adjacencyList.resize(nLines*nColumns);
		reward.resize(nLines*nColumns);
		string trash;
		for (int i=0; i < nLines; i++) {
			for (int j=0; j < nColumns; j++) {
				cin >> trash;
			}
		}
		std::vector <string> contentPos;
		contentPos.resize(nLines*nColumns);
		for (int i=0; i < nLines; i++) {
			for (int j=0; j < nColumns; j++) {
				int pos = getIndex(i,j);
				std::cin >> contentPos[pos];

			}
		}
	// end reading input

	//start initing vars
		for (int i=0; i < nLines*nColumns; i++) {
			vector<int> qElem;
			if (contentPos[i] == "X")
				adjacencyList[i] = qElem;
		}
		for (int i=0; i < nLines*nColumns; i++) {
			if (contentPos[i] != "X")
				for (int j=0; j < 4; j++)
					adjacencyList[i].push_back(getNeighbour(i,j));
			}
			


			for (int i=0; i < nLines*nColumns; i++) {
				if (contentPos[i] == "X") {
					std::vector< float > qElement, rElement;
					q[i] = qElement;
					r[i] = rElement;
				}
				else if (contentPos[i] == "D") {
					reward[i] = defaultValue;
					std::vector< float > qElement, rElement;
					for (int ind = 0; i < 4; i++) {
						qElement.push_back(0);
						rElement.push_back(0);
					}
					q[i] = qElement;
					r[i] = rElement;
				}
				else {
					double tmp = atof(contentPos[i].c_str());
					reward[i] = (float) tmp;
					std::vector< float > qElement, rElement;
					for (int ind = 0; i < 4; i++) {
						qElement.push_back(0);
						rElement.push_back(0);
					}
					isFinal[i] = true;
					q[i] = qElement;
					r[i] = rElement;
				}
			}
	// init r(state,action)
			for (int i=0; i < nLines*nColumns; i++) {
				if (!isWall(i)) {
					int upNode = adjacencyList[i][UP];
					int downNode = adjacencyList[i][DOWN];
					int leftNode = adjacencyList[i][LEFT];
					int rightNode = adjacencyList[i][RIGHT];
					r[i][UP] = (0.8*reward[upNode]) + (0.1 * reward[leftNode]) + (0.1 * reward[rightNode]);
					r[i][DOWN] = (0.8*reward[downNode]) + (0.1 * reward[leftNode]) + (0.1 * reward[rightNode]);
					r[i][LEFT] = (0.8*reward[leftNode]) + (0.1 * reward[upNode]) + (0.1 * reward[downNode]);
					r[i][RIGHT] = (0.8*reward[rightNode]) + (0.1 * reward[upNode]) + (0.1 * reward[downNode]);
				}
			}
	// end of initing vars
			int initState = 0;
			performEpisode(initState);

			return 0;
		}
