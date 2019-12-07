#include <utility/progress_bar.hpp>

void DisplayProgressBar(int complete, int total_amount)
{
    double percent_done = ((double)complete / (double)total_amount) * 100.0;
    uint num_bars_to_print = percent_done * 40;
    uint num_spaces_to_print = 40 - num_bars_to_print;

    std::string bars(num_bars_to_print, '=');
    std::string spaces(num_spaces_to_print, ' ');

    std::cout << std::fixed << std::setprecision(2)
              << '|' << bars << '>' << spaces << "| %" << percent_done << '\r'
              << std::flush;
}