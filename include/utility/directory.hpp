#pragma once
#include <sys/stat.h>
#include <string>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
// --Windows--
#include <direct.h>
bool CreateDirectory(const char* path); 
#else
// --Linux/MacOS--
bool CreateDirectory(const char* path); 
#endif

bool DirectoryExists(const std::string& directory);