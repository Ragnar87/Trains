#include "TimeConverterHeader.h"

/*
Helper class to convert time to correct format
*/

int TimeConverter::formatToTime(int time)
{

	if(time % 100 <= 59)
	{
		time = ((time /100) *60) + (time % 100);
		return time;
	}
	return 0;
}

string TimeConverter::formatToString(int time){
	string timeString = "";
	timeString = std::to_string(time /60);
	timeString += ":" + std::to_string(time % 60);
	return timeString;
}