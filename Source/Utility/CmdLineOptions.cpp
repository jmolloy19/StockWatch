#include <Utility/CmdLineOptions.hpp>

/**
 * Function to parse command line arguments for options.
 * @param argc    command line argument count
 * @param argv[]  array of command line arguments
 * @param options pointer to array specifying given command line options
 */
void checkCmdLineOptions(int argc, char* argv[], bool* options)
{
    bool invalid = false;
    int invalidIdx;
    bool help = false;

    for(int i = 1; i < argc && !help && !invalid; i++)
    {
        if(strlen(argv[i]) < 2)
        {
            invalid = true;
            invalidIdx = i;
            break;
        }

        if(strncmp(argv[i], "-", 1) == 0 && strncmp(argv[i], "--", 2) != 0)
        {
            for(size_t c = 1; c < strlen(argv[i]); c++)
            {
                if(argv[i][c] == 'n')
                {
                    *(options) = true;
                    continue;
                }
                else if(argv[i][c] == 'r')
                {
                    *(options + 1) = true;
                    continue;
                }
                else if(argv[i][c] == 'w')
                {
                    *(options + 1) = true;
                    continue;
                }
                else if(argv[i][c] == 'h')
                {
                    help = true;
                    break;
                }
                else
                {
                    invalid = true;
                    invalidIdx = i;
                    break;
                }  
            }
        }
        else
        {
            if(strcmp(argv[i], "--nyse") == 0)
            {
                *(options) = true;
                continue;
            }
            else if(strcmp(argv[i], "--read-file") == 0)
            {
                *(options + 1) = true;
                continue;
            }
            else if(strcmp(argv[i], "--write-file") == 0)
            {
                *(options + 2) = true;
                continue;
            }
            else if(strcmp(argv[i], "--help") == 0)
            {
                help = true;      
            }
            else
            {
                invalid = true;
                invalidIdx = i;
            }  
        }   
    }

    if(invalid)
    {
        std::cerr << "\nInvalid Option: " << '\'' << argv[invalidIdx] << "\'\n";
    }

    if(help || invalid)
    {
        displayManual();
        exit(1);
    }

    if(*(options + 2))  // No point in reading from file if we already have to make API calls to write to files
    {
        *(options + 1) = false;
    }
}

/**
 * Function to display usage manual.
 */
void displayManual()
{
    std::cout << "\nUsage: ./Build/Analyze [OPTIONS...]\n\n"
              << "Options:\n"
              << "  -h, --help          Display usage manual.\n"
              << "  -n, --nyse          Also scans all stocks on the NYSE.\n"
              << "                      (Only scans NASDAQ by default)\n"
              << "  -w, --write-file    Writes historical data of each stock to\n"
              << "                      a .csv file. Files are stored in directory\n"
              << "                      ./DataFiles (which will be created if\n"
              << "                      it doesn't exist already).\n"
              << "  -r, --read-file     Reads historical data from files in\n"
              << "                      DataFiles directory instead of making API\n"
              << "                      calls. Can only use this option if\n"
              << "                      previously ran with \'--write-file\'.\n\n";
}