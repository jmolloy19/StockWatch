#include <utility/files.hpp>

/**
 * Reads file contents and writes to string.
 * @param file_path     path of file to read from
 * @param read_buffer   string that will be written to
 */
void ReadFromFile(const std::string& file_path, std::string* read_buffer)
{
    if(!FileExists(file_path))
    {
        throw std::runtime_error("Unable to read datafile: Does not exist\n");
    }

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
    if(FileExists(file_path))
    {
        std::remove(file_path.c_str());
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
 * Creates the directory specified by dir_path.
 * @param dir_path  dirctory path
 */
void CreateDirectory(const std::string& dir_path)
{
    if(mkdir(dir_path.c_str(), ACCESSPERMS) != 0)
    {
        std::cerr << "Could not create directory: " << dir_path << "\n"
                  << std::strerror(errno) << '\n';
        exit(-1);
    }
}

/**
 * Returns true if the directory exists.
 * @param dir_path   path of directory
 */
bool DirectoryExists(const std::string& dir_path)
{
    struct stat buffer;
    return stat(dir_path.c_str(), &buffer) == 0;
}

/**
 * Returns true if the file exists.
 * @param file_path   path of file
 */
bool FileExists(const std::string& file_path)
{
    struct stat buffer;
    return stat(file_path.c_str(), &buffer) == 0;
}