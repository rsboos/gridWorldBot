#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <fstream>
using namespace std;


#define GAMMA 0.9 // taxa de desconto
#define ALPHA 0.1
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
vector<int> directions;
vector<int> choosenAction;


int nColumns,nLines, totalValidCells;
float defaultValue;

/* auxiliar Function */
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
				return "↑";
				break;
			case 1:
				return "↓";
				break;
			case 2:
				return "←";
				break;
			case 3:
				return "→";
				break;
			default:
				return "N";
				break;
		}	
	}
	int makeMove(int move) {
		int randNum = rand() % 10;
		
		if (move == UP || move == DOWN) {
			if (randNum <= 7)
				return move;
			else if (randNum == 8)
				return LEFT;
			else
				return RIGHT;
		}
		else if (move == LEFT || move == RIGHT) {
			if (randNum <= 7)
				return move;
			else if (randNum == 8)
				return UP;
			else
				return DOWN;
		}
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

/* end auxiliar Functions */

	float calculateQ(int pos, int action) {
		float ret = 0;
		//ret = q[pos][action];
		ret = (1-ALPHA) * q[pos][action];
		int nextState = getNeighbour(pos,action);
		float maxFactor = -INF;
		float val;

		for (int i = 0; i < 4; i++) {
			val = q[nextState][i];
			if (val > maxFactor)
				maxFactor = val;
		}
		ret += ALPHA * (r[pos][action] + (GAMMA*maxFactor));
		return ret;
	}

	float normalizeQ (int index, vector<float> &v) {
		//return v[index];
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

	

	bool performRandomAction(int episodeNumber) {

		int probConst = 50;
		int eta = (count(visited.begin(),visited.end(),true) * 100) / totalValidCells; // percentage of visited
		eta = eta / 4; // amortize constant
		int constTime = (episodeNumber*100) / (totalValidCells*2); // first n*2 should explore episodes adds up
		//if (constTime >= 100)
		//	return false;
		constTime = constTime / 2;
		constTime = min(50,constTime);
		int probRes = probConst - constTime + 1;

		int randNum = rand() % 100;
		/*if (episodeNumber >= 49) {
			cout << "probRand " << randNum << " "; 
		}*/
		if (randNum < probRes)
			return true;
		else
			return false;
	}

	void selectActions(int pos, vector<int> &v) {
		vector<int> retSub;
		for (int i=0; i < 4; i++)
			if (!(getNeighbour(pos,i) == pos)) { // not going to the same place
				retSub.push_back(i);
					//if (!visited[getNeighbour(pos,i)]) // better if not visited
						//v.push_back(i);
			}
		if (v.size() == 0) {
			v.clear();
			for (int i=0; i < retSub.size(); i++) {
				v.push_back(retSub[i]);
			}
		}
	}

	vector<int> performEpisode(int pos, int curEpisode) {
		int changed = 0;
		vector<int> ret;
		// calculate numfber of vis
		for (int i=0; i < nLines*nColumns; i++) {
			if (isFinal[i] && (reward[i] < 0)) 
				visited[i] = true;
			else
				visited[i]= false;
		}

		int totalSteps = 0;
		bool isFinalState = false;
		vector<float> valuesTmp;  
		vector<float> values; 
		values.resize(4);
		valuesTmp.resize(4); 
		int curPos = pos;
		do {
			visited[curPos] == true;
			int bestMove;
			float bestValue;
			
			bool chooseRand = performRandomAction(curEpisode);
			if (chooseRand) {
				
				for (int i=0; i < valuesTmp.size(); i++)
					valuesTmp[i] = calculateQ(curPos,i);
				for (int i=0; i < values.size(); i++)
					values[i] = normalizeQ(i,valuesTmp);
				vector<int> actions;
				selectActions(curPos,actions);
				
				if (actions.size() == 0) {
					bestMove = rand() % 4;
				}
				else {
					int bestMoveIndex = rand() % actions.size();
					bestMove = actions[bestMoveIndex];					
				}
				bestValue = values[bestMove];

					//cout << "length opt: " << actions.size() << " best index: " << bestMove << " training: " << curEpisode << " ";
				
			}
			else {


				for (int i=0; i < valuesTmp.size(); i++) {
					valuesTmp[i] = calculateQ(curPos,i); 

				}
				for (int i=0; i < values.size(); i++) {
					values[i] = normalizeQ(i,valuesTmp); 
					//cout << values[i] << endl;
				}
				bestValue = *max_element(values.begin(),values.end());
				size_t tmp = max_element(values.begin(),values.end()) - values.begin();
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
				else 
					bestMove = static_cast<int>(tmp);
			}
			if (choosenAction[curPos] != bestMove)
				changed=1;
			choosenAction[curPos] = bestMove;
			int bestMoveFixed = makeMove(bestMove);
			
			q[curPos][bestMoveFixed] = bestValue;
		int nextPos = getNeighbour(curPos,bestMoveFixed);
		
		
		if (isFinal[nextPos])
			isFinalState=true;
		curPos = nextPos;
		totalSteps++;
	} while (!isFinalState);
	ret.push_back(totalSteps);
	ret.push_back(changed);
	ret.push_back(curPos);
	return ret;
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
	ofstream fMap ("map.txt");
	for (int i=0; i < nLines; i ++) {
		for (int j=0; j < nColumns; j++) {
			int pos = getIndex(i,j);
			int bestMove = -1;
			if (!isWall(pos)) {
				bool onlyZeros = true;
				for (int index = 0; index < 4; index++)
					if (q[pos][index] != 0)
						onlyZeros = false;
				if (isFinal[pos]) {
					if (reward[pos] > 0)
						fMap << "\u25CE ";
					else
						fMap << "\u25C9 ";
				}
				else if (onlyZeros) {
					fMap << "N "; // NOT VISITED
				}
				else {
				fMap << actionPrint(choosenAction[pos]) << " ";
				/*float bestVal = -INF;
				for (int index = 0; index < 4; index++) {
					if ((q[pos][index] != 0) && (q[pos][index] > bestVal)) {
						bestVal = q[pos][index];
						bestMove = index;
					}
				}
				cout << actionPrint(bestMove) << "\t"; */
				}
			}
			else
				fMap << "\u25A0 ";
						
		}
		fMap << endl;
	}
	fMap.close();
	//int i=(nLines-1) * nColumns;
	//cout << q[i][0] << " " << q[i][1] << " " << q[i][2] << " " << q[i][3] << endl;
}

int main() {
	srand((unsigned) time(NULL));
	totalValidCells = 0;
	string nLinesStr, nColumnsStr, defaultValueStr;
	// Start reading input
	std::cin >> nLinesStr >> nColumnsStr >> defaultValueStr;
	nLinesStr.erase (0,2);
	nColumnsStr.erase (0,2);
	defaultValueStr.erase (0,2);
	nLines = atoi(nLinesStr.c_str());
	nColumns = atoi(nColumnsStr.c_str());
	defaultValue = atof(defaultValueStr.c_str());
	cout << defaultValue;
	q.resize(nLines*nColumns);
	r.resize(nLines*nColumns);
	choosenAction.resize(nLines*nColumns);
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
	int initState;
	//initState = 0;
	initState = (nLines-1) * nColumns;
	int bestFound = INF;
	int epBest;
	float bestReward = *max_element(reward.begin(),reward.end());
	int countNoChange = 0;
	int totalEpisodes = 2000;
	ofstream myfile ("graphic.txt");
	for (int i=0; i < 2000; i++) {
		vector<int> ret;
		ret = performEpisode(initState,i+1);

		if (ret[0] < bestFound && reward[ret[2]] > 0) {
			bestFound = ret[0];
			epBest = i+1;
		}
		if (ret[1] == 0) // if changed false
			countNoChange++;
		else
			countNoChange = 0;
		
  if (myfile.is_open())
  {
    myfile << i+1 << "\t" << ret[0] << endl;    
  }
		
		if (countNoChange == 10) {
			totalEpisodes = i+1;
			break;
		}
	}
	myfile.close();
	printQ();
	cout << "Best Path: " << bestFound << " steps. First episode: " << epBest << " Total number of episodes: " << totalEpisodes << endl;

	return 0;
}