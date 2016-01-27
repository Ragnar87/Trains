#ifndef EVENTHEADER_H
#define EVENTHEADER_H

#include "TrainHeader.h"
#include "StationHeader.h"
#include <iostream>

using std::cout;
using std::endl;
using std::shared_ptr;
using std::make_shared;

const int INCOMPLETETIMEINCREASE = 10;
const int READYEVENTTIMEINCREASE = 20;
const int NOTASSEVENTDECREASE = 30;
const int RUNNINGEVENTINCREASE =10;
const int FINISHEDEVENTINCREASE = 20;
//forward declaration of Simulation
class Simulation;


/*
Base event class
contains pure virtual function processEvent()
which is called when event is to be processed
*/
class Event
{
private:
	int time;
public:
	Event(int);
	virtual ~Event(){}
	virtual void processEvent() = 0;
	int getTime() const;
};

/*
Event comparison class
used to sort priority queue
*/
class EventComparison {
public:
	bool operator() (shared_ptr<Event> left, shared_ptr<Event> right) {
        return left->getTime() > right->getTime();
    }
};

/*
All classes derived from base class Event
have private Train and Simulation members
and they implement pure virtual function Event::processEvent
*/
class NotAssembledEvent : public Event
{
private:
	shared_ptr<Train> train;
	shared_ptr<Simulation> sim;
public:
	NotAssembledEvent(shared_ptr<Train> train, shared_ptr<Simulation> s, int t): Event(t), train(train), sim(s) {}
	virtual void processEvent();
};

class IncompleteEvent: public Event
{
private:
	shared_ptr<Train> train;
	shared_ptr<Simulation> sim;
public:
	IncompleteEvent(shared_ptr<Train> train, shared_ptr<Simulation> s, int t): Event(t), train(train), sim(s) {}
	virtual void processEvent();

};

class AssembledEvent: public Event
{
private:
	shared_ptr<Train> train;
	shared_ptr<Simulation> sim;
public:
	AssembledEvent(shared_ptr<Train> train, shared_ptr<Simulation> s, int t): Event(t), train(train), sim(s) {}
	virtual void processEvent();
};

class ReadyEvent: public Event
{
private:
	shared_ptr<Train> train;
	shared_ptr<Simulation> sim;
public:
	ReadyEvent(shared_ptr<Train> train, shared_ptr<Simulation> s, int t): Event(t), train(train), sim(s) {}
	virtual void processEvent();
};

class RunningEvent: public Event
{
private:
	shared_ptr<Train> train;
	shared_ptr<Simulation> sim;
public:
	RunningEvent(shared_ptr<Train> train, shared_ptr<Simulation> s, int t): Event(t), train(train), sim(s) {}
	virtual void processEvent();
};

class ArrivedEvent: public Event
{
private:
	shared_ptr<Train> train;
	shared_ptr<Simulation> sim;
public:
	ArrivedEvent(shared_ptr<Train> train, shared_ptr<Simulation> s, int t): Event(t), train(train), sim(s) {}
	virtual void processEvent();
};

class FinishedEvent: public Event
{
private:
	shared_ptr<Train> train;
	shared_ptr<Simulation> sim;
public:
	FinishedEvent(shared_ptr<Train> train, shared_ptr<Simulation> s, int t): Event(t), train(train), sim(s) {}
	virtual void processEvent();
};

#endif