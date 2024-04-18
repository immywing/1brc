#pragma once

#ifndef WSTATION_DATA_H
#define WSTATION_DATA_H
#include <string>
#include <algorithm>
struct WStationData 
{
	int min = 100;
	int max = -100;
	int sum = 0;
	long count = 0;
	void update(int& value);
	void update(WStationData& other);
};

#endif


