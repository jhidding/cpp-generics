#include <iterator>
#include <valarray>

namespace Numeric
{
    template <typename T, unsigned D>
    class NdIterator: public std::iterator<std::forward_iterator_tag, T>
    {
        std::valarray<T> &data;
        size_t flat_index;
        shape_t<D> index, shape, semi_stride;

        public:
            NdIterator(std::valarray<T> &data,
                    shape_t<D> const &shape):
                data(data), flat_index(0), shape(shape),
            {
                index.fill(0);
                calc_semi_stride(calc_stride(shape));
            }

            NdIterator(std::valarray<T> &data,
                    size_t offset,
                    shape_t<D> const &shape,
                    shape_t<D> const &stride):
                data(data), flat_index(offset), shape(shape)
            {
                calc_semi_stride(stride);
            }

            NdIterator &operator++()
            {
                ++index[0];
                flat_index += semi_stride[0];

                unsigned i = 0;
                while (index[i] == shape[i])
                {
                    ++i;
                    index[i-1] = 0;
                    ++index[i];
                    flat_index += semi_stride[i];
                }
            }

            T &operator*()
            {
                return data[flat_index];
            }

            bool operator!=(NdIterator const &other)
            {
                return flat_index != other.flat_index;
            }

        private:
            void calc_semi_stride(shape_t<D> const &stride)
            {
                semi_stride[0] = stride[0];
                for (unsigned i = 1; i < shape.size(); ++i)
                {
                    semi_stride[i] = stride[i] - shape[i-1] * stride[i-1];
                }
            }
    };
}

