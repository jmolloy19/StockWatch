#ifndef STOCKWATCH_PATTERNS_HIGH_TIGHT_FLAG_H_
#define STOCKWATCH_PATTERNS_HIGH_TIGHT_FLAG_H_

#include <type_traits>

#include "patterns/pattern_detector.h"

namespace sw {

class HighTightFlag : public PatternDetector {
   public:
    HighTightFlag() = default;
    ~HighTightFlag() = default;

    static bool ExhibitsPattern(const Candles& candles);

    protected:
    static google::protobuf::RepeatedField<float>::const_iterator FlagPoleBottom(const Candles& candles);
    static google::protobuf::RepeatedField<float>::const_iterator FlagPoleTop(const Candles& candles);

    private:
    static constexpr int kMinAverageVolume = 100000;
    static constexpr float kMinAverageClose = 1.0;
};

}  // namespace sw
#endif  // STOCKWATCH_PATTERNS_HIGH_TIGHT_FLAG_H_