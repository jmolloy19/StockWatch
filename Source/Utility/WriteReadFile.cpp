#include <Utility/WriteReadFile.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
/**
 * Windows
 * Returns true if the directory was successfully created.
 * @param path directory path and name
 */
bool createDir(const char* path)
{
    if(_mkdir(path) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }  
}
#else

/**
 * Returns true if the directory was successfully created.
 * @param path directory path and name
 */
bool createDir(const char* path)
{
    if(mkdir(path, ACCESSPERMS) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }  
}
#endif

/**
 * Returns true if the directory exists.
 * @param dir directory path and name
 */
bool directoryExists(const std::string& dir)
{
    struct stat buffer;
    if(stat(dir.c_str(), &buffer) == 0)
    {
        return true;
    }
    else
    {
       return false;
    }
    
}

/**
 * Writes string to file.
 * @param filename    file path and name
 * @param writeBuffer string that will be written to file
 */
void writeToFile(const std::string& filename, const std::string& writeBuffer)
{
    if(!directoryExists(DATAFILES_PATH))
    {
        if(!createDir(DATAFILES_PATH.c_str()))
        {
            std::cerr << "Could not create directory: " << std::strerror(errno) << "\n";
            exit(-1);
        }     
    }

    std::ofstream outfile(DATAFILES_PATH + filename, std::ios::trunc);
    if(outfile.is_open())
    {
        outfile << writeBuffer << std::flush;
        outfile.close();
    }
    else
    {
        std::cerr << "Could not open file for writing: " << DATAFILES_PATH + filename << "\n";
        exit(-1);
    }
}

/**
 * Reads file contents and writes to string.
 * @param filename   file path and name
 * @param readBuffer string that will be written to
 */
void readFromFile(const std::string& filename, std::string* readBuffer)
{
    std::ifstream infile(DATAFILES_PATH + filename);
    if(infile.is_open())
    {
        std::stringstream buffer;
        buffer << infile.rdbuf();
        *readBuffer = buffer.str();
    }
    else
    {
        std::cerr << "Could not open file for reading: " << DATAFILES_PATH + filename << "\n";
        exit(-1);
    }    
}
