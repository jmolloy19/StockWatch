#include "cxxopts/cxxopts.h"
#include "glog/logging.h"

#include "stockwatch/stock_watch.h"

static cxxopts::Options BuildOptions() {
    cxxopts::Options options("StockWatch",
                             "Scans common stocks on US exchange (NYSE and NASDAQ) for patterns.");

    options.add_options()("f, finnhub_api_key",
                          "Finnhub API key. See https://finnhub.io for a free api key.",
                          cxxopts::value<std::string>()->default_value(""));
    options.add_options()("v, verbose", "Verbosity level for googles verbose logging.",
                          cxxopts::value<int>()->default_value("0"));
    options.add_options()(
        "w, write_to_file",
        "Write candles json to file after making requests. Files are written to directory "
        "specified by \'json_dirs\' option. "
        "Can run with \'read_from_file\' option "
        "after running program with this option. Used for developement and testing.",
        cxxopts::value<bool>()->default_value("false")->implicit_value("true"));
    options.add_options()("r, read_from_file",
                          "Read candles json from file insted of requesting from finnhub. Files "
                          "are read from directory specified by \'json_dirs\' option. Used for "
                          "developement and testing.",
                          cxxopts::value<bool>()->default_value("false")->implicit_value("true"));
    options.add_options()(
        "j, jsons_dir", "Directory to write/read candle data json files.",
        cxxopts::value<std::experimental::filesystem::path>()->default_value(STOCKWATCH_JSONS_DIR));
    options.add_options()("n, db_name", "Name of database to save results to.",
                          cxxopts::value<std::string>()->default_value("stockwatch_sandbox"));
    options.add_options()("u, db_user", "Postgres user.",
                          cxxopts::value<std::string>()->default_value("jmolloy"));
    options.add_options()("s, db_password", "Postgres password.",
                          cxxopts::value<std::string>()->default_value("stockwatch"));
    options.add_options()("a, db_address", "Address of database.",
                          cxxopts::value<std::string>()->default_value("0.0.0.0"));
    options.add_options()("p, db_port", "Address port number.",
                          cxxopts::value<std::string>()->default_value("5432"));
    options.add_options()("e, email_password", "Email password for emailing results.",
                          cxxopts::value<std::string>()->default_value("password_not_given_in_args"));
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

    if (args["finnhub_api_key"].as<std::string>().empty() and not args["read_from_file"].as<bool>()) {
        std::cerr << "Missing required argument: api_key\n" << options.help();
        exit(0);
    }

    if (args["write_to_file"].as<bool>() and args["read_from_file"].as<bool>()) {
        std::cerr << "Invalid combination of options. Both \'read_from_file\' and "
                     "\'write_to_file\' options set to true. "
                     "Only one or the other can be set to true.";
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
