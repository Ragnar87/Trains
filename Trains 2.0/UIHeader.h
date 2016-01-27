/*
User interface header file

*/

#ifndef UIHEADER_H
#define UIHEADER_H

#include <iostream>
#include "SimulationHeader.h"
#include "RailwayHeader.h"
#include <exception>

//default simulation values
const int DEFSIMSTART = 0, DEFSIMEND = 1439, DEFINCREMENT = 10;

using std::cout;
using std::endl;
using std::cin;
using std::ios;

class UI
{
private:
	

	void startMenu();
	bool startDefaultSim();
	bool specifySim();

	std::ofstream errorOutput;

	void simMenu();
	void incrementTime();
	void nextEvent();

	char getMenuOption();

	bool changeIncrement();

	void printStations();
	void printTrain();

	int userInput();

	void stationMenu(shared_ptr<Stations>);
	void trainMenu(shared_ptr<Train>);

	unique_ptr<Railway> railway;
	shared_ptr <Simulation> sim;

public:
	void run();
};

#endif