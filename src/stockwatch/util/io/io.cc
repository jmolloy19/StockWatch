#include "io.h"

#include <fstream>

#include "glog/logging.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"

namespace stockwatch {
namespace util {
namespace io {

bool ReadFromFile(const std::string& filename, std::string* contents) {
    contents->clear();
    std::ifstream in_file(filename, std::ios::ate);

    if (not in_file.is_open() or not in_file.good()) {
        LOG(ERROR) << "Failed opening file to read_from: " << filename << ": good=" << in_file.good();
        return false;
    }

    size_t size = in_file.tellg();
    in_file.seekg(0);

    contents->resize(size);
    in_file.read(contents->data(), size);

    return true;
}

bool WriteToFile(const std::string& filename, const std::string& contents) {
    std::ofstream out_file(filename);

    if (not out_file.is_open() or not out_file.good()) {
        LOG(ERROR) << "Failed opening file to write to: " << filename << ": good=" << out_file.good();
        return false;
    }

    out_file.write(contents.c_str(), contents.size());

    return true;
}

bool ReadFromFile(const std::string& filename, rapidjson::Document* document) {
    std::ifstream in_file(filename);

    if (not in_file.is_open() or not in_file.good()) {
        LOG(ERROR) << "Failed opening file to read_from: " << filename << ": good=" << in_file.good();
        return false;
    }

    rapidjson::IStreamWrapper in_stream_wrapper(in_file);

    document->ParseStream(in_stream_wrapper);

    return true;
}

bool WriteToFile(const std::string& filename, const rapidjson::Document& document) {
    std::ofstream out_file(filename);

    if (not out_file.is_open() or not out_file.good()) {
        LOG(ERROR) << "Failed opening file to write to: " << filename << ": good=" << out_file.good();
        return false;
    }

    rapidjson::OStreamWrapper out_stream_wrapper(out_file);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(out_stream_wrapper);

    document.Accept(writer);

    return true;
}

}  // namespace io
}  // namespace util
}  // namespace stockwatch
