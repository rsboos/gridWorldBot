#include "maze.h"


void maze::readInput() {
	std::cin >> nLines >> nColumns >> defaultValue;
	q.resize(nLines*nColumns);
	r.resize(nLines*nColumns);
	string trash;
	for (int i=0; i < nLines; i++) {
		for (int j=0; j < nColumns; j++) {
			cin >> trash;
		}
	}
	vector< vector<string contentPos;
	for (int i=0; i < nLines; i++) {
		for (int j=0; j < nColumns; j++) {
			std::cin >> contentPos;
		}
	}

}
