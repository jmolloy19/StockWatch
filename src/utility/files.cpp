#include <utility/files.hpp>

/**
 * Reads file contents and writes to string.
 * @param file_name     name of file to write to
 * @param dir_path      path of the directory the file will be written to
 * @param read_buffer   string that will be written to
 */
void ReadFromFile(const std::string& file, const std::string& dir_path,  std::string* read_buffer)
{
    std::string file_path = dir_path + file;

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
 * @param file_name     name of file to write to
 * @param dir_path      path of the directory the file will be written to
 * @param write_buffer  string that will be written to file
 */
void WriteToFile(const std::string& file_name, const std::string& dir_path, const std::string& write_buffer)
{
    if(!DirectoryExists(dir_path))
    {
        if(!CreateDirectory(dir_path.c_str()))
        {
            std::cerr << "Could not create directory: " << std::strerror(errno) << "\n";
            exit(-1);
        }     
    }

    std::string file_path = dir_path + file_name;

    std::ofstream out_file(file_path, std::ios::trunc);
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