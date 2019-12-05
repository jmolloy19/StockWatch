#include <utility/manual.hpp>

/**
 * Function to display usage manual.
 */
void DisplayManual()
{
    std::cout << "\nUsage: ./build/AnalyzeStocks [OPTIONS...]\n\n"
              << "Options:\n"
              << "  -h, --help          Display usage manual.\n"
              << "  -n, --nyse          Also scans all stocks on the NYSE.\n"
              << "                      (Only scans NASDAQ by default)\n"
              << "  -w, --write-file    Writes historical data of each stock to\n"
              << "                      a .csv file. Files are stored in directory\n"
              << "                      ./datafiles (which will be created if\n"
              << "                      it doesn't exist already).\n"
              << "  -r, --read-file     Reads historical data from files in\n"
              << "                      datafiles directory instead of making API\n"
              << "                      calls. Can only use this option if\n"
              << "                      previously ran with \'--write-file\'.\n\n";
}