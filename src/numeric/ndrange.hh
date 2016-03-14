#include "types.hh"
#include "support.hh"

namespace numeric
{
    template <unsigned D>
    class NdRange //: public Expression<shape_t<D>, NdRange<D>>
    {
        uint32_t flat_index_;
        shape_t<D> index_, shape_;
        stride_t<D> semi_stride_;

        public:
            NdRange(shape_t<D> const &shape):
                flat_index_(0), shape_(shape)
            {
                index_.fill(0);
                calc_semi_stride(calc_stride(shape_));
            }

            NdRange(size_t offset,
                    shape_t<D> const &shape,
                    stride_t<D> const &stride):
                flat_index_(offset), shape_(shape)
            {
                index_.fill(0);
                calc_semi_stride(stride);
            }

            NdRange &operator++()
            {
                ++index_[0];
                flat_index_ += semi_stride_[0];

                unsigned i = 0;
                while (i < (D-1) && index_[i] == shape_[i])
                {
                    ++i;
                    index_[i-1] = 0;
                    ++index_[i];
                    flat_index_ += semi_stride_[i];
                }

                return *this;
            }

            shape_t<D> const &operator*() const
            {
                return index_;
            }

            uint32_t flat_index() const
            {
                return flat_index_;
            }

            bool operator!=(NdRange const &other)
            {
                return flat_index_ != other.flat_index_;
            }

        private:
            void calc_semi_stride(stride_t<D> const &stride)
            {
                semi_stride_[0] = stride[0];

                for (unsigned i = 1; i < shape_.size(); ++i)
                {
                    semi_stride_[i] = stride[i] - shape_[i-1] * stride[i-1];
                }
            }
    };
} // namespace numeric

