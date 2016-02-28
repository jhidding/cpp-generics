#include "command.hh"
#include "../src/misc.hh"
#include "../src/parsers/json.hh"

using System::read_lines;
using System::console::fg;
using System::console::reset;

namespace console = System::console;

Command json_parser("json",
    "Parse JSON lines and pretty print output.",
    [] (std::vector<std::string> const &) -> int
{
    std::cout << "Enter JSON lines:\n";
    std::cout << console::fg(90, 90, 90) << "≫  "
              << console::fg(210, 210, 180);
    for (std::string const &line : read_lines(std::cin))
    {
        Json::ptr r;
        try {
            r = Json::parse_string(line);
        } catch (char const *s) {
            std::cerr << fg(250, 100, 70) << s << reset() << std::endl;
            return 1;
        }
        std::cout << reset();
        if (r)
        {
            std::cout << console::fg(80, 200, 80) << "<matched> " 
                      << console::reset() << std::endl;
            std::cout << Json::pretty_print(r) << std::endl;
        } 
        else
        {
            std::cout << console::fg(200, 80, 80) << "<failed>" 
                      << console::reset() << std::endl;
        }
        std::cout << console::fg(90, 90, 90) << "≫  "
                  << console::fg(210, 210, 180);
    }
    std::cout << " Bye!" << console::reset() << std::endl;
    return 0;
});

