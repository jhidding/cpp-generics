#pragma once
#include <complex>
#include <iostream>

namespace parse_complex
{
    extern std::istream &operator>>(std::istream &in, std::complex<double> &result);
    extern std::ostream &operator<<(std::ostream &out, std::complex<double> const &z);
}

