#include "StationHeader.h"
#include "WagonHeader.h"
#include <algorithm>
#include <sstream>



Stations::Stations(string name): stationName(name)
{}
/*
	this method gets the train, and wagon type as a parameter
	if the wagon is inserted, the requestWagon will call the trains
	popRequestQueue and return true
*/
bool Stations::requestWagon(std::shared_ptr<Train> train, WagonType wt)
{
	switch (wt)
	{
	case Electric:
		{
			if(wagons.hasWagon(Electric)&& train->getWagons().front() == Electric){
				ElectricMotorWagon *emw = dynamic_cast<ElectricMotorWagon *> (wagons.getWagon(Electric));
				if(train->addWagon(emw)){
					train->popWagonRequestQueue();
					return true;
				}
			}
			return false;
		}
	
	case Diesel:
		{
			if(wagons.hasWagon(Diesel)&& train->getWagons().front() == Diesel){
				DieselMotorWagon *dmw = dynamic_cast<DieselMotorWagon *> (wagons.getWagon(Diesel));
				if(train->addWagon(dmw))
				{
					train->popWagonRequestQueue();
					return true;
				}
			}
			return false;
		}

	case Sitting:
		{
			if(wagons.hasWagon(Sitting)&& train->getWagons().front() == Sitting){
				SittingWagon *sw = dynamic_cast<SittingWagon *> (wagons.getWagon(Sitting));
				if(train->addWagon(sw))
				{
					train->popWagonRequestQueue();
					return true;

				}
			}
			return false;
		}

	case Sleeping:
		{
			if(wagons.hasWagon(Sleeping)&& train->getWagons().front() == Sleeping){
				SleepWagon *sw = dynamic_cast<SleepWagon *> (wagons.getWagon(Sleeping));
				if(train->addWagon(sw))
				{
					train->popWagonRequestQueue();
					return true;
				}
			}
			return false;
		}

	case Open:
		{
			if(wagons.hasWagon(Open)&& train->getWagons().front() == Open){
				OpenFreightWagon *ofw = dynamic_cast<OpenFreightWagon *> (wagons.getWagon(Open));
				if(train->addWagon(ofw))
				{
					train->popWagonRequestQueue();
					return true;
				}
			}
			return false;
		}

	case Closed:
		{
			if(wagons.hasWagon(Closed)&& train->getWagons().front() == Closed){
				ClosedFreightWagon *cfw = dynamic_cast<ClosedFreightWagon *> (wagons.getWagon(Closed));
				if(train->addWagon(cfw))
				{
					train->popWagonRequestQueue();
					return true;
				}
			}
			return false;
		}
	default:
		{
			return false;
		}
	}
	
}
/*
adds wagon to station
*/
void Stations::addWagon(Wagons* wagon)
{
	wagons.releaseWagon(wagon);

}
/*
adds train to station
*/
void Stations::addTrain(shared_ptr<Train> train)
{
	trains.push_back(train);
}
/*
removes train from station
*/
void Stations::removeTrain(shared_ptr<Train> train)
{
	trains.remove(train);
}
/*
prints info on wagons at station
*/
void Stations::printWagons()
{
	cout << wagons.getInfo() << endl;
}
/*
prints info on trains at this station
*/
void Stations::printTrains() const
{
	if(!(trains.empty())){
		for(auto it = trains.begin(); it!= trains.end(); it++)
		{
			it->get()->getInfo();
		}
	}
	else
		cout << "No trains in this station" << endl;
}
/*
sets station name
*/
void Stations::setName(string name)
{
	stationName = name;
}
/*
returns station name
*/
string Stations::getName() const
{
	return stationName;
}
/*
overloaded operator >> used when stations, and wagons are read from file
*/
std::istream &operator>>(std::istream &is, std::shared_ptr<Stations> &station)
{
	vector<int> params;
	std::stringstream ss, wagonstream;
	string name, line;
	std::getline(is, line);
	ss << line;
	std::getline(ss, name, '|');
	//station.reset( /*std::make_shared<Stations>*/ (Stations(name));
	try{
		station.reset(new Stations(name));
	}catch(std::bad_alloc &ba)
	{
		throw ba;
		return is;
	}

	string wagon;
	std::getline(ss, wagon, '|');
	
	while(!wagon.empty())
	{
		try{
			wagonstream.clear();
			int type;
			wagonstream << wagon;
			wagonstream >> type;
			bool check = true;
			switch(type)
			{
			case Sitting:
			
				int internet;
				int seats;
				wagonstream >> internet;
				wagonstream >> seats;
				check = (seats >= 0);
				if(check){
					station->addWagon(new SittingWagon(internet == 1, seats));
				}
				break;
			
			case Sleeping:
			
				int beds;
				wagonstream >> beds;
				check = (beds >= 0);
				if(check)
				{
					station->addWagon(new SleepWagon(beds));
				}
				break;
				
			case Open:
			{	
				int sqMeters;
				int capacity;
				wagonstream >> sqMeters;
				wagonstream >> capacity;
				check = (sqMeters >= 0);
				check = (capacity >= 0);
				if(check)
				{
					station->addWagon(new OpenFreightWagon(sqMeters, capacity));
				}
				break;
			}
			case Closed:
			{
				int capacity;
				wagonstream >> capacity;
				check = capacity >= 0;
				if(check)
				{
					station->addWagon(new ClosedFreightWagon(capacity));
				}
				break;
			}
			case Electric:
			{
				int effect;
				int maxSpeed;
				wagonstream >> effect;
				wagonstream >> maxSpeed;
				check = (effect >= 0);
				check = (maxSpeed >= 0);
				if(check)
				{
					station->addWagon(new ElectricMotorWagon(effect, maxSpeed));
				}

				break;
			}
			case Diesel:
			{
				int use;
				int maxSpeed;
				wagonstream >> use;
				wagonstream >> maxSpeed;
				check = (use >= 0);
				check = (use >= 0);
				if(check)
				{
					station->addWagon(new DieselMotorWagon(use, maxSpeed));
				}
				break;
			}
			default:
				break;
				
			}
		
			std::getline(ss, wagon, '|');
		}
		catch(std::bad_alloc &ba)
		{
			throw ba;

		}
	}

	return is;
}