#include <iostream>
#include <exception>
#include <unistd.h>

#include "threadpool.h"

RauOs::Output greet_rau(RauOs::Input input)
{
    std::cout << "Hello RAU!" << std::endl;
    return nullptr;
}

RauOs::Output greet_008(RauOs::Input input)
{
    std::cout << "Hello 008!" << std::endl;
    return nullptr;
}

int main(int argc, char** argv)
{
    try {
        RauOs::ThreadPool threadPool(20);

        threadPool.run(greet_rau, NULL);
        threadPool.run(greet_008, NULL);

        sleep(1);
    }
    catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    sleep(2);
    return 0;
}
