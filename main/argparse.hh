#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <experimental/optional>
#include <stdexcept>
#include "../src/misc/format.hh"

namespace argparse
{
    using std::experimental::optional;
    using std::experimental::nullopt;
    using std::experimental::make_optional;
    using System::format;

    class Exception: public std::exception
    {
        std::string msg;

        public:
            Exception(Exception const &other): msg(other.msg) {};
            Exception(Exception &&other) = default;

            template <typename ...Args>
            Exception(Args &&...args): 
                msg(format(std::forward<Args>(args)...)) {}

            char const *what() const throw () { return msg.c_str(); }

            ~Exception() throw () {}
    };

    template <typename T>
    optional<T> from_string(std::string const &s)
    {
        std::istringstream iss(s);
        T result;
        iss >> result;

        return (iss.fail() ? nullopt : make_optional(result));
    }


    class Args;

    class Option
    {
        friend class Args;

        std::string tag_;
        std::string description_;
        std::string default_value_;

        bool optional_, flag_;

        public:
            Option() = default;

            Option(std::string const &tag, std::string const &description):
                tag_(tag), 
                description_(description),
                default_value_("false"),
                optional_(true),
                flag_(true) {}

            Option(std::string const &tag, std::string const &description,
                   std::string const &default_value, bool optional = true):
                tag_(tag),
                description_(description),
                default_value_(default_value),
                optional_(optional),
                flag_(false) {}
    };

    class Args
    {
        std::map<std::string, Option> option_;
        std::map<std::string, std::string> value_;

        public:
            Args(std::initializer_list<Option> const &options)
            {
                for (Option const &o : options)
                    option_[o.tag_] = o;
            }
 
            template <typename T>
            optional<T> get(std::string const &tag) const
            {
                return from_string<T>(value_.at(tag));
            }

            template <typename Rng>
            void parse(Rng const &args)
            {
                auto begin = args.begin(), end = args.end();

                while (begin != end)
                {
                    std::string tag = *begin;
                    ++begin;

                    if (option_.count(tag) == 0)
                        throw Exception("Unknown option: ", tag);

                    if (option_[tag].flag_)
                    {
                        value_[tag] = "true";
                    } 
                    else 
                    {
                        value_[tag] = *begin;
                        ++begin;
                    }
                }

                for (auto const &kv : option_)
                {
                    if (value_.count(kv.first) == 0)
                    {
                        if (!kv.second.optional_)
                            throw Exception("Missing option: ", kv.first);
                        else
                            value_[kv.first] = kv.second.default_value_;
                    }
                }
            };
    };
}

