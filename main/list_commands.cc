#include "../src/misc.hh"
#include "command.hh"

using namespace System;

using console::fg;
using console::reset;
using fancy::LongString;

Command list_commands("list",
    "Give a list of the different commands contained in this little program.",
    [] (std::vector<std::string> const &args) -> int
{
    std::cout << "Spirit test ground, list of commands:\n";
    for (auto const &p : Command::dir())
    {
        int line_no = 0;
        auto left_side = [&line_no, &p] () -> fancy::ptr
        {
            switch (line_no)
            {
                case 0: ++line_no;
                    return fancy::compose(
                        fg(colour::fluxom_lime), p.first, 
                        fg(colour::dark_gray), ":  ", reset());

                default:
                    return fancy::compose(
                        fg(colour::dark_gray), "  ┃ ", reset());
            }
        };

        std::cout << LongString(p.second->description, 60, left_side) << std::endl;
    }

    return 0;
});

