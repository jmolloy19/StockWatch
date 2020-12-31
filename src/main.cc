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
    options.add_options()("r, read_from_file", "Read candles from json files insted of making requests. Used for developement and teesting.",
                          cxxopts::value<bool>()->default_value("false")->implicit_value("true"));
    options.add_options()("w, write_to_file", "Write candles to json files after making requests. Can run with \'read_from_file\' option after running program with this option.",
                          cxxopts::value<bool>()->default_value("false")->implicit_value("true"));
    options.add_options()("h, help", "Print usage menu.");
    return options;
}

static void InitLogging(const cxxopts::ParseResult& args) {
    google::LogToStderr();
    google::InstallFailureSignalHandler();
    FLAGS_v = args["verbose"].as<int>();
    google::InitGoogleLogging("StockWatch");
}

static stockwatch::Options ParseArgs(int argc, char** argv) {
    cxxopts::Options options = BuildOptions();
    cxxopts::ParseResult args;
    stockwatch::Options stockwatch_options;

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

    InitLogging(args);

    stockwatch_options.api_key = args["key"].as<std::string>();

    if (args.count("read_from_file") == 1) {
        stockwatch_options.read_from_file = args["read_from_file"].as<bool>();
    }

    if (args.count("write_to_file") == 1) {
        stockwatch_options.read_from_file = args["write_to_file"].as<bool>();
    }

    return stockwatch_options;
}

int main(int argc, char** argv) {
    stockwatch::Options options = ParseArgs(argc, argv);

    stockwatch::StockWatch program(options);
    program.Run();

    return 0;
}
