#include "type_erasure.h"

#include <iostream>

int main()
{
    auto x = type_erasure{};
    x = 500;
    std::cout << x.get<int>() << std::endl;

    x.emplace<std::string>("Hello World!");
    std::cout << x.get<std::string>() << std::endl;

    x = std::string("hello!");
    std::cout << x.get<std::string>() << std::endl;

    std::cout << x.capacity() << " " << x.size() << std::endl;

    x = 500;

    std::cout << x.capacity() << " " << x.size() << std::endl;

    x.shrink_to_fit();

    std::cout << x.capacity() << " " << x.size() << std::endl;
}