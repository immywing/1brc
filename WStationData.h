
#ifndef WSTATION_DATA_H
#define WSTATION_DATA_H
#include <string>
struct WStationData 
{
	int min = 1000;
	int max = -1000;
	long long sum = 0;
	long long count = 0;
	void update(int& value);
	void aggregate(WStationData& other);
};
#endif


