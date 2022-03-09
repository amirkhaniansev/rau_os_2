#include <iostream>
#include <exception>

#include "threadpool.h"

RauOs::Output greet_rau(RauOs::Input input)
{
    std::cout << "Hello RAU!" << std::endl;
    return nullptr;
}

RauOs::Output greet_005(RauOs::Input input)
{
    std::cout << "Hello 005!" << std::endl;
    return nullptr;
}

int main(int argc, char** argv)
{
    try {
        RauOs::ThreadPool threadPool(32);

        threadPool.run(greet_rau);
        threadPool.run(greet_005);

        threadPool.wait(5000);
    }
    catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}