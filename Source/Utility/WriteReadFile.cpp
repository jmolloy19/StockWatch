#include <Utility/WriteReadFile.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
//Windows
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

void writeToFile(const std::string& filename, const std::string& writeBuffer)
{
    if(!directoryExists(DATAFILES_PATH))
    {
        if(!createDir(DATAFILES_PATH.c_str()))
        {
            std::cerr << std::strerror(errno) << std::endl;
            exit(-1);
        }     
    }

    std::ofstream outfile(DATAFILES_PATH + filename, std::ios::trunc);
    if(outfile.is_open())
    {
        outfile << writeBuffer << std::endl;
        outfile.close();
    }
    else
    {
        std::cerr << "Could not write to file " << DATAFILES_PATH + filename << std::endl;
        exit(-1);
    }
}

void readFromFile(const std::string& filename, std::string* readBuffer)
{
    std::ifstream infile(DATAFILES_PATH + filename, std::ios::binary | std::ios::ate);
    if(infile.is_open())
    {
        std::ifstream::pos_type fileSize = infile.tellg();
        readBuffer->reserve(fileSize);
        infile.seekg(0, std::ios::beg);
        infile.read(&(*readBuffer)[0], fileSize);
    }
    else
    {
        std::cerr << "Could not read from file " << DATAFILES_PATH + filename << std::endl;
        exit(-1);
    }    
}
