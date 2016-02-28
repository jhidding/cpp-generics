#include "parse_string.hh"
#include "grammar.hh"

using namespace Json;

ptr Json::parse_string(std::string const &s)
{
    return parse(s.begin(), s.end());
}

