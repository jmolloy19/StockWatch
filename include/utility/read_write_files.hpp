#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

void WriteToFile(const std::string& file_name, const std::string& write_buffer);
void ReadFromFile(const std::string& file_name, std::string* read_buffer);