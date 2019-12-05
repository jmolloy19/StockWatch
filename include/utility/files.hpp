#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <utility/directory.hpp>

void ReadFromFile(const std::string& stock_name, std::string* read_buffer);
void WriteToFile(const std::string& stock_name, const std::string& write_buffer);
