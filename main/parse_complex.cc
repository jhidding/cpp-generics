#include "parse_complex.hh"
#include <iterator>
#include <iostream>
#include <boost/spirit/home/qi.hpp>

using namespace parse_complex;

template <typename Iterator>
bool do_parse_complex(Iterator begin, Iterator end, std::complex<double> &result)
{
    using boost::spirit::qi::double_;
    using boost::spirit::qi::phrase_parse;
    using boost::spirit::ascii::space;

    bool r = phrase_parse(begin, end,
           double_[ ([&result] (double a) { result.real(a); }) ] 
        >> double_[ ([&result] (double a) { result.imag(a); }) ] >> 'i', space);

    return r;
}

std::istream &parse_complex::operator>>(std::istream &in, std::complex<double> &result)
{
    // std::istreambuf_iterator<char> begin(in), end;
    // bool r = do_parse_complex(begin, end, result);
    std::string s;
    in >> s;
    bool r = do_parse_complex(s.begin(), s.end(), result);
    if (!r)
        in.setstate(std::ios_base::failbit);
    return in;
}

std::ostream &parse_complex::operator<<(std::ostream &out, std::complex<double> const &z)
{
    return out << z.real() << (z.imag() < 0 ? "" : "+") << z.imag() << "i";
}

