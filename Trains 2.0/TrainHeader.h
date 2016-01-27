
/*
Train header file
*/

#ifndef TRAIN_H
#define TRAIN_H

#include "WagonHeader.h"
#include "StationHeader.h"
#include <list>
#include <memory>
#include <string>
#include <ostream>
#include <vector>
#include <iostream>
#include <queue>

using std::queue;
using std::list;
using std::unique_ptr;
using std::string;
using std::ostream;
using std::shared_ptr;
using std::vector;
using std::cout;
using std::endl;

//types
enum State {NOT_ASSEMBLED, INCOMPLETE, ASSEMBLED, READY, RUNNING, ARRIVED, FINISHED};
//used to print types
static const char * TrainStateStrings[] = {"NOT_ASSEMBLED", "INCOMPLETE", "ASSEMBLED", "READY", "RUNNING", "ARRIVED", "FINISHED"};

class Stations;

class Train
{
public:
	Train(std::shared_ptr<Stations>, std::shared_ptr<Stations>, int, int, int);
	int getId() const;
	int getTravelTime() const;
	State getState() const;
	void incrementState(bool);
	string getTypes() const;
	vector<shared_ptr<Wagons>> getTrain() const;
	int getWagonId() const;
	bool addWagon(Wagons *);
	Wagons* releaseWagon();
	void getInfo() const;

	std::shared_ptr<Stations> getOrigin() const;
	std::shared_ptr<Stations> getDestination() const;
	int getDepartureTime()const;

	bool wagonToRequest(WagonType);
	void wagonTypeQueue(queue<WagonType>);
	queue<WagonType> getWagons() const;
	void popWagonRequestQueue();

private:

	vector<unique_ptr<Wagons>> train;
	int id;
	State state;
	

	std::shared_ptr<Stations> origin;
	std::shared_ptr<Stations> destination;

	int travelTime;
	int departureTime;
	queue<WagonType> wagons;

	
};

#endif