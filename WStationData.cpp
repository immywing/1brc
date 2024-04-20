#include "WStationData.h"

void WStationData::update(int& value)
{
	min = min < value ? min : value;
	max = max > value ? max : value;
	sum += value;
	count++;
}

void WStationData::aggregate(WStationData& other)
{
	min = min < other.min ? min : other.min;
	max = max > other.max ? max : other.max;
	sum += other.sum;
	count += other.count;
}