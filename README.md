# StockWatch

Requests historical data for all stocks on NASDAQ and NYSE and scans for high tight flag patterns.

## Dependencies

This project requires: 
* [curl](https://curl.haxx.se/download.html)
* [CMake](https://cmake.org/download/)
* [World Trading Data API](https://www.worldtradingdata.com/)

You will need to insert your World Trading Data API key into the file include/stockwatch/api.hpp
There are more detailed instructions with examples in this file.
[File To Insert API Key](https://github.com/jmolloy19/StockWatch/blob/master/include/stockwatch/api.hpp)

Note: World Trading Data offers a free API key however it only allows 250 requests/day. 
      The 8 dollars a month API key allows 5000 requests/day, which easily covers all of the NASDAQ and most of the NYSE.

## Usage

To build the project, run the following command in terminal:

```bash
bash build.sh
```
To run the program after building the project, run the following command in terminal:

__Windows__
```bash
.\build\StockWatch.exe
```

__Linux/MacOS__
```bash
./build/StockWatch.exe
```

### Options
```
-h, --help          Display usage manual.

-n, --nyse          Also scans all stocks on the NYSE. (Only scans NASDAQ by default)

-w, --write-file    Writes historical data of each stock to a .csv file.
                    Files are stored in directory ./datafiles (which will be created
                    if it doesn't exist already).

-r, --read-file     Reads historical data from .csv files instead of making API calls. 
                    Can only use this option if previously ran with '--write-file' option.
```
### Usage Examples

```bash
./build/StockWatch.exe --nyse --write-file
```

```bash
./build/StockWatch.exe -nw
```

The two examples above are equivalent
(and for Windows, you would replace the '/'s with '\\'s).
