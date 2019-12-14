#include <stockwatch/options.hpp>

/**
 * Constructor for Options class.
 * @param argc      command line argument count
 * @param argv[]    array of command line arguments
 */
Options::Options(int argc, char* argv[])
{
    int invalidIdx;
    bool invalid = false;
    bool help    = false;

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
                    include_nyse_ = true;
                    continue;
                }
                else if(argv[i][c] == 'r')
                {
                    read_from_file_ = true;
                    continue;
                }
                else if(argv[i][c] == 'w')
                {
                    write_to_file_ = true;
                    continue;
                }
                 else if(argv[i][c] == 'u')
                {
                    update_list_ = true;
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
                include_nyse_ = true;
                continue;
            }
            else if(strcmp(argv[i], "--read-file") == 0)
            {
                read_from_file_ = true;
                continue;
            }
            else if(strcmp(argv[i], "--write-file") == 0)
            {
                write_to_file_ = true;
                continue;
            }
            else if(strcmp(argv[i], "--update-list") == 0)
            {
                update_list_ = true;
                continue;
            }
            else if(strcmp(argv[i], "--help") == 0)
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

    if(invalid)
    {
        std::cerr << "\nInvalid Option: " << '\'' << argv[invalidIdx] << "\'\n";
    }

    if(help || invalid)
    {
        DisplayManual();
        exit(1);
    }

    if(write_to_file_)  // No point in reading from file if we already have to make API call to write to file
    {
        read_from_file_ = false;
    }
}

/**
 * Returns option signifying whether to also analyze stocks from the NYSE
 */
bool Options::IncludeNYSE() const
{
    return include_nyse_;
}

/**
 * Returns option signifying whether to read data from files
 */
bool Options::ReadFromFile() const
{
    return read_from_file_;
}

/**
 * Returns option signifying whether to write data to files
 */
bool Options::WriteToFile() const
{
    return write_to_file_;
}

/**
 * Returns option signifying whether to create a new 'stocklist.csv' file
 */
bool Options::UpdateList() const
{
    return update_list_;
}

/**
 * Function to display usage manual.
 */
void Options::DisplayManual()
{
    std::cout << "\nUsage: ./build/StockWatch.exe [OPTIONS...]\n\n"
              << "Options:\n"
              << "  -h, --help          Display usage manual.\n"
              << "                      \n"
              << "  -n, --nyse          Also scans all stocks on the NYSE. Only\n"
              << "                      scans NASDAQ by default.\n"
              << "                      \n"
              << "  -w, --write-file    Writes the historical data of each stock to a\n"
              << "                      .csv file. These historcial data files are named\n" 
              << "                      according to each stock's corresponding symbol and\n" 
              << "                      are stored in the directory \'datafiles\' (which will\n" 
              << "                      be created if it does not exist already).\n"
              << "                      \n"
              << "  -r, --read-file     Reads the historical data of each stock from their\n"
              << "                      corresponding .csv file instead of making API calls.\n"
              << "                      This option can only be used if previously ran with\n"
              << "                      the \'--write-file\' option.\n"
              << "                      \n"
              << "  -u, --update-list   Updates the file \'stocklist.csv\' by making an API\n"
              << "                      call for the list of stocks, and writing a new file.\n\n";
}