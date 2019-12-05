#include <utility/read_write_files.hpp>
#include <utility/create_directory.hpp>

/**
 * Writes string to file.
 * @param file_name     file path and name
 * @param write_buffer  string that will be written to file
 */
void WriteToFile(const std::string& file_name, const std::string& write_buffer)
{
    if(!DirectoryExists(kDataFilesPath))
    {
        if(!CreateDirectory(kDataFilesPath.c_str()))
        {
            std::cerr << "Could not create directory: " << std::strerror(errno) << "\n";
            exit(-1);
        }     
    }

    std::ofstream out_file(kDataFilesPath + file_name, std::ios::trunc);
    if(out_file.is_open())
    {
        out_file << write_buffer << std::flush;
        out_file.close();
    }
    else
    {
        std::cerr << "Could not open file for writing: " << kDataFilesPath + file_name << "\n";
        exit(-1);
    }
}

/**
 * Reads file contents and writes to string.
 * @param file_name     file path and name
 * @param read_buffer   string that will be written to
 */
void ReadFromFile(const std::string& file_name, std::string* read_buffer)
{
    std::ifstream in_file(kDataFilesPath + file_name);
    if(in_file.is_open())
    {
        std::stringstream buffer;
        buffer << in_file.rdbuf();
        *read_buffer = buffer.str();
    }
    else
    {
        std::cerr << "Could not open file for reading: " << kDataFilesPath + file_name << "\n";
        exit(-1);
    }    
}
