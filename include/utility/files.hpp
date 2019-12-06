#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <utility/directory.hpp>

void ReadFromFile(const std::string& file_name, const std::string& dir_path, std::string* read_buffer);
void WriteToFile(const std::string& file_name, const std::string& dir_path, const std::string& write_buffer);
