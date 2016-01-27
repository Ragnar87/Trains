/*
Header file for wagons

also contains implementations for derived classes
*/

#ifndef WAGONS_H
#define WAGONS_H

#include <string>
#include <vector>
#include <memory>

using std::make_shared;
using std::vector;

enum WagonType{Person, Freight, Motor, Sitting, Sleeping, Open, Closed, Electric, Diesel};
static const char * WagonTypeStrings[] = {"Person", "Freight", "Motor", "Sitting", "Sleeping", "Open", "Closed", "Electric", "Diesel"};

using std::string;

class Wagons
{
public:
	Wagons();
	Wagons(WagonType, int);
	int getId() const;
	virtual WagonType getType() const;
	virtual string getInfo();
	static int generateId();

private:
	const int id;
	const WagonType type;
	static vector<int> ids;

};

class PersonWagon : public Wagons
{
public:
	PersonWagon(WagonType, int);
	WagonType getType() const;
	virtual int capacity() = 0;
	virtual string getInfo();

private:
	WagonType type;
};


class MotorWagon: public Wagons
{
public:
	MotorWagon(WagonType, int, int);
	int getMaxSpeed() const;
	WagonType getType() const;
	virtual string getInfo();
private:
	int maxSpeed;
	WagonType type;

};




class FreightWagon: public Wagons
{
public:
	FreightWagon(WagonType, int);  //Add more
	FreightWagon();
	virtual int capacity()=0;
	WagonType getType() const;
	virtual string getInfo();

private:
	WagonType type;
};


class SleepWagon : public PersonWagon
{
public:
	SleepWagon(int);
	SleepWagon();
	virtual int capacity();
	virtual string getInfo() ;

private:
	int beds;

};

class SittingWagon : public PersonWagon
{
public:
	SittingWagon(bool, int);
	SittingWagon();
	bool hasInternet() const;
	virtual int capacity();
	virtual string getInfo();

private:
	bool internet;
	int seats;

};

class OpenFreightWagon: public FreightWagon
{
public:
	OpenFreightWagon(int, int); 
	OpenFreightWagon();
	virtual int capacity();
	int getMaxWeight() const;
	virtual string getInfo();

private:
	int sqMeters;
	int maxWeight;

};

class ClosedFreightWagon: public FreightWagon
{
public:
	ClosedFreightWagon(int);
	ClosedFreightWagon();
	virtual int capacity();
	virtual string getInfo();

private:
	int maxCapacity;

};

class ElectricMotorWagon: public MotorWagon
{
public:
	ElectricMotorWagon(int, int);
	ElectricMotorWagon();
	int getMaxEffect() const;
	virtual string getInfo();

private:
	int maxEffect;
};

class DieselMotorWagon: public MotorWagon
{
public:
	DieselMotorWagon();
	DieselMotorWagon(int, int);
	int getUse() const;
	virtual string getInfo();

private:
	int use;

};



#endif