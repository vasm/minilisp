#include "util.hpp"

#include <cctype>

namespace util
{
    void omit_whitespace(std::istream& is)
    {
        while (std::isspace(is.peek()))
            is.get();
    }
}
