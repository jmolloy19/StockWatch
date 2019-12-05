#include <utility/files.hpp>

/**
 * Reads file contents and writes to string.
 * @param stock_name    name of stock (ticker symbol)
 * @param read_buffer   string that will be written to
 */
void ReadFromFile(const std::string& stock_name, std::string* read_buffer)
{
    std::string file_name = kStockDataFilesDirectory + stock_name + ".csv";

    std::ifstream in_file(file_name);
    if(in_file.is_open())
    {
        std::stringstream buffer;
        buffer << in_file.rdbuf();
        *read_buffer = buffer.str();
    }
    else
    {
        std::cerr << "Could not open file for reading: " << file_name << "\n";
        exit(-1);
    }    
}

/**
 * Writes string to file in directory datafiles.
 * File is named '<stock_name>.csv'. ie. AAPL.csv
 * @param stock_name    name of stock (ticker symbol)
 * @param write_buffer  string that will be written to file
 */
void WriteToFile(const std::string& stock_name, const std::string& write_buffer)
{
    std::string file_name = kStockDataFilesDirectory + stock_name + ".csv";

    if(!DirectoryExists(kStockDataFilesDirectory))
    {
        if(!CreateDirectory(kStockDataFilesDirectory.c_str()))
        {
            std::cerr << "Could not create directory: " << std::strerror(errno) << "\n";
            exit(-1);
        }     
    }

    std::ofstream out_file(file_name, std::ios::trunc);
    if(out_file.is_open())
    {
        out_file << write_buffer << std::flush;
        out_file.close();
    }
    else
    {
        std::cerr << "Could not open file for writing: " << file_name << "\n";
        exit(-1);
    }
}