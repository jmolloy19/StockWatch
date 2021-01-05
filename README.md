# StockWatch

Scans common stocks on US exchange (NYSE and NASDAQ) for patterns and pushes results to a data base for future analysis.

## Dependencies

This project requires:
* Linux/MacOS Enviroment
* [curl](https://curl.haxx.se/download.html)
* [google/glog](https://github.com/google/glog)
* [CMake](https://cmake.org/download/) >= 3.14
* [Finnhub API Key](https://finnhub.io)

Note: Finnhub provides a free API key for unlimitied requests but with a limit of 60 calls per minute.

## Building
To build the project, run the following command from the project directory:

```bash
./build.sh
```

## Usage
```
Usage:
  StockWatch [OPTION...]

  -f, --finnhub_api_key arg  Finnhub API key. See https://finnhub.io/ for a
                             free api key.
  -v, --verbose arg          Verbosity level for googles verbose logging.
                             (default: 0)
  -w, --write_to_file        Write candles json to file after making
                             requests. Files are written to directory specified by
                             'json_dirs' option. Can run with 'read_from_file'
                             option after running program with this option. Used
                             for developement and testing.
  -r, --read_from_file       Read candles json from file insted of requesting
                             from finnhub. Files are read from directory
                             specified by 'json_dirs' option. Used for
                             developement and testing.
  -j, --jsons_dir arg        Directory to write/read candle data json files.
                             (default:
                             /home/jmolloy/Biometrics/StockWatch/jsons/)
  -n, --db_name arg          Name of database to save results to. (default:
                             stockwatch_sandbox)
  -u, --db_user arg          Postgres user. (default: jmolloy)
  -s, --db_password arg      Postgres password. (default: stockwatch)
  -a, --db_address arg       Address of database. (default: 0.0.0.0)
  -p, --db_port arg          Address port number. (default: 5432)
  -e, --email_password arg   Email password for emailing results.
  -h, --help                 Print usage menu.
```