#include "RetrieveStockSymbols.h"
#include "RetrieveHistoricalData.h"
#include "HistoricalDataObj.h"
#include <iomanip>

int main(int argc, char** argv) 
{
	string stockSymbols = fetchStockSymbols();
	vector<string> symbList = createSymbolList(stockSymbols);
	
	vector<string>::iterator symbol, 
							 begin = symbList.begin(), 
							 end = symbList.end();
	
	cout << "Stock Ticker Symbols Extracted. Total # of Stocks to Analyze: " << (end - begin) << endl
		 << "Analyzing Stocks for Patterns..." << endl;
	
	for(symbol = begin; symbol != end; symbol++)
	{
		HistoricalData stock;
		stock.inputData( fetchHistoricalData(*symbol) );
		//cout << (symbol - begin) << "  " << *symbol << "  input successful   " << endl;
	
		if( stock.checkForHTF() )
			cout << *symbol << endl;
			
		if( (symbol - begin) % 500 == 0 && symbol != begin)
			cout << setprecision(2)
				 <<	(double)(symbol - begin) / (double)(end - begin) * 100.0 << "% " << " complete" << endl;
	}
	system("pause");
	return 0;
}

