#ifndef RAILWAYHEADER_H
#define RAILWAYHEADER_H

#include <vector>
#include <memory>
#include "WagonHeader.h"
#include "TrainHeader.h"
#include "StationHeader.h"
#include "DataFileReader.hpp"
#include "SimulationHeader.h"
#include <exception>

using std::vector;
using std::shared_ptr;

/*
default file names
*/
const string STATIONSFILE = "stations.txt", TRAINFILE = "trains.txt", ERRORFILE = "errors.txt";


class MockTrain;
class Railway
{
private:
	vector<shared_ptr<Train>> trains;
	vector<shared_ptr<Stations>> stations;
	shared_ptr<Simulation> sim;

	void setup();
	void setupSim();
	void setupSim(int, int, int);
	
	const string stationsFile, trainFile, errorFile;


public:
	Railway(shared_ptr<Simulation>);
	Railway(string stationsFile, string TrainFile, string errorFile, shared_ptr<Simulation>);
	shared_ptr<Stations> findStation(string);
	shared_ptr<Train> findTrain(int);
	void printTrains();
	void printStations();

};

std::istream &operator>>(std::istream &, MockTrain &);
#endif