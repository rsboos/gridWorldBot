#ifndef POS_H
#define POS_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>


class position {
	public:
		float currentValue;
		std::pair<int,int> coordinates;
		std::vector<position> neighbours;
		void testing();
		void getNextPosition(char movement);
		bool isNeighbour(std::pair <int,int> posCompare);
};

#endif