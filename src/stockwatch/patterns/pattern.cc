#include "stockwatch/patterns/pattern.h"

#include "glog/logging.h"

namespace stockwatch {

std::string ToString(enum Pattern pattern) {
    switch (pattern) {
        case Pattern::kHighTightFlag:
            return "htf";
        default:
            LOG(FATAL) << "Unknown pattern enum: " << static_cast<int>(pattern);
    }
}
enum Pattern PatternFromString(const std::string& pattern) {
    if (pattern == "htf") {
        return Pattern::kHighTightFlag;
    }

    LOG(FATAL) << "Unknown pattern string: " << pattern;
}

}  // namespace stockwatch