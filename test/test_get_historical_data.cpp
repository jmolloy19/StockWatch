#include <stockwatch/stock.hpp>

int main()
{
	curl_global_init(CURL_GLOBAL_ALL);

    Stock stock("AAPL");
    std::cout << stock;

	curl_global_cleanup();
}   