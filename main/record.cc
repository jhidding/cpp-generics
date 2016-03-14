#include "command.hh"
#include "../src/misc/record-array.hh"

Command cmd_record("record",
    "test the record-array facility.",
    [] (std::vector<std::string> const &args)
{
    return 0;
});

