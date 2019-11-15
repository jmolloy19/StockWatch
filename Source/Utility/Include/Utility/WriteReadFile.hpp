#pragma once
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <string>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
//Windows
#include <direct.h>
const std::string DATAFILES_PATH = ".\\DataFiles\\"; 
bool createDirectory(const std::string& path); 
#else
const std::string DATAFILES_PATH = "./DataFiles/";
bool createDirectory(const std::string& path, mode_t mode); 
#endif

bool directoryExists(const std::string& dir);
bool writeToFile(const std::string& filename, const std::string& writeBuffer);
bool readFromFile(const std::string& filename, std::string* readBuffer);