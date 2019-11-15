#include <Utility/CmdLineOpt.hpp>

void checkCmdLineOptions(int argc, char* argv[], bool* options)
{
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "--nyse") == 0)
        {
            *(options) = true;
            continue;
        }
        else if(strcmp(argv[i], "--write-file") == 0)
        {
            *(options + 1) = true;
            continue;
        }
        else if(strcmp(argv[i], "--read-file") == 0)
        {
            *(options + 2) = true;
            continue;
        }
        else
        {
            if(strcmp(argv[i], "-h") != 0 && strcmp(argv[i], "-h") != 0)
            {
                std::cout << "Invalid Option: \'" << argv[i] << "\'\n\n";
            }
            displayManual();
            exit(1);
        }   
    }
}

void displayManual()
{
    std::cout << "---------------------------------------------\n"
              << "|           STOCKWATCH MANUAL               |\n"
              << "---------------------------------------------\n"
              << "   OPTION               DESCRIPTION\n"
              << " -h, --help    - Display Manual\n"
              << " --nyse        - Also scans all stocks on the\n"
              << "                 NYSE. (Only scans NASDAQ by\n"
              << "                 default)\n"
              << " --write-file  - Writes historical data to file.\n"
              << "                 Files are stored in directory\n"
              << "                 named DataFiles (which will be\n"
              << "                 created if non-existing)\n"
              << " --read-file   - Reads historical data from\n"
              << "                 files in directory DataFiles\n"
              << "                 Can only use this option when\n"
              << "                 running with \'--write-file\'\n"
              << "                 or if previously ran with\n"
              << "                 \'--write-file\'. (Meant to\n"
              << "                 save on API calls)\n\n";
}