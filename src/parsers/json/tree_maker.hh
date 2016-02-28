#pragma once
#include <stack>
#include <string>
#include <utility>

#include "types.hh"

namespace Json
{
    /*! Builds a JSON tree step by step.
     *
     * The class uses only unique pointers to store the tree, forcing
     * move-semantics internally. Each time a new list or object is added, the
     * current pointer is pushed to a stack. When the list or object is closed,
     * the previous container is popped from the stack and the previous item is
     * added to this container. 
     *
     * The constructor rootst the tree with a list.
     *
     * TODO: This scheme could be improved upon by using boost::variant for the
     * `small' values like integer, float and bool.
     */
    class TreeMaker
    {
        std::stack<std::pair<std::string, ptr>> stack;
        ptr top;
        std::string key;

        public:
            /*! constructor, creates a new list as top container */
            TreeMaker();

            /*! push a new list to the stack */
            void new_list();

            /*! push an object (associative list) to the stack */
            void new_object();

            /*! add a value to the current top container */
            void add_value(ptr &&v);

            /*! set the key to which to associate the next value */
            void set_key(std::string const &key_);

            /*! close the current top container */
            void close();

            /*! check if whether root container is top */
            bool done() const;

            /*! return the resulting container */
            ptr result();

            /*! add atom value, can be string, bool or number */
            template <typename T>
            void add_atom(T const &v)
            {
                add_value(make_atom<T>(v));
            }
    };
}
