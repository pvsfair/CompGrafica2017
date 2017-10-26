#pragma once
#include <iostream>
#include <cmath>
class Math
{
private:
	Math();
	~Math();
public:
	static int distanceBtwPoints(std::pair<int, int> p1, std::pair<int, int> p2) {

		return (int)round(sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2)));
	}
};

