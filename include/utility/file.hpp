#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <sys/stat.h>
#include <string>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
// --Windows--
#include <direct.h>
#endif


class File
{
    public:
        File() = delete;
        File(const std::string& file_name, const std::string& dir_path);
        void Read(std::string* read_buffer);
        void Write(const std::string& write_buffer, std::ios::openmode open_mode = std::ios::trunc);
    private:
        bool DirectoryExists();
        bool CreateDirectory();
        std::string file_name_;
        std::string dir_path_;
};
