#pragma once
#include <tuple>
#include <array>
#include <map>
#include <string>
#include <cstdint>
#include <vector>

namespace record_array
{
    class Exception: public std::exception
    {
        std::string msg;

        public:
            Exception(std::string const &msg_): msg(msg_) {}
            char const *what() const throw () { return msg.c_str(); }
            ~Exception() throw () {}
    };

    enum TYPE_ID { 
        T_CHAR, T_UCHAR, T_SHORT, T_USHORT, 
        T_INT,  T_UINT,  T_FLOAT, T_DOUBLE  };

    template <typename T>
    struct Type {
        static TYPE_ID     id;
        static std::string name;
    };

    struct Type_info
    {
        std::string name;
        unsigned    size;
    };

    extern std::map<TYPE_ID, Type_info> Type_map;

    struct Field
    {
        TYPE_ID     type;
        std::string name;
    };

    using RecordSpec = std::vector<Field>;

    inline size_t record_size(RecordSpec const &r)
    {
        size_t a = 0;
        for (Field const &f : r)
            a += Type_map[f.type].size;
        return a;
    }

    template <unsigned ...n>
    struct seq {};

    template <unsigned n, unsigned ...i>
    struct make_seq : make_seq<n-1, n-1, i...> {};

    template <unsigned ...i>
    struct make_seq<0, i...> : seq<i...> {};

    template <typename T, typename U>
    static T cast(char const *d)
    {
        return static_cast<T>(*reinterpret_cast<U const *>(d));
    }

    template <typename T>
    inline char const *read_field(Field const &f, char const *data, T &tgt)
    {
        switch (f.type)
        {
            case T_CHAR:    tgt = cast<T, int8_t  >(data);
                            return data + 1;
            case T_UCHAR:   tgt = cast<T, uint8_t >(data);
                            return data + 1;
            case T_SHORT:   tgt = cast<T, int16_t >(data);
                            return data + 2;
            case T_USHORT:  tgt = cast<T, uint16_t>(data);
                            return data + 2;
            case T_INT:     tgt = cast<T, int32_t >(data);
                            return data + 4;
            case T_UINT:    tgt = cast<T, uint32_t>(data);
                            return data + 4;
            case T_FLOAT:   tgt = cast<T, float   >(data);
                            return data + 4;
            case T_DOUBLE:  tgt = cast<T, double  >(data);
                            return data + 8;
        }
    }

    template <typename T>
    inline char const *read_record(
        RecordSpec const &spec, char const *data, T &tgt, seq<>)
    {
        return data;
    }

    template <typename T, unsigned i, unsigned ...is>
    inline char const *read_record(
        RecordSpec const &spec, char const *data, T &tgt, seq<i,is...>)
    {
        return read_record(spec, 
            read_field(spec[i], data, std::get<i>(tgt)), 
            tgt, seq<is...>());
    }

    template <typename T>
    class RecordIterator: public std::iterator<std::forward_iterator_tag, T>
    {
        RecordSpec const &spec;
        char const *data;
        T record;

        public:
            RecordIterator(RecordSpec const &spec, char const *data):
                spec(spec), data(data) 
            {
            }

            RecordIterator &operator++()
            {
                data = read_record(spec, data, record, make_seq<std::tuple_size<T>::value>());
                return *this;
            }

            T const &operator*() const
            {
                return record;
            }

            bool operator!=(RecordIterator const &other) const
            {
                return data != other.data;
            }
    };

    template <typename T>
    class RecordArrayView
    {
        RecordSpec const &spec;
        char const *data;
        size_t stride, size;

        public:
            RecordArrayView(RecordSpec const &spec, char const *data, size_t size):
                spec(spec), data(data), stride(record_size(spec)), size(size)
            {
                if (std::tuple_size<T>::value != spec.size())
                {
                    throw Exception("Trying to iterate a RecordArray with wrong tuple or array.");
                }
            }

            RecordIterator<T> begin() const
            { 
                return RecordIterator<T>(spec, data);
            }

            RecordIterator<T> end() const
            {
                return RecordIterator<T>(spec, data + size*stride);
            }
    };

    class RecordArray
    {
        std::vector<char> data;

        public:
            RecordSpec spec;

            RecordArray() {}

            template <typename T>
            RecordArrayView<T> as()
            {
                return RecordArrayView<T>(spec, data.data, record_size(spec));
            }
    };
}

