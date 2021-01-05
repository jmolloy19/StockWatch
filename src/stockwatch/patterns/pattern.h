#ifndef STOCKWATCH_PATTERNS_PATTERN_H_
#define STOCKWATCH_PATTERNS_PATTERN_H_

#include <string>

namespace stockwatch {

enum class Pattern {
    kHighTightFlag,
};

std::string ToString(enum Pattern type);
enum Pattern PatternFromString(const std::string& type);

}  // namespace stockwatch
#endif  // STOCKWATCH_PATTERNS_PATTERN_H_
