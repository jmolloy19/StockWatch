#include <utility/file.hpp>

/**
 * Constructor for File class.
 * @param file_name     name of file
 * @param dir_path      path of the directory containing the file
 */
File::File(const std::string& file_name, const std::string& dir_path) :
    file_name_(file_name),
    dir_path_(dir_path)
{}

/**
* Reads file contents and writes to string.
* @param read_buffer   string that will be written to
*/
void File::Read(std::string* read_buffer)
{
    std::string file_path = dir_path_ + file_name_;

    std::ifstream in_file(file_path);
    if(in_file.is_open())
    {
        std::stringstream buffer;
        buffer << in_file.rdbuf();
        *read_buffer = buffer.str();
    }
    else
    {
        std::cerr << "Could not open file for reading: " << file_path << "\n";
        exit(-1);
    } 

}

/**
 * Writes string to file.
 * @param write_buffer  string that will be written to file
 * @param open_mode     the open mode for the file. Defaults to truncate
 */
void File::Write(const std::string& write_buffer, std::ios::openmode open_mode)
{
    if(!DirectoryExists())
    {
        if(!CreateDirectory())
        {
            std::cerr << "Could not create directory: " << dir_path_ << " : "
                      << std::strerror(errno) << "\n";
            exit(-1);
        }     
    }

    std::string file_path = dir_path_ + file_name_;

    std::ofstream out_file(file_path, open_mode);
    if(out_file.is_open())
    {
        out_file << write_buffer << std::flush;
        out_file.close();
    }
    else
    {
        std::cerr << "Could not open file for writing: " << file_path << "\n";
        exit(-1);
    }
}

/**
 * Returns true if the directory exists.
 */
bool File::DirectoryExists()
{
    struct stat buffer;
    if(stat(dir_path_.c_str(), &buffer) == 0)
    {
        return true;
    }
    else
    {
       return false;
    }
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
/**
 * --Windows--
 * Returns true if the directory was successfully created.
 */
bool File::CreateDirectory()
{
    if(_mkdir(dir_path_.c_str()) == 0)
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
 */
bool File::CreateDirectory()
{
    if(mkdir(dir_path_.c_str(), ACCESSPERMS) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }  
}
#endif