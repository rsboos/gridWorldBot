#include "position.h"

std::vector< std::map <char,int> > q;
std::vector< std::map <char,int> > r;

#define GAMMA 0.8; // taxa de desconto
#define ALPHA 0.1;

float calculateQ(int state, char action) {
	float ret;
	ret = r[state][action];
	nextState = state + action
	float maxFactor = -INF;
	foreach move {
		float val = q[nextState][move];
		if (val > maxFactor)
			maxFactor = val;
	}
	ret += GAMMA*maxFactor;
	return ret;
}

bool isNull(std::map<char,int> m) {
	if (m.size() == 0)
		return true;
	else
		return false;
}

int getIndex(int line, int column, int nColumns) {
	return nColumns * line + column;
}

int main() {
	int nLines,nColumns,defaultValue;
	std::cin >> nLines >> nColumns >> defaultValue;
	q.resize(nLines*nColumns);
	r.resize(nLines*nColumns);
	for (int i=0; i < nLines; i++) {
		for (int j=0; j < nColumns; j++) {
			int pos = getIndex(i,j,nColumns);
			int contentPos;
			cin >> contentPos;
			if (contentPos != 'D') {
				q[pos] = 
				r[pos] =

			}
			else if (isNumber(contentPos)) { // isFinal State
				q[pos] = 
				r[pos] =
				isFinal[pos] = true;
			}
		}
	}
	int initState = 0;
	bool isFinalState = false;
	do {
		// choose arbitrary action -> or choose best action (calculation Q(state,action) for all possible moves)
		curBest = qState,Action
		value = calculateQ(state,action);
		q[state][action] = value;
		curState = newState
		if (isFinal(curState))
			isFinalState = true;
	} while (!isFinalState);


	return 0;
}