#include <utility/options.hpp>

/**
 *  Constructor for Options class.
 * @param argc      command line argument count
 * @param argv[]    array of command line arguments
 */
Options::Options(int argc, char* argv[]) : argc_(argc)
{
    for(int i = 0; i < argc; i++)
    {
        argv_.push_back(argv[i]);
    }
}

/**
 * Function that parses command line arguments for program options.
 */
void Options::Parse()
{
    bool invalid = false;
    bool help = false;
    int invalidIdx;

    for(int i = 1; i < argc_ && !help && !invalid; i++)
    {
        if(strlen(argv_[i]) < 2)
        {
            invalid = true;
            invalidIdx = i;
            break;
        }

        if(strncmp(argv_[i], "-", 1) == 0 && strncmp(argv_[i], "--", 2) != 0)
        {
            for(size_t c = 1; c < strlen(argv_[i]); c++)
            {
                if(argv_[i][c] == 'n')
                {
                    include_nyse_ = true;
                    continue;
                }
                else if(argv_[i][c] == 'r')
                {
                    read_from_file_ = true;
                    continue;
                }
                else if(argv_[i][c] == 'w')
                {
                    write_to_file_ = true;
                    continue;
                }
                else if(argv_[i][c] == 'h')
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
            if(strcmp(argv_[i], "--nyse") == 0)
            {
                include_nyse_ = true;
                continue;
            }
            else if(strcmp(argv_[i], "--read-file") == 0)
            {
                read_from_file_ = true;
                continue;
            }
            else if(strcmp(argv_[i], "--write-file") == 0)
            {
                write_to_file_ = true;
                continue;
            }
            else if(strcmp(argv_[i], "--help") == 0)
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
        std::cerr << "\nInvalid Option: " << '\'' << argv_[invalidIdx] << "\'\n";
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
    std::cout << "\nUsage: ./build/StockWatch [OPTIONS...]\n\n"
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