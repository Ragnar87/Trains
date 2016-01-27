#include "WagonHeader.h"
#include <cmath>
#include <random>
#include <ctime>
#include <sstream>

vector<int> Wagons::ids;


Wagons::Wagons(WagonType ty, int wid): type(ty), id(wid)
{}

/*
returns baseclass id
*/
int Wagons::getId() const
{
	return id;
}
/*

*/
WagonType Wagons::getType() const
{
	return type;
}
/*
virtual function used in all derived Wagon classes
*/
string Wagons::getInfo()
{
	return "Wagon ID: " + std::to_string(getId()) ;
}
/*
generates wagon id, and ensures it is distinct
*/
int Wagons::generateId()
{
	int id;
	auto it = ids.begin();
	std::default_random_engine re(std::time(0));
	std::uniform_int_distribution<int> intDist(1000, 2000);
	do
	{
		id = intDist(re);
		it = find(ids.begin(), ids.end(), id);
	}while(it != ids.end());

	ids.push_back(id);
	return id;

}


/**/
std::istream &operator>>(std::istream & is, std::shared_ptr<Wagons> &wagons)
{
	string wagon;
	std::stringstream ss;
	std::getline(is, wagon, '|');
	int type;
	ss << wagon;
	ss >> type;
	bool check = true;
	switch(type)
	{
	case Sitting:
		{
			int internet;
			int seats;
			ss >> internet;
			ss >> seats;
			check = (seats >= 0);
			if(check)
			{
				wagons = make_shared<SittingWagon>( SittingWagon(internet == 1, seats));
			}
			else
				wagons = nullptr;
			break;
		}
	case Sleeping:
		{
			int beds;
			ss >> beds;
			check = (beds >= 0);
			if(check)
			{
				wagons= make_shared<SleepWagon>( SleepWagon(beds));
			}
			else
				wagons = nullptr;
			break;
		}
	case Open:
		{
			int sqMeters;
			int capacity;
			ss >> sqMeters;
			ss >> capacity;
			check = (sqMeters >= 0);
			check = (capacity >= 0);
			if(check)
			{
				wagons=make_shared<OpenFreightWagon>( OpenFreightWagon(sqMeters, capacity));
			}
			else
				wagons = nullptr;
			break;
		}
	case Closed:
		{
			int capacity;
			ss >> capacity;
			check = capacity >= 0;
			if(check)
			{
				wagons = make_shared<ClosedFreightWagon>( ClosedFreightWagon(capacity));
			}
			else
				wagons = nullptr;
			break;
		}
	case Electric:
		{
			int effect;
			int maxSpeed;
			ss >> effect;
			ss >> maxSpeed;
			check = (effect >= 0);
			check = (maxSpeed >= 0);
			if(check)
			{
				wagons = make_shared<ElectricMotorWagon>( ElectricMotorWagon(effect, maxSpeed));
			}
			else
				wagons = nullptr;
			break;
		}
	case Diesel:
		{
			int use;
			int maxSpeed;
			ss >> use;
			ss >> maxSpeed;
			check = (use >= 0);
			check = (use >= 0);
			if(check)
			{
				wagons = make_shared<DieselMotorWagon>( DieselMotorWagon(use, maxSpeed));
			}
			else
				wagons = nullptr;
			break;
		}

	default:
		{
			wagons = nullptr;
			break;
		}
	}
	return is;

}

/*----------PERSON-WAGON-----------*/

PersonWagon::PersonWagon(WagonType type, int id): type(type), Wagons(Person, id)
{}
//returns type
WagonType PersonWagon::getType() const
{
	return type;
}
/*
returns string with wagon info
*/
string PersonWagon::getInfo()
{
	return Wagons::getInfo() + " Person Wagon type: " + WagonTypeStrings[getType()];
}

/*-------MOTOR-WAGON-------*/

MotorWagon::MotorWagon(WagonType t, int maxSpeed, int id): type(t), maxSpeed(maxSpeed), Wagons(Motor, id)
{}
//returns type
WagonType MotorWagon::getType() const
{
	return type;
}
//returns max speed
int MotorWagon::getMaxSpeed() const
{
	return maxSpeed;
}
//returns string with info
string MotorWagon::getInfo()
{
	return Wagons::getInfo() + " Motor wagon type: " + WagonTypeStrings[getType()] + " Max speed: " + std::to_string(getMaxSpeed());
}

/*------FREIGHT-WAGON--------*/

FreightWagon::FreightWagon(WagonType t, int id): type(t), Wagons(Freight, id)
{}

//returns type
WagonType FreightWagon::getType() const
{
	return type;
}
//returns string with info
string FreightWagon::getInfo()
{
	return Wagons::getInfo() + " Freight Wagon type: " + WagonTypeStrings[getType()];
}

/*------SLEEP-WAGON-------*/

SleepWagon::SleepWagon(int b): beds(b), PersonWagon(Sleeping, Wagons::generateId())
{}

//returns number of beds
int SleepWagon::capacity()
{
	return beds;
}

//returns string with info
string SleepWagon::getInfo()
{
	return PersonWagon::getInfo() + " Total Beds: " + std::to_string(capacity());
}

/*------SITTING-WAGON------*/

SittingWagon::SittingWagon(bool internet, int seats):internet(internet), seats(seats), PersonWagon(Sitting, Wagons::generateId()){}
//returns seats
int SittingWagon::capacity()
{
	return seats;
}

//returns string with info
string SittingWagon::getInfo()
{
	return PersonWagon::getInfo() + " Seats: " + std::to_string(capacity()) + " Has internet: " + ((hasInternet()) ? "yes" : "no");
}

//returns true if wagon has internet
bool SittingWagon::hasInternet() const
{
	return internet;
}

/*------ELECTRIC-MOTOR-WAGON------*/

ElectricMotorWagon::ElectricMotorWagon(int me, int maxSpeed): maxEffect(me), MotorWagon(Electric, maxSpeed, Wagons::generateId())
{}

//returns maxEffect
int ElectricMotorWagon::getMaxEffect() const
{
	return maxEffect;
}

//returns string with info
string ElectricMotorWagon::getInfo()
{
	return MotorWagon::getInfo() + " Max effect: " + std::to_string(getMaxEffect());
}

/*------DIESEL-MOTOR-WAGON------*/

DieselMotorWagon::DieselMotorWagon(int use, int maxSpeed): use(use), MotorWagon(Diesel, maxSpeed, Wagons::generateId())
{}

//returns gasolin use
int DieselMotorWagon::getUse() const
{
	return use;
}

//returns string with info
string DieselMotorWagon::getInfo()
{
	return MotorWagon::getInfo() + " Use: " + std::to_string(getUse()); 
}

/*------OPEN-FREIGHT-WAGON*/

OpenFreightWagon::OpenFreightWagon(int sqMeters, int maxWeight): sqMeters(sqMeters), maxWeight(maxWeight), FreightWagon(Open, Wagons::generateId())
{}

//returns capacity in sqMeters
int OpenFreightWagon::capacity()
{
	return sqMeters;
}

//returns max loading wight
int OpenFreightWagon::getMaxWeight() const
{
	return maxWeight;
}

//returns string with info
string OpenFreightWagon::getInfo()
{
	return FreightWagon::getInfo() + " Max capacity in square meters: " + std::to_string(capacity()) + "Max weight: " + std::to_string(getMaxWeight());
}

/*------CLOSED-FREIGHT-WAGON-------*/

ClosedFreightWagon::ClosedFreightWagon(int maxC): maxCapacity(maxC), FreightWagon(Closed, Wagons::generateId())
{}

//returns max capacity in cubic meters
int ClosedFreightWagon::capacity()
{
	return maxCapacity;
}

//returns string with info
string ClosedFreightWagon::getInfo()
{
	return FreightWagon::getInfo() + " Max Capacity in cubic meters: " + std::to_string(capacity());
}

