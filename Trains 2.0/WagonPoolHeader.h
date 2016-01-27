/*
wagon pool header
*/

#ifndef WAGONPOOLHEADER_H
#define WAGONPOOLHEADER_H

#include <memory>
#include <typeinfo>
#include <list>
#include "WagonHeader.h"

using std::unique_ptr;

class WagonPool
{
private:
	std::list<unique_ptr<Wagons>> pool;
public:
	bool hasWagon(WagonType);
	Wagons* getWagon(WagonType);
	void releaseWagon(Wagons *w);
	string getInfo();

};

#endif