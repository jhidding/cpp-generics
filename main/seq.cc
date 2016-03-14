#include "command.hh"
#include <iostream>
#include <array>

template <unsigned ...n>
struct seq {};

template <unsigned n, unsigned ...i>
struct make_seq : make_seq<n-1, n-1, i...> {};

template <unsigned ...i>
struct make_seq<0, i...> : seq<i...> {};

inline std::ostream &operator<<(std::ostream &out, seq<>)
{
    return out;
}

template <unsigned i, unsigned ...is>
inline std::ostream &operator<<(std::ostream &out, seq<i,is...>)
{
    return out << i << " " << seq<is...>();
}

template <typename T>
struct seq_size 
{
    enum { value = -1 };
};

template <typename U, size_t n>
struct seq_size<std::array<U, n>>
{
    enum { value = n };
};

template <typename ...Args>
struct seq_size<std::tuple<Args...>>
{
    enum { value = sizeof...(Args) };
};

Command cmd_seq("seq",
    "try out template sequences.",
    [] (std::vector<std::string> const &args)
{
    std::cout << make_seq<10>() << std::endl;
    std::cout << seq_size<std::tuple<char, char, char>>::value << std::endl;
    std::cout << seq_size<std::array<unsigned, 5>>::value << std::endl;
    return 0;
});

