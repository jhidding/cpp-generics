#include "../src/misc/global.hh"
#include <functional>
#include <vector>
#include <string>

using System::Global;

class Command_: public std::function<int (std::vector<std::string> const &)>
{
    public:
        std::string description;

        typedef std::function<int (std::vector<std::string> const &)> base_type;

        using base_type::base_type;

        template <typename Fn>
        Command_(std::string const &description, Fn fn):
            base_type(fn), description(description) {}
};

using Command = Global<Command_>;

