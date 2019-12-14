#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <sys/stat.h>

void ReadFromFile(const std::string& file_path, std::string* read_buffer);
void WriteToFile(const std::string& file_path, const std::string& write_buffer, std::ios::openmode open_mode = std::ios::trunc);
bool CreateDirectory(const std::string& dir_path); 
bool DirectoryExists(const std::string& dir_path);
bool FileExists(const std::string& file_path);