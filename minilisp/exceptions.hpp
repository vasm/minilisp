#ifndef exceptions_hpp
#define exceptions_hpp

#include "value.hpp"

#include <sstream>
#include <exception>

using value::type_id;


class unexpected_symbol: public std::exception
{
public:
    unexpected_symbol(char sym, const std::string& scope)
    {
        std::stringstream ss;
        ss << "Unexpected symbol '";

        if  (sym == 0)
            ss << "EOS";
        else
            ss << sym;

        ss << "' in " << scope;
        _s = ss.str();
    }

    const char* what() const noexcept
    {
        return _s.c_str();
    }

private:
    std::string _s;
};


class typeid_exception: public std::exception
{
public:
    typeid_exception(type_id t)
    {
        std::stringstream ss;
        ss << "Type with id = " << (int)t << " is unknown";
        _s = ss.str();
    }

    const char* what() const noexcept
    {
        return _s.c_str();
    }

private:
    std::string _s;
};


#endif /* exceptions_hpp */
