#include <Utility/WriteReadFile.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
//Windows
bool createDir(const char* path)
{
    return _mkdir(DATAFILES_PATH.c_str()) == 0;
}
#else
bool createDir(const char* path, mode_t mode)
{
    return mkdir(DATAFILES_PATH.c_str(), ACCESSPERMS) == 0;
}
#endif

bool directoryExists(const std::string& dir)
{
    struct stat buffer;
    return (stat(dir.c_str(), &buffer) == 0);
}

bool writeToFile(const std::string& filename, const std::string& writeBuffer)
{
    std::ofstream outfile(DATAFILES_PATH + filename, std::ios::trunc);
    if(outfile.is_open())
    {
        outfile << writeBuffer << std::endl;
        outfile.close();
    }
    else
    {
        std::cerr << "Could not write to file " << DATAFILES_PATH + filename << std::endl;
    }
}

bool readFromFile(const std::string& filename, std::string* readBuffer)
{
    std::ifstream infile(DATAFILES_PATH + filename, std::ios::binary | std::ios::ate);
    if(infile.is_open())
    {
        std::ifstream::pos_type fileSize = infile.tellg();
        infile.seekg(0, std::ios::beg);
        infile.read(&(*readBuffer)[0], fileSize);
    }
    else
    {
        std::cerr << "Could not read from file " << DATAFILES_PATH + filename << std::endl;
    }    
}
