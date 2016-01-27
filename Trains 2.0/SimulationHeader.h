/*
Simulation class

*/

#ifndef SIMULATIONHEADER_H
#define SIMULATIONHEADER_H

#include "EventHeader.h"
#include "TimeConverterHeader.h"
#include <queue>
#include <vector>


using std::vector;
using std::priority_queue;


class Simulation
{
public:
	Simulation(int, int, int);
	bool incrementTime();
	bool processNext();
	void registerEvent(shared_ptr<Event> newEvent);
	void run();
	int getCurrentTime() const {return currentTime;}
	int getEndTime() const {return endTime;}
	bool changeIncrement(int);
	vector<std::pair<shared_ptr<Train>, int>> getDelays() const {return delays;}
	void addDelay(shared_ptr<Train>, int);

private:
	int currentTime;
	int endTime;
	int increment;
	priority_queue<shared_ptr<Event>, vector<shared_ptr<Event>>, EventComparison> events;
	vector<std::pair<shared_ptr<Train>, int>> delays;


};
#endif