#include "stockwatch/stock_watch.h"

#include "glog/logging.h"

int main() {
    google::LogToStderr();
    google::InitGoogleLogging("StockWatch");

    stockwatch::StockWatch program;
    program.Run();

    return 0;
}