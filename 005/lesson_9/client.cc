#include <iostream>
#include <string>
#include <cstring>
#include <memory>
#include <sstream>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Error: No server name is provided." << std::endl;
        return 1;
    }
 
    const std::string clientName = "/tmp/" + std::to_string(getpid());
    if (mkfifo(clientName.c_str(), 0666) == -1)
    {
        std::cout << "Error: " << std::strerror(errno) << std::endl;
        return 1;
    }

    const std::string serverName = "/tmp/" + std::string(argv[1]);
    while(true)
    {
        std::cout << "Please, type input in Lukasiewicz's notation: ";

        std::string op;
        std::cin >> op;

        std::string left;
        std::cin >> left;

        std::string right;
        std::cin >> right;

        std::string input     = clientName + ' ' + op + ' ' + left + ' ' + right;
        std::string inputSize = std::to_string(input.size());
        if (inputSize.size() < 4)
            inputSize.insert(0, 4 - inputSize.size(), '0');

        std::cout << "Info: Input: " << input << std::endl;

        input = inputSize + ' ' + input;
        std::cout << "Info: Sending: " << input << std::endl;

        const int server = open(serverName.c_str(), O_WRONLY);
        if (server == -1)
        {
            std::cout << "Error : " << std::strerror(errno) << std::endl;
            return 1;
        }
        std::cout << input.c_str() << std::endl;
        if (write(server, input.c_str(), input.size()) == -1)
        {
            std::cout << "Error: " << std::strerror(errno) << std::endl;
            continue;
        }

        close(server);

        const int client = open(clientName.c_str(), O_RDONLY);
        if (client == -1)
        {
            std::cout << "Error: " << std::strerror(errno) << std::endl;
            continue;
        }    

        char sizeBuffer[4];
        if (read(client, sizeBuffer, 4) == 0)
        {
            std::cout << "Error: " << std::strerror(errno) << std::endl;
            continue;
        }

        std::size_t             bufferSize = std::stoi(sizeBuffer);
        std::unique_ptr<char[]> buffer(new char[bufferSize]);
        if (read(client, buffer.get(), bufferSize) == 0)
        {
            std::cout << "Error: " << std::strerror(errno) << std::endl;
            continue;
        }

        std::cout << "Result: " << std::stod(buffer.get()) << std::endl;

        close(client);   
    }

    return 0;
}