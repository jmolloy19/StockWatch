#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <sys/stat.h>
#include <libgen.h>

void ReadFromFile(const char* file_path, std::string* read_buffer);
void WriteToFile(const char* file_path, const std::string& write_buffer, std::ios::openmode open_mode = std::ios::trunc);
bool CreateDirectory(const char* dir_path); 
bool DirectoryExists(const char* dir_path);