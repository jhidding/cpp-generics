#include "command.hh"
#include "../src/misc.hh"
#include <iostream>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct roman: qi::grammar<Iterator, unsigned()>
{
    qi::rule<Iterator, unsigned()> start;
    qi::symbols<char,unsigned> hundreds, tens, ones;

    roman(): roman::base_type(start)
    {
        hundreds.add
            ("C", 100)("CC", 200)("CCC", 300)
            ("CD", 400)("D", 500)("DC", 600)
            ("DCC", 700)("DCCC", 800)("CM", 900);

        tens.add
            ("X", 10)("XX", 20)("XXX", 30)
            ("XL", 40)("L", 50)("LX", 60)
            ("LXX", 70)("LXXX", 80)("XC", 90);
        
        ones.add
            ("I", 1)("II", 2)("III", 3)
            ("IV", 4)("V", 5)("VI", 6)
            ("VII", 7)("VIII", 8)("IX", 9);

        start = qi::eps   [qi::_val = 0] >> (
            +qi::lit('M') [qi::_val += 1000]
            || hundreds   [qi::_val += qi::_1]
            || tens       [qi::_val += qi::_1]
            || ones       [qi::_val += qi::_1]);
    }
};


using System::read_lines;

Command roman_numbers("roman",
    "Parse Roman numbers.",
    [] (std::vector<std::string> const &) -> int
{
    std::cout << "Enter Roman numerals:\n≫ ";
    for (std::string const &line : read_lines(std::cin))
    {
        roman<std::string::const_iterator> roman_parser;
        unsigned result;
        auto begin = line.cbegin(), end = line.cend();
        bool r = qi::parse(begin, end, roman_parser, result);

        if (r && begin == end)
        {
            std::cout << "<matched> " << result << std::endl;
        } 
        else
        {
            std::string rest(begin, end);
            std::cout << "<failed> stopped at '" << rest << "'" << std::endl;
        }
        std::cout << "≫ ";
    }
    std::cout << " Bye!" << std::endl;
    return 0;
});

