#include "tree_maker.hh"
#include <memory>
#include <vector>

using namespace Json;

TreeMaker::TreeMaker()
{
    top = std::make_unique<list>();
}

void TreeMaker::new_list()
{
    stack.emplace(key, std::move(top));
    top = std::make_unique<list>();
}

void TreeMaker::new_object()
{
    stack.emplace(key, std::move(top));
    top = std::make_unique<object>();
}

void TreeMaker::add_value(ptr &&v)
{
    if (list *a = dynamic_cast<list *>(top.get()))
    {
        a->emplace_back(std::move(v));
        return;
    }

    if (object *b = dynamic_cast<object *>(top.get()))
    {
        b->emplace(key, std::move(v));
        return;
    }

    throw "fail";
}

void TreeMaker::set_key(std::string const &key_)
{
    key = key_;
}

void TreeMaker::close()
{
    ptr a = std::move(top);
    top = std::move(stack.top().second);
    key = stack.top().first;

    stack.pop();
    add_value(std::move(a));
}

bool TreeMaker::done() const
{
    return stack.empty();
}

ptr TreeMaker::result()
{
    if (list *lst = dynamic_cast<list *>(top.get()))
        return std::move(lst->back());
    else
        return ptr();
}

