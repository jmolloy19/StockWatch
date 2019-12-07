#include <utility/directory.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
/**
 * --Windows--
 * Returns true if the directory was successfully created.
 * @param path  directory path and name
 */
bool CreateDirectory(const char* path)
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
 * --Linux/MacOS--
 * Returns true if the directory was successfully created.
 * @param path  directory path and name
 */
bool CreateDirectory(const char* path)
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
 * @param directory     directory path and name
 */
bool DirectoryExists(const std::string& directory)
{
    struct stat buffer;
    if(stat(directory.c_str(), &buffer) == 0)
    {
        return true;
    }
    else
    {
       return false;
    }
}