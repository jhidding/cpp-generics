#pragma once
#include <map>
#include <utility>
#include <string>
#include <memory>
#include <vector>

#include "../../misc/format.hh"
#include "../../misc/iterators.hh"

namespace Json
{
    using System::head;
    using System::tail;
    using System::format;

    /*! base class for JSON values */
    class value
    {
        public:
            virtual ~value() noexcept {}
            virtual std::string repr() const = 0;
    };

    /*! store everything in unique_ptrs */
    using ptr = std::unique_ptr<value>;

    inline std::string repr(ptr const &p)
    {
        if (p)
            return p->repr();
        else
            return "null";
    }

    /*! objects are maps from string to ptr */
    class object: public value, public std::map<std::string, ptr>
    {
        public:
            std::string repr() const
            {
                std::ostringstream ss;
                ss << "{";
                ss << head(*this).first << ":" 
                   << Json::repr(head(*this).second);
                for (auto const &kv : *this)
                    ss << "," << kv.first << ":"
                       << Json::repr(kv.second);
                ss << "}";
                return ss.str();
            }
    };

    /*! lists storing ptrs */
    class list: public value, public std::vector<ptr>
    {
        public:
            std::string repr() const
            {
                std::ostringstream ss;
                ss << "[";
                ss << Json::repr(head(*this));
                for (auto const &v : *this)
                    ss << "," << Json::repr(v);
                ss << "]";
                return ss.str();
            }
    };

    /*! base class for atomic data (numeric, boolean, string) */
    template <typename T>
    class atom_base: public value
    {
        T data;

        public:
            atom_base(T const &data): data(data) {}

            /*! retrieve the data stored */
            T const &get() const 
                { return data; }

            /*! give string representation of data */
            virtual std::string repr() const
                { return format(data); }
    };

    template <typename T>
    class atom: public atom_base<T>
    {
        public:
            using atom_base<T>::atom_base;
    };

    /*! specialisation for string, with overloaded repr() */
    template <>
    class atom<std::string>: public atom_base<std::string>
    {
        public:
            using atom_base<std::string>::atom_base;
            virtual std::string repr() const
                { return format('"', get(), '"'); }
    };

    /*! specialisation for boolean, with overloaded repr() */
    template <>
    class atom<bool>: public atom_base<bool>
    {
        public:
            using atom_base<bool>::atom_base;
            virtual std::string repr() const
                { return get() ? "true" : "false"; }
    };

    template <typename T>
    inline ptr make_atom(T const &data)
    {
        return std::make_unique<atom<T>>(data);
    }
}

