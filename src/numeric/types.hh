#include <array>
#include <cstddef>

namespace Numeric
{
    using std::size_t;

    template <unsigned D>
    using shape_t = std::array<size_t, D>;
}

