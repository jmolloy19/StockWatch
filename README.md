# StockWatch

Requests historical data for all stocks on NASDAQ and NYSE and scans for high tight flag patterns.

## Dependencies

This project requires:
* Linux/MacOS Enviroment
* [curl](https://curl.haxx.se/download.html)
* [CMake](https://cmake.org/download/)
* [World Trading Data API](https://www.worldtradingdata.com/)

Note: World Trading Data offers a free API key however it only allows 250 requests/day. 
      The 8 dollars a month API key allows 5000 requests/day, which easily covers all of the NASDAQ and most of the NYSE.

#### Inserting Your API Key

You will need to insert your World Trading Data API key into the file
[world_trading_api.hpp](https://github.com/jmolloy19/StockWatch/blob/master/include/stockwatch/world_trading_api.hpp)

```
Your Example API Key: 667myExampleApiKey99099IAmAPoorBoyToo77
```
Using the example API Key above, I would change
```cpp
const std::string API_KEY = "Insert_Your_API_Key_Here";
``` 
To
```cpp
const std::string API_KEY = "667myExampleApiKey99099IAmAPoorBoyToo77";
``` 

## Usage

To build the project, run the following command in terminal:

```bash
bash build.sh
```
To run the program after building the project, run the following command in terminal:

```bash
./build/StockWatch
```

### Options
```
-h, --help          Display usage manual.

-n, --nyse          Also scan all stocks on the NYSE. (Only scans NASDAQ by default)

-w, --write-file    Writes the historical data of each stock to a .csv file. These historcial 
                    data files are named according to each stock's corresponding symbol and are 
                    stored in the directory 'datafiles' (which will be created if it does not 
                    exist already).

-r, --read-file     Reads the historical data of each stock from their corresponding .csv file 
                    instead of making API calls. This option can only be used if previously ran 
                    with the '--write-file' option.

-u, --update-list   Updates the file 'stocklist.csv' by making an API call for the list of
                    stocks, and writing a new file.

```
### Usage Examples

```bash
./build/StockWatch --nyse --write-file
```

```bash
./build/StockWatch -nw
```

The two examples above are equivalent