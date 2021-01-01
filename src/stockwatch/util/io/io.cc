#include "io.h"

#include <fstream>

#include "glog/logging.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"

namespace stockwatch {
namespace util {
namespace io {

bool ReadFromFile(const std::experimental::filesystem::path& path, std::string* contents) {
    std::ifstream in_file(path, std::ios::ate);

    if (not in_file.is_open() or not in_file.good()) {
        LOG(FATAL) << "Failed opening file to read from: " << path;
        return false;
    }

    size_t size = in_file.tellg();
    in_file.seekg(0);

    contents->clear();
    contents->resize(size);
    in_file.read(contents->data(), size);

    return true;
}

bool WriteToFile(const std::experimental::filesystem::path& path, const std::string& contents) {
    if (not std::experimental::filesystem::exists(path.parent_path())) {
        LOG_IF(FATAL, not std::experimental::filesystem::create_directories(path.parent_path())) << "Failed creating parent directoris for path: " << path;
    }

    std::ofstream out_file(path);

    if (not out_file.is_open() or not out_file.good()) {
        LOG(FATAL) << "Failed opening file to write to: " << path << ": good=" << out_file.good();
        return false;
    }

    out_file.write(contents.c_str(), contents.size());

    return true;
}

bool ReadFromFile(const std::experimental::filesystem::path& path, rapidjson::Document* document) {
    std::ifstream in_file(path);

    if (not in_file.is_open() or not in_file.good()) {
        LOG(FATAL) << "Failed opening file to read from: " << path;
        return false;
    }

    rapidjson::IStreamWrapper in_stream_wrapper(in_file);

    document->ParseStream(in_stream_wrapper);

    return true;
}

bool WriteToFile(const std::experimental::filesystem::path& path, const rapidjson::Document& document) {
    if (not std::experimental::filesystem::exists(path.parent_path())) {
        LOG_IF(FATAL, not std::experimental::filesystem::create_directories(path.parent_path())) << "Failed creating parent directoris for path: " << path;
    }

    std::ofstream out_file(path);

    if (not out_file.is_open() or not out_file.good()) {
        LOG(FATAL) << "Failed opening file to write to: " << path;
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
