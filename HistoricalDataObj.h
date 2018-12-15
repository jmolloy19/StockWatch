#ifndef HISTORICAL_DATA_H
#define HISTORICAL_DATA_H
#include <vector>
#include <iostream>
using namespace std;

class HistoricalData
{
	public:
		HistoricalData();
		void inputData(const string& readBuffer);
		int getSize() const;
		bool checkForHTF();
		friend ostream & operator << (ostream &out, const HistoricalData &obj);
	private:
		vector<double> highs;    
		vector<double> lows;
		vector<double> closes;
		vector<int> volumes;
		int size;
		bool valid;		
};

#endif
