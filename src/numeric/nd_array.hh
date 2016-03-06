#pragma once
#include <valarray>

namespace Numeric
{
    template <unsigned D>
    inline shape_t<D> calc_stride(shape_t<D> const &shape)
    {
        shape_t<D> stride;

        stride[0] = 1;
        for (unsigned i = 1; i < shape.size(); ++i)
            stride[i] = stride[i-1] * shape[i-1];
       
        return stride; 
    }

    template <unsigned D>
    inline size_t calc_size(shape_t<D> const &shape)
    {
        size_t size = 1;
        for (unsigned i = 0; i < shape.size(); ++i)
            size *= shape[i];
        return size;
    }
}

