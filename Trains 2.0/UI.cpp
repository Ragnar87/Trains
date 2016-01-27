#include "UIHeader.h"



/*
only pulbic method
runs start menu
*/
void UI::run()
{
	startMenu();
	
}
/*
Start menu. Choices are to start default simulation, or to start 
user specified simulation
*/
void UI::startMenu()
{
	bool cont = true;
	while(cont){
		cout << "Welcome to the Railway simulation" << endl;
		cout << "Please choose a menu item:" << endl;
		cout << "[1] Start default simulation" << endl;
		cout << "[2] Start user specified simulation" << endl;
		cout << "[x] Exit" << endl;
		switch(getMenuOption())
		{
		case '1':
			startDefaultSim();
			simMenu();
			break;
		case '2':
			specifySim();
			simMenu();
			break;
		case 'x':
		case 'X':
			cont = false;
		default:
			break;
		}
	}

}
/*
main simulation menu
*/
void UI::simMenu()
{
	bool cont = true;
	while(cont)
	{
		cout << "Simulation menu" << endl;
		cout << "Please choose option:" << endl;
		cout << "[1] Increment time" << endl;
		cout << "[2] Process next event" << endl;
		cout << "[3] Change increment" << endl;
		cout << "[4] Print station" << endl;
		cout << "[5] Print train" << endl;
		cout << "[6] Back to start menu" << endl;
		switch(getMenuOption())
		{
		case '1':
			cont = sim->incrementTime();
			break;
		case '2':
			cont = sim->processNext();
			break;
		case '3':
			if(changeIncrement())
				cout << "Increment changed" << endl;				
			else
				cout << "Invalid increment argument!!!" << endl;
			break;
		case '4':
			printStations();
			break;
		case '5':
			printTrain();
			break;
		case '6':
			cont = false;
			break;
		}
	}
	/*
	when simulation is over
	stats are presented
	*/
	int totalDelay = 0;
	cout << endl;
	for(std::pair<shared_ptr<Train>, int> t: sim->getDelays())
	{
		if(t.second != 9999){
			cout << "Train: " << t.first->getId() << " delayed: " << TimeConverter::formatToString(t.second) << endl;
			totalDelay += t.second;
		}
		else
			cout << "Train: " << t.first->getId() << " did not depart from: " << t.first->getOrigin()->getName() << endl;
	}
	if(totalDelay > 0)
		cout << "Total delay: " << TimeConverter::formatToString(totalDelay) << endl << endl;
	cout << "Simulation is finished" << endl << endl;


}


/*
function to get user char input
*/
char UI::getMenuOption()
{
	char choice;


	std::ios_base::iostate mask = ios::eofbit | ios::failbit | ios::badbit;
	cin.exceptions(mask);
	try{
		cin >> choice;
		cin.sync();
	}catch(std::ios_base::failure &e)
	{
		cout << "Exception caught" << endl;
		errorOutput << e.what();
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
	}
	return choice;

}
/*
starts default simulation
*/
bool UI::startDefaultSim()
{
	errorOutput.open(ERRORFILE, std::ios::app);
	sim=make_shared<Simulation>( Simulation(DEFSIMSTART, DEFSIMEND, DEFINCREMENT));
	try{
		railway.reset(new Railway(sim));
	}catch(std::bad_alloc &ba)
	{
		errorOutput<< "Exception caught while making unique_ptr railway" << ba.what();
		return false;
	}catch(std::exception &e)
	{
		cout << e.what();
	}
	return true;
}
/*
user specified simulaiton
invalid values will result in default values being used
*/
bool UI::specifySim()
{
	int simStart = 0, simEnd = 0, simIncrement = 0;
	string stationFile = "", trainFile = "", errorFile = "";
	cout << "If you want default values, write 0 for time input," << endl;
	cout << "and press enter for file input" << endl;
	cout << "Specify when sim should start, format HHMM: ";
	simStart = userInput();
	cout << endl;

	cout << "Specify when sim should end, format HHMM: ";
	simEnd = userInput();
	cout << endl;

	cout << "Specify sim increment, format HHMM: ";
	simIncrement = userInput();
	cout << endl;

	cout << "Specify the station file: ";
	getline(cin, stationFile);
	cout << "Specify the train file: ";
	getline(cin, trainFile);
	cout << "Specify the error output file: ";
	getline(cin, errorFile);

	simStart = TimeConverter::formatToTime(simStart);
	simEnd = TimeConverter::formatToTime(simEnd);
	simIncrement = TimeConverter::formatToTime(simIncrement);

	cout << endl;
	if(simStart < 0){
		cout << "Invalid sim start, using default" << endl;
		simStart = DEFSIMSTART;
	}
	if(simEnd == 0 || simEnd <= simStart){
		cout << "Invalid sim end, using default" << endl;
		simEnd = DEFSIMEND;
	}
	if(simIncrement <= 0){
		cout << "Invalid sim Increment, using default" << endl;
		simIncrement = DEFINCREMENT;
	}
	if(stationFile.empty()){
		cout << "Invalid station file, using default" << endl;
		stationFile = STATIONSFILE;
	}
	if(trainFile.empty()){
		cout << "Invalid train file, using default" << endl;
		trainFile = TRAINFILE;
	}
	if(errorFile.empty()){
		cout << "Invalid error file, using default" << endl;
		errorFile = ERRORFILE;
	}

	errorOutput.open(errorFile, std::ios::app);
	sim = make_shared<Simulation>( Simulation(simStart, simEnd, simIncrement));
	try{
	railway.reset(new Railway(stationFile, trainFile, errorFile, sim));
	}catch(std::bad_alloc &ba)
	{
		errorOutput << "Exception thrown while making unique_ptr railway" << ba.what();
		return false;
	}
	return true;


	
}
/*
calls Simulation::changeIncrement
*/
bool UI::changeIncrement()
{
	int increment;
	cout << "Specify what the increment should be format HHMM" << endl;
	increment = userInput();
	increment = TimeConverter::formatToTime(increment);
	if(!sim->changeIncrement(increment))
		return false;
	return true;
}
/*
user specifies which station it wants to view
*/
void UI::printStations()
{
	string sName = "";
	railway->printStations();
	cout << "Write the name of the station you want to view" << endl;
	std::getline(cin, sName);
	shared_ptr<Stations> station = railway->findStation(sName);
	if(station.get() != nullptr)
	{
		stationMenu(station);
	}
	else
	{
		cout << "Could not find station with the name " + sName << endl;
		cout << "press enter to return to sim menu..." << endl;
		cin.get();
	}
}
/*
allows user to view specific train
*/
void UI::printTrain()
{
	int id = 0;
	railway->printTrains();
	cout << "Write the id of the train you want to view" << endl;
	id = userInput();
	shared_ptr<Train> train = railway->findTrain(id);
	if(train.get() != nullptr)
	{
		trainMenu(train);
	}
	else
	{
		cout << "Could not fint train with id: " << id << endl;
		cout << "ptriss enter to return to sim menu..." << endl;
		cin.get();
	}
}
/*
allows user to view either trains in station
or wagons in station
*/
void UI::stationMenu(shared_ptr<Stations> station)
{
	cout << "Welcome to " + station->getName() + "'s menu" << endl;
	cout << "Please choose one of the following" << endl;
	cout << "[1] print trains in station" << endl;
	cout << "[2] print wagons in station" << endl;

	switch(getMenuOption())
	{
	case '1':
		station->printTrains();
		break;
	case '2':
		station->printWagons();
		break;
	default:
		cout << "Choice not recognised..." << endl;
		break;
	}

	cout << "Press enter to return to sim menu" << endl;
	cin.get();

}
/*
allows user to view different information about specific train
*/
void UI::trainMenu(shared_ptr<Train> train)
{
	cout << "Welcome to train: " << train->getId() << " menu" << endl;
	cout << "Please choose one of the following" << endl;
	cout << "[1] print train state " << endl;
	cout << "[2] print train info" << endl;
	cout << "[3] get train origin" << endl;
	cout << "[4] get train destination" << endl;
	
	switch(getMenuOption())
	{
	case '1':
		cout << "Train is in state: " << TrainStateStrings[train->getState()] << endl;
		break;
	case '2':
		train->getInfo();
		break;
	case '3':
		cout << train->getOrigin()->getName() << " Train departure: " << TimeConverter::formatToString(train->getDepartureTime()) << endl;
		break;
	case '4':
		cout << train->getDestination()->getName() << " Expected arrival: "<< TimeConverter::formatToString(train->getDepartureTime()+train->getTravelTime())<< endl;
		break;
	default:
		cout << "Choice not recognised" << endl;
		break;
	}
	cout << "Press enter to return to menu" << endl;
	cin.get();
	
}
/*
function used to get user int input
*/
int UI::userInput()
{
	int number = 0;
	std::ios_base::iostate mask = ios::eofbit | ios::failbit | ios::badbit;
	cin.exceptions(mask);
	try{
		cin >> number;
		cin.sync();
	}catch(std::ios_base::failure &e)
	{
		errorOutput << e.what() << endl;
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
	}
	return number;
}