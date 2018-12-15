#include "HistoricalDataObj.h"
#include <string>
#include <algorithm>
#include <iterator>
using namespace std;

/*****************************************************************************************************/
// Constructor for HistoricalData class 
/*****************************************************************************************************/ 
HistoricalData::HistoricalData(): size(0), valid(false)
{}

/*****************************************************************************************************/
// This function takes as input the string returned by fetchHistoricalData(). It parses the string for 
// the closing prices and daily volumes of each trading day and inputs the data into a HistoricalData object. 
// Data is inputted in the order of oldest trading day first to most recent trading day last
/*****************************************************************************************************/  
void HistoricalData::inputData(const string& readBuffer)
{															 											
	if( !readBuffer.empty() && readBuffer[0] != 'U') 		 // If the readbuffer string isn't empty and it doesnt begin													
	{														 // with the first character of the HTTP request error message, its most
		valid = true;										 // likely valid and "valid" data member becomes true
		
		string::const_iterator it,
							   begin = readBuffer.begin(), 
							   end = readBuffer.end();
		
		size_t found = 0;									// Initialize found to 0
		while(found != string::npos)						// Loop until readBuffer.find() returns npos (ie no more "close"/"volume" strings to find)
		{													//
			found = readBuffer.find("close", found);		// Look for string "close"
			if (found != string::npos)						// If found is not equal to npos, then string was found
			{
				size++;										// Increase object size becuase another close value is found
				it = begin + found + 7; 					// Set iterator to start of "close" string and skip 7 characters to reach start of close value
				string closeStr(it, find(it, end, ','));	// Take close value as string, convert to double, and push back into closes vector
				closes.push_back(stod(closeStr));
				found = readBuffer.find("volume", found);   // Repeat for volume
				if (found != string::npos)
				{
					it = begin + found + 8; 				 
					string volStr(it, find(it, end, ','));
					volumes.push_back(stoi(volStr));
				}
			}
		}
	}
}

/*****************************************************************************************************/
// This function returns the size of the HistoricalData object (or number of trading days of data saved to object)
/*****************************************************************************************************/ 
int HistoricalData::getSize() const
{
	return size;
}

/*****************************************************************************************************/
// This function returns true if the historical data of a stock exhibits patterns of a high and tight flag.
// Otherwise it returns false
/*****************************************************************************************************/  
bool HistoricalData::checkForHTF()
{
	if( !valid || size < 60 )													
		return false;
		
	vector<double>::iterator it,
							 end = closes.end(),
							 highest = max_element(end - 60, end),
							 lowest = min_element(end - 60, end);
	
	if(*lowest == 0.0)
		return false;
	else if (*highest / *lowest < 2.0)
		return false;
	else if(highest < lowest)			//added this after working
		return false;
	else if( distance(highest, end) > 15 )
		return false;
	else
		for(it = (end - 15); it != end; it++)
			if( *it < (*highest * .8) )
				return false;
			
	return true;
}

/*****************************************************************************************************/
// Overloaded << operator to display HistoricalData objects
/*****************************************************************************************************/ 
ostream & operator << (ostream &out, const HistoricalData &obj)
{
	out << "#\tCloses\tVolume\n";
	for(int i = 0; i < obj.size; i++)
		out << i << "\t" << obj.closes[i] << "\t" << obj.volumes[i] << endl;
}
