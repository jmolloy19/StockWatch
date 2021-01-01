#include <iostream>

#include "cxxopts/cxxopts.h"
#include "glog/logging.h"

#include "stockwatch/stock_watch.h"

static cxxopts::Options BuildOptions() {
    cxxopts::Options options("StockWatch", "Scans US exchange for chart patterns.");

    options.add_options()("f, finnhub_api_key", "Finnhub API key. See https://finnhub.io/ for a free api key.",
                          cxxopts::value<std::string>());
    options.add_options()("v, verbose", "Verbosity level for googles verbose logging.",
                          cxxopts::value<int>()->default_value("0"));
    options.add_options()("w, write_to_file",
                          "Write candle jsons to \"<stockwatch_project_dir>/Build/jsons/\" after making requests. Can run with \'read_from_file\' option "
                          "after running program with this option. Used for developement and testing.",
                          cxxopts::value<bool>()->default_value("false")->implicit_value("true"));
    options.add_options()("r, read_from_file",
                          "Read candles from json files insted of making requests. Used for developement and testing.",
                          cxxopts::value<bool>()->default_value("false")->implicit_value("true"));
    options.add_options()("n, db_name", "Name of database to save results to.",
                          cxxopts::value<std::string>()->default_value("stockwatch_sandbox"));
    options.add_options()("u, db_user", "Postgres user.", cxxopts::value<std::string>()->default_value("jmolloy"));
    options.add_options()("s, db_password", "Postgres password.",
                          cxxopts::value<std::string>()->default_value("stockwatch"));
    options.add_options()("a, db_address", "Address of database.",
                          cxxopts::value<std::string>()->default_value("0.0.0.0"));
    options.add_options()("p, db_port", "Address port number.", cxxopts::value<std::string>()->default_value("5432"));
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

    if (args.count("finnhub_api_key") != 1) {
        std::cerr << "Missing required argument: api_key\n" << options.help();
        exit(0);
    }

    return args;
}

static void InitLogging(const cxxopts::ParseResult& args) {
    FLAGS_v = args["verbose"].as<int>();
    google::InitGoogleLogging("StockWatch");
    google::LogToStderr();
    google::InstallFailureSignalHandler();
}

int main(int argc, char** argv) {
    cxxopts::ParseResult args = ParseArgs(argc, argv);

    InitLogging(args);

    stockwatch::Config config(args);
    stockwatch::StockWatch program(config);
    program.Run();

    return 0;
}
