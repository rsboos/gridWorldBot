#include "position.h"


int main() {
	int l,c;
	std::pair <int,int> posCompare;
	position p;
	p.currentValue = 5;
	p.testing();
	posCompare = std::make_pair(1,1);
	std::cout << p.isNeighbour(posCompare);
	std::vector< std::vector<int> > values; 
	std::cin >> l >> c;

	return 0;
}