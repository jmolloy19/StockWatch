#include <stockwatch/options.hpp>

/**
 * Constructor for Options class.
 * @param argc      command line argument count
 * @param argv[]    array of command line arguments
 */
Options::Options(int argc, char* argv[])
{
    bool invalid = false;
    bool help = false;
    int invalidIdx;

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
 * Setter function to set options
 */
void Options::Set(bool nyse, bool read, bool write)
{
    include_nyse_ = nyse;
    read_from_file_ = read;
    write_to_file_ = write;
}

/**
 * Returns option signifying whether to also analyze stocks from the NYSE
 */
bool Options::IncludeNYSE() const
{
    return include_nyse_;
}

/**
 * Returns option signifying whether to read historical data from files
 */
bool Options::ReadFromFile() const
{
    return read_from_file_;
}

/**
 * Returns option signifying whether to write historical data to files
 */
bool Options::WriteToFile() const
{
    return write_to_file_;
}

/**
 * Function to display usage manual.
 */
void Options::DisplayManual()
{
    std::cout << "\nUsage: ./build/StockWatch.exe [OPTIONS...]\n\n"
              << "Options:\n"
              << "  -h, --help          Display usage manual.\n"
              << "  -n, --nyse          Also scans all stocks on the NYSE.\n"
              << "                      (Only scans NASDAQ by default)\n"
              << "  -w, --write-file    Writes a list of all stocks to the file\n" 
              << "                      \'stocklist.csv\'. Also Writes the historical\n"
              << "                      data of each stock to a .csv file. These\n"
              << "                      historcial data files are named according to\n" 
              << "                      each stock's corresponding symbol and stored\n" 
              << "                      in the directory \'datafiles\' (which will be\n" 
              << "                      created if it does not existalready).\n"
              << "  -r, --read-file     Reads files instead of making API calls.\n"
              << "                      It reads the list of stocks from the file\n"
              << "                      \'stocklist.csv\', and reads the historical\n"
              << "                      data of each stock from their corresponding\n"
              << "                      .csv file. This option can only be used if\n"
              << "                      previously ran with the \'--write-file\' option.\n";
}