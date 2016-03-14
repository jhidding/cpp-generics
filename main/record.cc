#include "command.hh"
#include "../src/misc/record-array.hh"
#include <iostream>
#include <array>

using record_array::RecordArray;

Command cmd_record("record",
    "test the record-array facility.",
    [] (std::vector<std::string> const &args)
{
    std::vector<uint8_t> data = {
        0, 1, 2, 3, 4, 5,
        6, 7, 8, 9, 10, 11 };

    RecordArray a;
    a.field<uint16_t>("x")
     .field<uint16_t>("y")
     .field<uint16_t>("z");

    a.load_bytes(data.begin(), data.end());

    for (auto const &item : a.as<std::array<float, 3>>())
    {
        std::cout << item[0] << " " << item[1] << " " << item[2] << "\n";
    }

    return 0;
});

