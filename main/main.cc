#include <iostream>
#include <vector>

#include "command.hh"

using namespace System;

int main(int argc_, char **argv_)
{
    std::vector<std::string> argv(argv_, argv_ + argc_);
    if (argv.size() > 1)
    {
        if (Command::exists(argv[1]))
        {
            Command::at(argv[1])(argv);
            return 0;
        } 
    }

    Command::at("list")(argv);
    return 1;
}

