#include "command.hh"
#include <array>
#include <string>
#include <vector>
#include <iostream>

class vector: public std::array<double, 3>
{
    public:
        vector(double x, double y, double z):
            std::array<double, 3>({x, y, z}) {}

        double operator()(vector const &other)
        {
            return at(0)*other[0] + at(1)*other[1] + at(2)*other[2];
        }
};

vector &operator+=(vector &a, vector const &b)
{
    a[0] += b[0];
    a[1] += b[1];
    a[2] += b[2];
    return a;
}

Command operator_test(
    "operator",
    "with my pocket calculator, play with operator overloading syntax",
    [] (std::vector<std::string> const &args)
{
    vector a(1, 2, 3);
    vector b(0, 1, -1);

    a += b;

    std::cout << a(b) << std::endl;

    return 0;
});

