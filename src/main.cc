#include "stockwatch/stock_watch.h"

#include <iostream>

#include "cxxopts/cxxopts.h"
#include "glog/logging.h"

static cxxopts::Options BuildOptions() {
    cxxopts::Options options("StockWatch", "Scans US exchange for chart patterns.");

    options.allow_unrecognised_options();

    options.add_options()("k, key", "Finnhub API key. See https://finnhub.io/ for a free api key.",
                          cxxopts::value<std::string>());
    options.add_options()("v, verbose", "Verbosity level for verbose logging.",
                          cxxopts::value<int>()->default_value("0"));
    options.add_options()("h, help", "Print usage menu.");
    return options;
}

static cxxopts::ParseResult ParseArgs(int argc, char** argv) {
    cxxopts::Options options = BuildOptions();
    cxxopts::ParseResult args;

    try {
        args = options.parse(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n" << options.help();
        exit(0);
    }

    if (args.count("help") == 1) {
        std::cerr << options.help();
        exit(0);
    }

    if (args.count("key") != 1) {
        std::cerr << "Missing required argument: key\n" << options.help();
        exit(0);
    }

    return args;
}

static void InitLogging(const cxxopts::ParseResult& args) {
    google::InitGoogleLogging("StockWatch");
    google::LogToStderr();
    google::InstallFailureSignalHandler();
    FLAGS_v = args["verbose"].as<int>();
}

int main(int argc, char** argv) {
    cxxopts::ParseResult args = ParseArgs(argc, argv);

    InitLogging(args);

    stockwatch::StockWatch program(args["key"].as<std::string>());
    program.Run();

    LOG(INFO) << "StockWatch finished successfully!";
    return 0;
}
