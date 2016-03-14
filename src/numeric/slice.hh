#include "ndrange.hh"

namespace numeric
{
    template <unsigned D>
    class Slice
    {
        public:
            size_t offset, size;
            shape_t<D> shape;
            stride_t<D> stride;

            Slice(shape_t<D> const &shape):
                offset(0), size(calc_size<D>(shape)),
                shape(shape),
                stride(calc_stride<D>(shape))
            {}

            Slice(size_t offset, shape_t<D> const &shape, shape_t<D> const &stride):
                offset(offset), size(calc_size<D>(shape)),
                shape(shape),
                stride(stride)
            {}

            size_t flat_index(shape_t<D> const &x)
            {
                size_t i = offset;
                for (unsigned i = 0; i < x.size(); ++i)
                    i += x[i] * stride[i];
                return i;
            }

            NdRange<D> begin() const
            {
                return NdRange<D>(offset, shape, stride);
            }

            NdRange<D> end() const
            {
                return NdRange<D>(offset + size, shape, stride);
            }
    };
} // namespace numeric

