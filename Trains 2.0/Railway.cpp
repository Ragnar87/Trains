#include "RailwayHeader.h"
#include <sstream>

/*
Function object used in algorithm find_if

*/
class FindStation
{
public:
	FindStation(string str): name(str)
	{}
	//operator== is actually not used in the program
	bool operator==(shared_ptr<Stations> const& station){return (station->getName() == name);}
	bool operator()(const shared_ptr<Stations> & station){return (station->getName() == name);}
private:
	string name;
};


/*
Function object used in algorithm find_if

*/
class FindTrain
{
public:
	FindTrain(int str): id(str)
	{}
	//operator== is not used in the program
	bool operator==(shared_ptr<Train> const& train){return (train->getId() == id);}
	bool operator()(const shared_ptr<Train> & train){return (train->getId() == id);}
private:
	int id;
};

/*
Mock train used to hold train info
*/
class MockTrain
{
public:
	MockTrain(){};
	void setOrigin(string o){ origin =o;}
	void setDestination(string d){destination = d;}
	void setId(int i){id = i;}
	void setDeparture(int d){departure = d;}
	void setTravelTime(int t){travelTime = t;}
	string getOrigin() const {return origin;}
	string getDestination() const {return destination;}
	int getId() const {return id;}
	int getDeparture() const {return departure;}
	int getTravelTime() const {return travelTime;}
	bool wagonToRequest(WagonType wt)
	{
		if(wt >= 3 && wt <= 8){
			if(wagons.empty())
			{
				if(wt == Electric || wt == Diesel){
					wagons.push(wt);
					return true;
				}
				return false;
			}
			wagons.push(wt);
			return true;
		}
		return false;

	}
	queue<WagonType> getWagons() const { return wagons; }

private:
	string destination;
	string origin;
	int id, departure, travelTime;
	queue<WagonType> wagons;
};

/*
Railway constructor when default files are used
*/
Railway::Railway(shared_ptr<Simulation> sim): stationsFile(STATIONSFILE), trainFile(TRAINFILE), errorFile(ERRORFILE), sim(sim)
{
	setup();
}
/*
constructor used when user wants to specify files
*/
Railway::Railway(string stationFile, string trainFile, string errorFile, shared_ptr<Simulation> simulation): sim(simulation),
	stationsFile(stationFile), trainFile(trainFile), errorFile(errorFile)
{
	setup();
}
/*
setup function
responsible for loading trains, stations and wagons


*/
void Railway::setup()
{
	DataFileReader<shared_ptr<Stations>> stationReader(stationsFile, errorFile);
	DataFileReader<MockTrain> trainReader(trainFile, errorFile);

	std::ofstream errorOutput(errorFile, std::ios::app);

	stationReader.openFiles();
	trainReader.openFiles();

	shared_ptr<Stations> tmpStation;
	MockTrain mTrain = MockTrain();

	/*
	while loop reads all stations and
	adds them to stations if they do not already exist
	*/
	while(stationReader.readNextValue(tmpStation))
	{	
		try{
			/*
			if the station doesn't exist it is added to the vector
			*/
			auto it = find(stations.begin(), stations.end(), tmpStation);
			if(it == stations.end())
				stations.push_back(tmpStation);
			else
				errorOutput << "Station already exists " << tmpStation->getName() << endl;

			shared_ptr<Stations> tmpStation;
		}catch(std::bad_alloc &ba)
		{
			errorOutput << "Exception caught while reading stations: " << ba.what();
		}


	}
	/*
	while loop reads all trains and
	adds them to trains if it doesn't already exist
	NotAssembledEvent is created for each train
	*/
	while(trainReader.readNextValue(mTrain))
	{
		shared_ptr<Stations> origin = findStation(mTrain.getOrigin());
		shared_ptr<Stations> destination = findStation(mTrain.getDestination());
		if(findTrain(mTrain.getId()) != nullptr)
		{
			errorOutput << "Train already exists " << std::to_string(mTrain.getId()) << endl;
		}
		else if(origin == nullptr)
		{
			errorOutput<< "Station of origin does not exist " << mTrain.getOrigin() << endl;
		}
		else if(destination == nullptr)
		{
			errorOutput << "Destination station does not exist " << mTrain.getDestination() << endl;
		}
		else if(mTrain.getDeparture() < sim->getCurrentTime())
		{
			errorOutput << "Train departs before current time. Current time: " << sim->getCurrentTime() 
				<< " Train departure: " << mTrain.getDeparture() << endl;
		}
		else if(mTrain.getDeparture() > sim->getEndTime())
		{
			errorOutput << "Train departs after simulation has ended. Sim ends: " << sim->getEndTime()
				<< " Train departure: " << mTrain.getDeparture() << endl;
		}
		else
		{
			shared_ptr<Train> train;

			try{
				train.reset(new Train(origin, destination, mTrain.getDeparture(), mTrain.getTravelTime(), mTrain.getId()));
				trains.push_back(train);
				train->wagonTypeQueue(mTrain.getWagons());
				train->getOrigin()->addTrain(train);
				sim->registerEvent(make_shared<NotAssembledEvent> ( NotAssembledEvent(train, sim, train->getDepartureTime()-NOTASSEVENTDECREASE)));
			}catch(std::bad_alloc &ba)
			{
				errorOutput << "Caught exception while making train" << ba.what();
			}
		}


	}
}

/*
searches for station with given name, and returns it if found
if not found it returns a nullptr. Uses find_if with function object
*/
shared_ptr<Stations> Railway::findStation(string stationName)
{
	FindStation name(stationName);
	std::vector<shared_ptr<Stations>>::iterator it;
	it = std::find_if(stations.begin(), stations.end(), name);
	if(it != stations.end())
		return *it;
	else
		return nullptr;
}

/*
searches for station with given id, and returns it if found
if not found it returns a nullptr. Uses find_if with function object
*/
shared_ptr<Train> Railway::findTrain(int id)
{
	FindTrain ft(id);
	std::vector<shared_ptr<Train>>::iterator it;
	it = std::find_if(trains.begin(), trains.end(), ft);
	if(it != trains.end())
		return *it;
	else
		return nullptr;
}
/*
prints id of all rains
*/
void Railway::printTrains()
{
	for(auto it = trains.begin(); it != trains.end(); it++)
	{
		cout << it->get()->getId() << endl;
	}
}
/*
prints names of all trains
*/
void Railway::printStations()
{
	for(auto it = stations.begin(); it!= stations.end(); it++)
	{
		cout << it ->get()->getName() << endl;
	}

}

/*
overloaded operator >> used to read trains from file
*/
std::istream &operator>>(std::istream &is, MockTrain &mTrain)
{

	mTrain = MockTrain();
	std::stringstream ss;
	string train;
	string origin, destination;
	int id, departure, travelTime, wagonType;
	std::getline(is, train);
	if(!train.empty())
	{
		ss << train;
		ss >> origin;
		ss >> destination;
		ss >> departure;
		ss >> travelTime;
		ss >> id;
		//Delete newline character, if it is there
		if(*origin.begin() == '\n'|| *origin.begin() == '\r')
			origin.erase(origin.begin(), origin.begin()+1);
		if(id > 0 && departure > 0 && travelTime > 0)
		{
			mTrain.setOrigin(origin);
			mTrain.setDestination(destination);
			mTrain.setId(id);
			mTrain.setDeparture(departure);
			mTrain.setTravelTime(travelTime);
			while(ss >> wagonType)
			{
				mTrain.wagonToRequest(static_cast<WagonType>(wagonType));
			}
		}
	}

	return is;

}