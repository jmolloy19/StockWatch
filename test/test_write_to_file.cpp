#include <stockwatch/stockwatch.hpp>

int main(int argc, char* argv[])
{
	curl_global_init(CURL_GLOBAL_ALL);

    Stock stock("AAPL", false, true);

	curl_global_cleanup();
}   