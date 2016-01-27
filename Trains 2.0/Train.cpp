#include "TrainHeader.h"
#include <typeinfo>
#include <memory>

Train::Train(shared_ptr<Stations> origin, shared_ptr<Stations> destination, int departureTime, int travelTime, int id): /*Route::Route(origin, destination, departureTime, travelTime, id),*/ state(NOT_ASSEMBLED), 
	origin(origin), destination(destination), departureTime(departureTime), travelTime(travelTime), id(id)
{}


/*
returns train state
*/
State Train::getState() const
{
	return state;
}
/*
returns train id
*/
int Train::getId() const
{
	return id;
}

/*
increments state
parameter is bool
bool is only used in NotAssembled and Incomplete, to differentiate between
Assembled and incomplete

*/

void Train::incrementState(bool increment)
{
	switch (state)
	{
	case NOT_ASSEMBLED:
		{
			if(increment)
				state = ASSEMBLED;
			else
				state = INCOMPLETE;

			break;
		}
	case INCOMPLETE:
		{
			if(increment)
				state = ASSEMBLED;
			break;
		}
	case ASSEMBLED:
		{
			state = READY;
			break;
		}
	case READY:
		{
			state = RUNNING;
			break;
		}
	case RUNNING:
		{
			state = ARRIVED;
			break;
		}
	case ARRIVED:
		{
			state = FINISHED;
			break;
		}
	}
}
/*
adds wagon to train
*/
bool Train::addWagon(Wagons *wagon)
{

	//cout << std::to_string(wagon->getType()) << endl;
	if(train.empty())
	{
		if((wagon->getType() == Electric) || (wagon->getType()== Diesel))
		{
			train.push_back(unique_ptr<Wagons>(wagon));
			return true;
		}
		else
			return false;
	}
	train.push_back(unique_ptr<Wagons>(wagon));
	return true;
	
}
/*
releases top wagon
*/
Wagons*  Train::releaseWagon()
{
	if(!train.empty())
	{
		Wagons* tmpWagon = train.back().release();
		train.pop_back();
		return tmpWagon;
	}
	return nullptr;
}
/*
prints train info
*/
void Train::getInfo() const
{
	cout << "Train info: " << endl;
	cout << "Train id: " << getId() << " Train state: " << TrainStateStrings[getState()] 
	<< " Number of wagons: "<< train.size() << endl;
	for(auto it = train.begin(); it != train.end(); it++)
	{
		cout << it->get()->getInfo() << endl;
	}
}

/*
returns pointer to origin station
*/
shared_ptr<Stations> Train::getOrigin() const
{
	return origin;
}
/*
returns pointer to destination station
*/
shared_ptr<Stations> Train::getDestination() const
{
	return destination;
}
/*
returns departure time in seconds
*/
int Train::getDepartureTime() const
{
	return departureTime;
}
/*
returns travel time in seconds
*/
int Train::getTravelTime() const
{
	return travelTime;
}

/*
adds a wagontype to the request queue 
*/
bool Train::wagonToRequest(WagonType wt)
{
	if(wt >= 3 && wt <= 8){
		if(wagons.empty())
		{
			if(wt == Electric || wt == Diesel){
				wagons.push(wt);
				return true;
			}
			return false;
		}
		wagons.push(wt);
		return true;
	}
	return false;

}
/*
adds a whole queue to the request queue
*/
void Train::wagonTypeQueue(queue<WagonType> queue)
{
	while(!queue.empty())
	{
		wagonToRequest(queue.front());
		queue.pop();
	}

}
/*
returns wagon to request queue
*/
queue<WagonType> Train::getWagons() const
{
	return wagons;
}
/*
pops wagon from request queue
*/
void Train::popWagonRequestQueue()
{
	if(!(wagons.empty()))
	{
		wagons.pop();
	}
}