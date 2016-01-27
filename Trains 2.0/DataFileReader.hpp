/*
Author: Ragnar Sandberg Mikkelsen
Class: HT14 DT060G Datateknik GR(B), Objektorienterad programmering i C++
Filename: DataFileReader.hpp
Description:
This is the DataFileReader file, a template class used for reading values

*/

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

/*
Datamembers are strings for filenames, and fstream object for reading from file
and ofstream for writing to file
*/
template<typename T>
class DataFileReader {
public:
	DataFileReader(std::string aDataFileName, std::string aErrorFileName);

	~DataFileReader();

	void openFiles();

	bool readNextValue(T &aValue);

	//void writeError(string str);

private:
	std::string aDataFileName;
	std::string aErrorFileName;
	std::fstream infile;
	std::ofstream outfile;

};
/*
constructor
*/
template<typename T>
DataFileReader<T>::DataFileReader(std::string dfn, std::string efn): aDataFileName(dfn), aErrorFileName(efn)
{}


/*
destructor
*/
template<typename T>
DataFileReader<T>::~DataFileReader()
{
	infile.close();
	outfile.close();
}

/*
open files
opens error file in append mode
*/
template<typename T>
void DataFileReader<T>::openFiles()
{

	infile.open(aDataFileName, std::ios::in);
	outfile.open(aErrorFileName, std::ios::app);
}

/*
reads value from file
*/
template<typename T>
bool DataFileReader<T>::readNextValue(T &aValue)
{
	std::string tmpString;
	while(!infile.eof())
	{
		try{
			infile >> aValue;

			return true;
		}
		catch(std::ios_base::failure f)
		{
			outfile << tmpString << std::endl;
		}
		catch(std::invalid_argument ia)
		{
			outfile << tmpString << std::endl;
		}

	}
	return false;

}
