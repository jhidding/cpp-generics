#include <array>
#include <cstddef>

namespace numeric
{
    using std::size_t;

    template <unsigned D>
    using shape_t = std::array<uint32_t, D>;

    template <unsigned D>
    using stride_t = std::array<int32_t, D>;
}

