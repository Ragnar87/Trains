#include "EventHeader.h"

#include "SimulationHeader.h"
/*

*/
Event::Event(int t): time(t)
{

}

int Event::getTime() const
{
	return time;
}

/*
implementation of Event::processEvent
train try's to get all wagons in wagon queue
if successful an AssembledEvent is created, else an
Incomplete event is created
*/
void NotAssembledEvent::processEvent()
{
	cout << train->getId() << " Is in NOT ASSEMBLED state " << endl;
	bool cont = true;
	/*
		while loop continues until either train can't get requested wagon
		or queue with wagons to request is empty
	*/
	while(cont)
	{
		if(!(train->getWagons().empty()))
		{
			cont = train->getOrigin()->requestWagon(train, train->getWagons().front());
		}
		else
		{
			cont = false;
			if(getTime()+train->getTravelTime() < sim->getEndTime()){
				sim->registerEvent(make_shared<AssembledEvent>( AssembledEvent(train, sim, getTime())));
				train->incrementState(true);
			}
		}
	}
	if(!(train->getWagons().empty()) && (getTime()+train->getTravelTime() < sim->getEndTime()))
	{
		sim->registerEvent(make_shared<IncompleteEvent>( IncompleteEvent(train, sim, getTime()+INCOMPLETETIMEINCREASE)));
		train->incrementState(false);
	}
	/*
		if trains travelTime is < than Simulation end time
		train is added to vector with delayd train, with
		integer value 9999, representing that the train did 
		not depart
	*/
	else if(getTime()+train->getTravelTime() > sim->getEndTime())
		sim->addDelay(train, 9999);
	
}

/*
IncompleteEvent implementation of processEvent
similar to NotAssembled event above
train try's to get missing wagons, if successful AssembledEvent is created
if not another incomplete event is created
*/
void IncompleteEvent::processEvent()
{
	cout << train->getId() << " Is in INCOMPLETE state " << endl;

	bool cont = true;
	/*
		while loop continues until either train can't get requested wagon
		or queue with wagons to request is empty
	*/
	while(cont)
	{
		if(!(train->getWagons().empty()))
		{
			cont = train->getOrigin()->requestWagon(train, train->getWagons().front());
		}
		else
		{
			cont = false;
			if(getTime()+train->getTravelTime() < sim->getEndTime()){
				sim->registerEvent(make_shared<AssembledEvent> (AssembledEvent(train, sim, getTime())));
				train->incrementState(true);
			}
		}
	}
	if(!(train->getWagons().empty())&&(getTime()+train->getTravelTime() < sim->getEndTime()))
	{
		sim->registerEvent(make_shared<IncompleteEvent> (IncompleteEvent(train, sim, getTime() + INCOMPLETETIMEINCREASE)));
		train->incrementState(false);
	}
	/*
		if trains travelTime is < than Simulation end time
		train is added to vector with delayd train, with
		integer value 9999, representing that the train did 
		not depart
	*/
	else if(getTime()+train->getTravelTime() > sim->getEndTime())
		sim->addDelay(train, 9999);

}

/*
Assembled event implementation os Event::processEvent
*/
void AssembledEvent::processEvent()
{
	cout << train->getId() << " Is in ASSEMBLED state " << endl;
	if(getTime()+train->getTravelTime() < sim->getEndTime()){
		sim->registerEvent(make_shared<ReadyEvent>( ReadyEvent(train, sim, getTime()+READYEVENTTIMEINCREASE)));
		train->incrementState(true);
	}


}
/*
ReadyEvent implementation of processEvent
if no delays have occured, getTime() + RUNNINGEVENTINCREASE is equal to
tain->getDepartureTime();
*/
void ReadyEvent::processEvent()
{
	cout << train->getId() << " Is in READY state " << endl;
	if(getTime()+train->getTravelTime() < sim->getEndTime()){
		sim->registerEvent(make_shared<RunningEvent>( RunningEvent(train, sim, getTime()+RUNNINGEVENTINCREASE)));
		train->incrementState(true);
	}

}
/*
Running event implementation of Event::processEvent()
*/
void RunningEvent::processEvent()
{
	cout << train->getId() << " Is in RUNNING state " << endl;

	sim->registerEvent(make_shared<ArrivedEvent>( ArrivedEvent(train, sim, getTime() + train->getTravelTime())));
	train->incrementState(true);
}

/*
ArrivedEvent implementation of Event::processEvent
event adds train to destination station, and if train
is delayed the train is added to vector containing delayed trains
with amount of time the train is delayed
*/
void ArrivedEvent::processEvent()
{
	cout << train->getId() << " Is in ARRIVED state " << endl;

	sim->registerEvent(make_shared<FinishedEvent> (FinishedEvent(train, sim, getTime() + FINISHEDEVENTINCREASE)));
	train->incrementState(true);
	train->getDestination()->addTrain(train);
	if(getTime() > (train->getDepartureTime()+train->getTravelTime()))
		sim->addDelay(train, getTime()-(train->getDepartureTime()+train->getTravelTime()));
		
}
/*
FinishedEvent implementation of Event::processEvent()
all wagons in train are released to station and train
is removed from station
*/
void FinishedEvent::processEvent()
{
	cout << train->getId() << " Is in FINISHED state " << endl;
	Wagons* wagon = train->releaseWagon();
	while(!(wagon == nullptr))
	{
		if(!(wagon == nullptr))
			train->getDestination()->addWagon(wagon);

		wagon = train->releaseWagon();
	}
	train->getDestination()->removeTrain(train);
	cout << train->getId() << " All wagons released and train removed from station" << endl;
}