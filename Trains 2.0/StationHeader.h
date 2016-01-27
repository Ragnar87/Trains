#ifndef STATIONHEADER_H
#define STATIONHEADER_H
/*
header file for station
*/
#include "TrainHeader.h"
#include "WagonPoolHeader.h"
#include <istream>
//forward decleration of Train
class Train;

class Stations
{
public:
	Stations(string);
	bool requestWagon(std::shared_ptr<Train>, WagonType );
	void printTrains() const;
	void printWagons();
	void addTrain(std::shared_ptr<Train>);
	void removeTrain(std::shared_ptr<Train>);
	void addWagon(Wagons *);
	void setName(string);
	string getName() const;
private:
	string stationName;
	WagonPool wagons;
	std::list<std::shared_ptr<Train>> trains;
};

std::istream &operator>>(std::istream &, std::shared_ptr<Stations> &); 
#endif