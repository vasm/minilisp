#include <iostream>
#include <sstream>
#include <limits>

#include "value.hpp"
#include "exceptions.hpp"

int main(int /*argc*/, const char** /*argv*/)
{
    std::stringstream testProgram;
    testProgram << "(1(2(x(z(_(_1()()))()))))";

    try {
        value::ptr root = value::read(testProgram);
        root->dump(std::cout);
        std::cout << std::endl;
    } catch (unexpected_symbol& ex) {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}
