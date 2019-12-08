#include <utility/progress_bar.hpp>
#include <chrono>
#include <thread>

int main()
{
    for(int i = 0; i < 100; i++)
    {
        if(i % 5 == 0)
        {
            DisplayProgressBar(i, 100);
            std::this_thread::sleep_for (std::chrono::seconds(1));
        }
    }
}