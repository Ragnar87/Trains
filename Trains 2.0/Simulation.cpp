#include "SimulationHeader.h"
#include <algorithm>
#include <iterator>



Simulation::Simulation(int start, int end, int sIncrement): currentTime(start), endTime(end), increment(sIncrement)
{}
/*
increments simulation time
and processes events occurring before currentTime
*/
bool Simulation::incrementTime()
{
	currentTime += increment;

	while(!events.empty() && events.top()->getTime() <= currentTime )
	{
		cout << "Time " << TimeConverter::formatToString(events.top()->getTime()) << " >> ";
		events.top()->processEvent();
		
		events.pop();
	}
	cout << "Current time is: " << TimeConverter::formatToString(currentTime) << endl;

	return (!events.empty());
}

void Simulation::run()
{
	while(currentTime < endTime)
	{
		incrementTime();
	}
}

/*
registers event to happen
*/
void Simulation::registerEvent(shared_ptr<Event> newEvent)
{
	shared_ptr<Event> e(newEvent);
	events.push(e);
}

/*
changes the amount the time is incremented by
*/
bool Simulation::changeIncrement(int inc)
{
	if(inc > 0)
	{
		increment = inc;
		return true;
	}

	return false;
}
/*
processes next event
*/
bool Simulation::processNext()
{
	if(currentTime < getEndTime())
	{
		currentTime = events.top()->getTime();
		cout << "Time " << TimeConverter::formatToString(events.top()->getTime()) << " >> ";
		events.top()->processEvent();
		events.pop();
		return (!events.empty());
	}
	return false;
}
/*
if train is delayed it is added to vector
*/
void Simulation::addDelay(shared_ptr<Train> train, int delay)
{
	delays.push_back(std::make_pair(train, delay));
}