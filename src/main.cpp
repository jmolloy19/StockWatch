#include "stockwatch/stockwatch.h"

#include "glog/logging.h"

int main(int argc, char* argv[]) {
    google::LogToStderr();
    google::InitGoogleLogging("StockWatch");

    sw::StockWatch program;
    program.Init();
    program.Run();
    program.PrintResults();
}