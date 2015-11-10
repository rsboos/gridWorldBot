#ifndef POS_H
#define POS_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>


class maze {
	private:
		float gamma 0.8; // taxa de desconto
		float alpha 0.1;
		std::pair<int,int> coordinates;
	public:
		void readInput();
};

#endif