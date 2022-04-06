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
        std::cout << "Please, provide the server name!" << std::endl;
        return 1;
    } 

    const std::string serverName = "/tmp/" + std::string(argv[1]);
    if (mkfifo(serverName.c_str(), 0666) == -1)
    {
        std::cout << "Error: " << std::strerror(errno) << std::endl;
        return 1;
    }

    while(true)
    {
        const int server = open(serverName.c_str(), O_RDONLY);
        if (server == -1)
        {
            std::cout << "Error: " << std::strerror(errno) << std::endl;
            return 1;
        }

        char sizeBuffer[4];
        if (read(server, sizeBuffer, 4) == 0)
        {
            std::cout << "Error: " << std::strerror(errno) << std::endl;
            continue;
        }    

        std::size_t             bufferSize = std::stoi(sizeBuffer) + 1;
        std::unique_ptr<char[]> buffer(new char[bufferSize]);
        if (read(server, buffer.get(), bufferSize) == 0)
        {
            std::cout << "Error: " << std::strerror(errno) << std::endl;
            continue;
        }

        close(server);

        std::cout << "Info: Received: " << sizeBuffer << " " << buffer.get() << std::endl;

        std::stringstream stream;
        stream << buffer.get();

        std::string clientName;
        stream >> clientName;

        char op;
        stream >> op;

        double left;
        stream >> left;

        double right;
        stream >> right;

        double result = 0;
        switch(op)
        {
            case '+': result = left + right; break;
            case '-': result = left - right; break;
            case '*': result = left * right; break;
            case '/': result = left / right; break;
            default:
                break;
        }

        if (clientName.empty())
        {
            std::cout << "Error: Incorrect client name." << std::endl;
            continue;
        }

        std::string resultStr     = std::to_string(result);
        std::string resultStrSize = std::to_string(resultStr.size());
        if (resultStrSize.size() < 4)
            resultStrSize.insert(0, 4 - resultStrSize.size(), '0');

        resultStr = resultStrSize + ' ' + resultStr;        

        const int client = open(clientName.c_str(), O_WRONLY);
        if (client == -1)
        {
            std::cout << "Error: " << std::strerror(errno) << std::endl;
            continue;
        }

        if (write(client, resultStr.c_str(), resultStr.size()) == -1)
        {
            std::cout << "Error: " << std::strerror(errno) << std::endl;
            continue;
        }

        close(client);
    }

    return 0;
}