#ifndef TIMECONVERTER_H
#define TIMECONVERTER_H

#include <string>
using std::string;
/*
helper class to format time
*/
class TimeConverter
{
public:
	static string formatToString(int);
	static int formatToTime(int);
};
#endif