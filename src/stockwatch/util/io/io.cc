#include "io.h"

#include <fstream>

#include "glog/logging.h"

namespace stockwatch {
namespace util {
namespace io {

bool ReadFromFile(const std::string& filename, std::string* contents) {
    std::ifstream in_file(filename, std::ios::ate);

    if (not in_file.is_open() or not in_file.good()) {
        LOG(ERROR) << "Failed opening file to read_from: " << filename << ": good=" << in_file.good();
        return false;
    }

    size_t size = in_file.tellg();
    in_file.seekg(0);

    contents->resize(size);
    in_file.read(contents->data(), size);

    return true;;
}

bool WriteToFile(const std::string& filename, const std::string& contents) {
    std::ofstream out_file(filename);

    if (not out_file.is_open() or not out_file.good()) {
        LOG(ERROR) << "Failed opening file to write to: " << filename << ": good=" << out_file.good();
        return false;
    }

    out_file.write(contents.c_str(), contents.size());

    return true;;
}


}  // namespace io
}  // namespace util
}  // namespace stockwatch
