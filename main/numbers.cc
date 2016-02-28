#include "../src/misc.hh"

#include "command.hh"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

using System::console::fg;
using System::console::reset;
using System::fancy::LongString;

using namespace System;

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

namespace Numeric 
{
    class double_parser
    {
        public:
            typedef std::shared_ptr<std::vector<double>> return_type;

            template <typename R>
            return_type operator()(R const &range) const
            {
                std::vector<double> p;

                auto begin = range.cbegin(), end = range.cend();
                bool r = qi::phrase_parse(begin, end,
                    qi::double_ % qi::eps, ascii::space, p);

                if (begin != end)
                    return nullptr;

                if (r)
                    return std::make_shared<std::vector<double>>(p);
                else
                    return nullptr;
            }
    };

    class value 
    {
        public:
            virtual ~value() {}
    };

    template <typename T>
    class number: public value
    {
        T data;
        public:
            number(T data): data(data) {}
            T get() const { return data; }
    };

    template <typename T>
    inline std::unique_ptr<value> make_number(T data)
    {
        return std::make_unique<number<T>>(data);
    }

    using value_list = std::vector<boost::variant<double, int>>;

    template <typename Iterator>
    class number_parser: public qi::grammar<Iterator, value_list(), ascii::space_type>
    {
        qi::rule<Iterator, value_list(), ascii::space_type> list;
        qi::real_parser<double, qi::strict_real_policies<double>> strict_double;

        public:
            number_parser():
                number_parser::base_type(list)
            {
                list = //qi::eps         [qi::_val = value_list()] >> 
                         ( strict_double    
                         | qi::int_ 
                         ) % ',';
            }
    };
}

Command test_numbers("numbers",
    "Demo parsing a list of floating point numbers. The numbers are entered on "
    "the command line and printed back when the parsing is successful.",
    [] (std::vector<std::string> const &args) -> int
{
    std::cout << "Spirit QI example, running with: " << head(args);
    for (auto const &arg : tail(args))
        std::cout << " `" << fg(255, 180, 180) << arg
                  << reset() << "'";
    std::cout << std::endl;

    Numeric::number_parser<std::string::const_iterator> parser;
    for (std::string const &s : read_lines(std::cin))
    {
        Numeric::value_list result;
        auto begin = s.cbegin(), end = s.cend();
        auto r = qi::phrase_parse(begin, end, parser, ascii::space, result);
        if (r && begin == end)
        {
            std::cout << fg(colour::fluxom_lime) << "<parser success> : ";
            for (auto const &v : result) 
            {
                //auto pi = dynamic_cast<Numeric::number<int> *>(v.get());
                if (int const *pi = boost::get<int>(&v))
                //if (pi)
                    std::cout << fg(255, 255, 180) << *pi << ' ';

                //auto pd = dynamic_cast<Numeric::number<double> *>(v.get());
                if (double const *pd = boost::get<double>(&v))
                //if (pd)
                    std::cout << fg(255, 180, 255) << *pd << ' ';
            }
            std::cout << reset() << std::endl;
        } else {
            std::cout << fg(255, 180, 180) << "<parsing failed>";
            std::cout << reset() << std::endl;
        }
    }

    return 0; 
});

