#ifndef STOCKWATCH_CANDLE_H_
#define STOCKWATCH_CANDLE_H_

#include <chrono>
#include <vector>

namespace stockwatch {

struct Candle {
    Candle(float c, float o, float h, float l, int v, int64_t t)
        : close(c), open(o), high(h), low(l), volume(v), timestamp(t) {}

    float close{0};
    float open{0};
    float high{0};
    float low{0};
    int volume{0};
    int64_t timestamp{0};
};

float AverageClose(std::vector<Candle>::const_iterator first,
                   std::vector<Candle>::const_iterator last);
std::vector<Candle>::const_iterator MaxClose(std::vector<Candle>::const_iterator first,
                                             std::vector<Candle>::const_iterator last);
std::vector<Candle>::const_iterator MinClose(std::vector<Candle>::const_iterator first,
                                             std::vector<Candle>::const_iterator last);
float ClosesResidualStandardDeviation(std::vector<Candle>::const_iterator first,
                                      std::vector<Candle>::const_iterator last);

float AverageOpen(std::vector<Candle>::const_iterator first,
                  std::vector<Candle>::const_iterator last);
std::vector<Candle>::const_iterator MinOpen(std::vector<Candle>::const_iterator first,
                                            std::vector<Candle>::const_iterator last);
std::vector<Candle>::const_iterator MaxOpen(std::vector<Candle>::const_iterator first,
                                            std::vector<Candle>::const_iterator last);

float AverageHigh(std::vector<Candle>::const_iterator first,
                  std::vector<Candle>::const_iterator last);
std::vector<Candle>::const_iterator MinHigh(std::vector<Candle>::const_iterator first,
                                            std::vector<Candle>::const_iterator last);
std::vector<Candle>::const_iterator MaxHigh(std::vector<Candle>::const_iterator first,
                                            std::vector<Candle>::const_iterator last);

float AverageLow(std::vector<Candle>::const_iterator first,
                 std::vector<Candle>::const_iterator last);
std::vector<Candle>::const_iterator MinLow(std::vector<Candle>::const_iterator first,
                                           std::vector<Candle>::const_iterator last);
std::vector<Candle>::const_iterator MaxLow(std::vector<Candle>::const_iterator first,
                                           std::vector<Candle>::const_iterator last);

int AverageVolume(std::vector<Candle>::const_iterator first,
                  std::vector<Candle>::const_iterator last);
std::vector<Candle>::const_iterator MinVolume(std::vector<Candle>::const_iterator first,
                                              std::vector<Candle>::const_iterator last);
std::vector<Candle>::const_iterator MaxVolume(std::vector<Candle>::const_iterator first,
                                              std::vector<Candle>::const_iterator last);

std::vector<Candle>::const_iterator FirstCandleAfter(
    const std::vector<Candle>& candles, const std::chrono::system_clock::time_point& time_point);
std::vector<Candle>::const_iterator FirstCandleBefore(
    const std::vector<Candle>& candles, const std::chrono::system_clock::time_point& time_point);

}  // namespace stockwatch
#endif  // STOCKWATCH_CANDLE_H_
