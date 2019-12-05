#include <utility/options.hpp>

/**
 *  Default constructor for Options class.
 */
Options::Options() {}

/**
 * Constructor for Options class. Parses command line arguments for program options.
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