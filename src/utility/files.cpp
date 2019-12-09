#include <utility/files.hpp>

/**
 * Reads file contents and writes to string.
 * @param file_path     path of file to read from
 * @param read_buffer   string that will be written to
 */
void ReadFromFile(const std::string& file_path, std::string* read_buffer)
{
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
 * @param file_path     path of file to write to
 * @param write_buffer  string that will be written to file
 * @param open_mode     the openmode of the file to be written to. Defaults to trunc
 */
void WriteToFile(const std::string& file_path, const std::string& write_buffer, std::ios::openmode open_mode)
{
    std::string dir_path;
    const size_t last_slash_idx = file_path.rfind('/');
    if (std::string::npos != last_slash_idx)
    {
        dir_path = file_path.substr(0, last_slash_idx);
    }

    if(!DirectoryExists(dir_path))
    {
        if(!CreateDirectory(dir_path))
        {
            std::cerr << "Could not create directory: " << dir_path << "\n"
                      << std::strerror(errno);
            exit(-1);
        }     
    }

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
 * Returns true if the directory was successfully created.
 * @param dir_path  dirctory path
 */
bool CreateDirectory(const std::string& dir_path)
{
    if(mkdir(dir_path.c_str(), ACCESSPERMS) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }  
}

/**
 * Returns true if the directory exists.
 * @param dir_path  directory path
 */
bool DirectoryExists(const std::string& dir_path)
{
    struct stat buffer;
    if(stat(dir_path.c_str(), &buffer) == 0)
    {
        return true;
    }
    else
    {
       return false;
    }
}