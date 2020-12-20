#include "stockwatch/stock_watch.h"

#include "glog/logging.h"

int main() {
    google::LogToStderr();
    google::InitGoogleLogging("StockWatch");

    sw::StockWatch program;
    program.Run();

    return 0;
}