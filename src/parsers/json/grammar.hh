#pragma once
#include "tree_maker.hh"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace Json
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    template <typename Iterator>
    ptr parse(Iterator begin, Iterator end)
    {
        TreeMaker tm;

        qi::rule<Iterator, ascii::space_type> 
            value_expr, object_expr, list_expr, 
            integer_expr, float_expr, string_expr,
            boolean_expr, null_expr;

        qi::rule<Iterator, std::string(), ascii::space_type> quoted_string;
        qi::real_parser<double, qi::strict_real_policies<double>> strict_double;
        
        quoted_string = qi::lexeme['"' >> +(qi::char_ - '"') >> '"'];

        list_expr = qi::char_('[')          [([&tm] () { tm.new_list(); })]
            >> (value_expr % ',')
            >> qi::char_(']')               [([&tm] () { tm.close(); })];

        object_expr = qi::char_('{')        [([&tm] () { tm.new_object(); })]
            >> (quoted_string               [([&tm] (std::string const &key) 
                                                { tm.set_key(key); })]
            >> ':' >> value_expr) % ','
            >> qi::char_('}')               [([&tm] () { tm.close(); })];

        integer_expr = qi::int_             [([&tm] (int a) 
                                                { tm.add_atom<int>(a); })];
        float_expr = strict_double          [([&tm] (double a) 
                                                { tm.add_atom<double>(a); })];

        boolean_expr = qi::lit("true")      [([&tm] () { tm.add_atom<bool>(true); })]
                     | qi::lit("false")     [([&tm] () { tm.add_atom<bool>(false); })];

        null_expr = qi::lit("null")         [([&tm] () { tm.add_value(ptr()); })];

        string_expr = quoted_string         [([&tm] (std::string const &s) 
                                                { tm.add_atom<std::string>(s); })];

        value_expr = list_expr
                   | object_expr
                   | float_expr
                   | integer_expr
                   | string_expr
                   | boolean_expr
                   | null_expr;

        bool r = qi::phrase_parse(begin, end, value_expr, ascii::space);
        if (r && begin == end && tm.done())
        {
            return std::move(tm.result());
        }
        else
        {
            return ptr();
        }
    }
}

