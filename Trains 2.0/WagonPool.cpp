#include "WagonPoolHeader.h"
/*
checks if wagon is in pool
*/
bool WagonPool::hasWagon(WagonType wt)
{ 
	for(auto it = pool.begin(); it != pool.end(); it++){
		if(wt==it->get()->getType())
			return true;
	}
	return false;
}

/*
returns wagon, of specified type
*/
Wagons* WagonPool::getWagon(WagonType wt)
{
	Wagons *tmpWagon;
	for(auto it = pool.begin(); it!= pool.end(); it++){
		if(wt == it->get()->getType())
		{
			tmpWagon = it->release();
			pool.erase(it);
			return tmpWagon;

		}
	}
	return nullptr;
}

/*
adds a wagon to the pool
*/
void WagonPool::releaseWagon(Wagons *w)
{
	pool.push_back(unique_ptr<Wagons>(w));
}
/*
returns string of info about wagons in pool
*/
string WagonPool::getInfo()
{
	string info = "";
	for(auto it = pool.begin(); it!= pool.end(); it++)
	{
		info += it->get()->getInfo() + "\n";
	}
	if(info == "")
		return "No wagons available";
	else
		return info;
}