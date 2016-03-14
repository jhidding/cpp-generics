#include "command.hh"
#include "argparse.hh"
#include "parse_complex.hh"

#include "../src/misc/iterators.hh"
#include "../src/numeric/slice.hh"

#include <complex>
#include <iostream>
#include <cstdint>
#include <functional>

using namespace argparse;
using parse_complex::operator<<;
using parse_complex::operator>>;
using System::tail;

namespace numeric
{

    template <typename T, unsigned D>
    class NdArray
    {
        std::vector<T> data_;
        shape_t<D> shape_;
        stride_t<D> stride_;

        public:
            NdArray(shape_t<D> const &shape):
                data_(calc_size<D>(shape)),
                shape_(shape),
                stride_(calc_stride<D>(shape))
            {}

            T &operator[](size_t i)
            {
                return data_[i];
            }

            T const &operator[](size_t i) const
            {
                return data_[i];
            }

            Slice<D> slice() const
            {
                return Slice<D>(shape_);
            }

            shape_t<D> shape() const
            {
                return shape_;
            }
    };

    template <typename T, typename E>
    class Expression: public std::iterator<std::input_iterator_tag, T>
    {
        public:
            T const &operator*() const
                { return *static_cast<E const &>(*this); }

            operator E &() 
                { return static_cast<E &>(*this); }

            operator E const &() 
                { return static_cast<E const &>(*this); }
    };

}


using numeric::shape_t;

std::function<std::complex<double>(shape_t<2> const &)> grid_function(
    std::complex<double> const &z, 
    double zoom, shape_t<2> const &shape)
{
    double res = 1.0 / (zoom * std::min(shape[0], shape[1]));
    std::complex<double> a(
        z.real() - res/2 * shape[0],
        z.imag() - res/2 * shape[1]);

    return [res, a] (shape_t<2> const &p) {
        return a + std::complex<double>(p[0] * res, p[1] * res);
    };
}

namespace julia
{
    using complex = std::complex<double>;
    using function = std::function<complex (complex)>;
    using predicate = std::function<bool (complex)>;
    using unit_map = std::function<double (complex)>;

    function f(complex c) {
        return [c] (complex z) {
            return z*z + c;
        };
    }

    bool pred(complex z) {
        return std::norm(z) < 4.0;
    }

    int iterate(function f, predicate pred, complex z, unsigned maxit) {
        unsigned i = 0;

        while (pred(z) && i < maxit) {
            z = f(z);
            ++i;
        }

        return i;
    }

    unit_map julia_c(complex c, int maxit) {
        return [c, maxit] (complex z) {
            return sqrt(double(iterate(f(c), pred, z, maxit)) / maxit);
        };
    }

    void write_pgm(numeric::NdArray<double, 2> const &image)
    {
        auto shape = image.shape();
        size_t idx = 0;

        std::cout << "P2\n" << shape[0] << " " << shape[1] << "\n255\n";
        for (unsigned j = 0; j < shape[1]; ++j)
        {
            for (unsigned i = 0; i < shape[0]; ++i, ++idx)
            {
                std::cout << static_cast<unsigned>(image[idx]*255) << " ";
            }
            std::cout << "\n";
        }
    }

    void draw(
        complex const &c, complex const &centre,
        unsigned long max_iter, double zoom, shape_t<2> const &shape)
    {
        auto grid = grid_function(centre, zoom, shape);

        numeric::NdArray<double, 2> image(shape);
        numeric::Slice<2> s(shape);

        auto fn = julia_c(c, max_iter);
        for (auto p = s.begin(); p != s.end(); ++p)
        {
            std::complex<double> z = grid(*p);
            image[p.flat_index()] = fn(z);
            // (z*std::conj(z)).real(); // fn(z);
        }

        write_pgm(image);
    }
}

Command cmd_julia("julia",
    "Render a Julia fractal.",
    [] (std::vector<std::string> const &args_)
{
    Args args = {
        Option("-h", "print usage."),
        Option("-c", "complex coordinate of Julia set.", "0+0i"),
        Option("-z", "centre of image.", "0+0i"),
        Option("-x", "zoom factor.", "1"),
        Option("-n", "max-iter", "256"),
        Option("-res", "resolution of image.", "1920x1080")};
    args.parse(tail(tail(args_)));

    auto c = args.get<std::complex<double>>("-c");
    auto z = args.get<std::complex<double>>("-z");
    auto x = args.get<double>("-x");
    auto n = args.get<unsigned long>("-n");

    if (!c || !z || !x || !n)
    {
        std::cout << "trouble parsing one of the arguments.\n";
        return 1;
    }

    julia::draw(*c, *z, *n, *x, {1920, 1080});

    return 0;
});

