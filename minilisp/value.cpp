#include "value.hpp"
#include "util.hpp"
#include "exceptions.hpp"

#include <iostream>

namespace value
{
    std::string type_id_as_str(type_id t)
    {
        switch (t) {
            case type_id::unknown:
                return "unknown";

            case type_id::identifier:
                return "id";

            case type_id::integer:
                return "int";

            case type_id::real:
                return "real";

            case type_id::str:
                return "str";

            case type_id::list:
                return "list";

            default:
                throw typeid_exception(t);
        }
    }

    bool is_delimiter(char c)
    {
        return (c == '\0') || std::string(" \t()").find(c) != std::string::npos;
    }

    value::ptr read_list(std::istream& is)
    {
        if (is.peek() != '(') {
            throw unexpected_symbol(is.peek(), "list");
        }

        ptr retval = ptr(new list);

        is.get();
        util::omit_whitespace(is);

        while (is.peek() != ')') {
            value::ptr list_elem = read(is);

            if (list_elem->type() != value::type_id::unknown) {
                dynamic_cast<list*>(retval.get())->push_back(list_elem);
            }

            util::omit_whitespace(is);
        }

        is.get();

        return retval;
    }


    value::ptr read_number(std::istream& is)
    {
        util::omit_whitespace(is);

        char c = is.peek();
        bool is_negative = false;
        if (c == '-') {
            is_negative = true;
            is.get();
        }

        c = is.get();

        long n = 0; // resulting number

        if (!std::isdigit(c)) {
            throw unexpected_symbol(c, "read_number");
        }

        while (std::isdigit(c)) {
            n = n * 10 + (c - '0');
            c = is.get();
        }

        is.unget();

        if (is_negative) {
            n = -n;
        }

        return value::ptr(new value::integer(n));
    }


    bool is_id_starting_symbol(char c)
    {
        return std::isalpha(c) || (c == '_');
    }

    bool is_id_symbol(char c)
    {
        return is_id_starting_symbol(c) || std::isdigit(c);
    }


    value::ptr read_id(std::istream& is)
    {
        if (!is_id_starting_symbol(is.peek())) {
            throw unexpected_symbol(is.peek(), "read_id");
        }

        std::string buffer;
        char c = is.get();

        while (is_id_symbol(c)) {
            buffer.push_back(c);
            c = is.get();
        }

        is.unget();

        return value::ptr(new value::identifier(buffer));
    }


    /*
     * This is the "R" step of the REPL loop: read value from stream,
     * can recursively read list of values
     */
    value::ptr read(std::istream& is)
    {
        ptr retval;
        util::omit_whitespace(is);

        if (!is.eof()) {
            char current(is.peek());
            if (current == '(')
                retval = read_list(is);
            // TODO: need to tell negative number from subtraction operator (look-ahead)
            else if (std::isdigit(current) || (current == '-'))
                retval = read_number(is);
            else if (is_id_starting_symbol(current))
                retval = read_id(is);
            else
                throw unexpected_symbol(current, "read_value_0");

            current = is.peek();
            if (!is.eof() && !is_delimiter(current))
                throw unexpected_symbol(current, "read_value; expected delimiter or EOF");
        } else {
            throw unexpected_symbol(0, "read_value_2");
        }

        return retval;
    }

    template <typename T>
    T get_value(value::ptr p)
    {
        return dynamic_cast<simple_value<T>*>(p.get())->get();
    }
} // namespace value
