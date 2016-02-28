#include "printer.hh"
#include "../../misc/console.hh"

using namespace Json;

size_t pretty_print::length() const
{
    return 0;
}

std::string pretty_print::str() const
{
    return format(w)->str();
}

fancy::ptr pretty_print::copy() const
{
    return fancy::ptr(new pretty_print(w, indent));
}

pretty_print::pretty_print(ptr const &w):
    w(w) {}

pretty_print::pretty_print(ptr const &w, MultiString const &indent):
    w(w), indent(indent) {}

fancy::ptr pretty_print::format(ptr const &p) const
{
    if (auto obj = dynamic_cast<object *>(p.get()))
        return format(obj);

    if (auto lst = dynamic_cast<list *>(p.get()))
        return format(lst);

    return fancy::compose(console::fg(100, 200, 140), repr(p));
}

fancy::ptr pretty_print::format(object const *obj) const
{
    MultiString next_indent(indent);
    next_indent << console::fg(60, 60, 60) << "┊   " << console::reset();
    return fancy::compose(
        console::fg(100, 100, 200), "{\n",
        fancy::intersperse_map(
            obj->begin(), obj->end(), 
            [&next_indent] (std::pair<std::string const, ptr> const &kv) 
            {
                return fancy::compose(next_indent,
                    console::fg(100, 160, 200), "\"" + kv.first + "\": ",
                    pretty_print(kv.second, next_indent)); 
            }, fancy::compose(console::fg(100, 100, 200), ",\n")),
        fancy::compose("\n", indent, console::fg(100, 100, 200), "}"));
}

fancy::ptr pretty_print::format(list const *lst) const
{
    return fancy::compose(
        console::fg(100, 100, 200), "[ ",
        fancy::intersperse_map(
            lst->begin(), lst->end(), 
            [this] (ptr const &v)
            {
                return fancy::ptr(new pretty_print(v, indent));
            }, fancy::compose(console::fg(100, 100, 200), ", ")),
        fancy::compose(console::fg(100, 100, 200), " ]"));
}
