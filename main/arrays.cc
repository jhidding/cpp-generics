//#include "command.hh"
//
//#include <valarray>
//#include <random>
//#include <memory>
//#include <iostream>
//#include <algorithm>
//
//#include "../src/numeric/nd_array.hh"
//
//namespace Numeric
//{
//    std::function<double()> normal_values(double mean = 0.0, double std_dev = 1.0, unsigned long seed = 0)
//    {
//        auto generator = std::make_shared<std::mt19937_64>(seed);
//        auto distribution = std::make_shared<std::normal_distribution<double>>(mean, std_dev);
//
//        return [generator, distribution] () -> double
//        {
//            return (*distribution)(*generator);
//        };
//    }
//
//    template <unsigned D>
//    class Slice
//    {
//        public:
//            size_t offset;
//            shape_t<D> shape, stride;
//
//            Slice(shape_t<D> const &shape):
//                offset(0),
//                shape(shape),
//                stride(calc_stride(shape))
//            {}
//
//            Slice(size_t offset, shape_t<D> const &shape, shape_t<D> const &stride):
//                offset(offset),
//                shape(shape),
//                stride(stride)
//            {}
//
//            size_t flat_index(shape_t<D> const &x)
//            {
//                size_t i = offset;
//                for (unsigned i = 0; i < x.size(); ++i)
//                    i += x[i] * stride[i];
//                return i;
//            }
//    };
//
//    template <typename T>
//    void print_array_2d(T const &data, std::valarray<size_t> const &shape)
//    {
//        if (shape.size() != 2)
//            throw "can only print 2d arrays.";
//
//        auto v = std::begin(data);
//        for (size_t j = 0; j < shape[1]; ++j)
//        {
//            for (size_t i = 0; i < shape[0]; ++i, ++v)
//            {
//                std::cout << *v << " ";
//            }
//            std::cout << "\n";
//        }
//    }
//}
//
//Command test_arrays("arrays",
//    "Experiment with std::valarray and std::gslice to get NdArray "
//    "functionality, similar to NumPy.",
//    [] (std::vector<std::string> const &args) -> int
//{
//    std::valarray<size_t>
//        shape = {256, 512},
//        stride(Numeric::calc_stride(shape));
//
//    size_t size = Numeric::calc_size(shape);
//
//    std::valarray<double> data(size);
//    std::generate(std::begin(data), std::end(data), Numeric::normal_values());
//
//    auto data_slice = data[std::gslice(0, {256, 256}, {1, 512})];
//    std::valarray<double>(data_slice).apply(exp);
//
//    Numeric::print_array_2d(data, shape);
//    
//    return 0;
//});
//
