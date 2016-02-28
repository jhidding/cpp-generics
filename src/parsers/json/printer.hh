#pragma once
#include "types.hh"
#include "../../misc.hh"

namespace Json
{
    namespace fancy = System::fancy;
    namespace colour = System::colour;
    namespace console = System::console;

    using fancy::MultiString;
    using System::head;
    using System::tail;

    class pretty_print: public fancy::Element
    {
        ptr const &w;
        MultiString indent;

        public:
            virtual size_t length() const;
            virtual std::string str() const;
            virtual fancy::ptr copy() const;
            pretty_print(ptr const &w);
            pretty_print(ptr const &w, MultiString const &indent);
            fancy::ptr format(ptr const &p) const;
            fancy::ptr format(object const *obj) const;
            fancy::ptr format(list const *lst) const;
    };
}

