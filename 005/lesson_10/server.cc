#include <iostream>
#include <string>
#include <cstring>
#include <memory>
#include <sstream>
 
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
 
struct request
{
   char* buffer;
   ~request()
   {
   	delete buffer;
   }
};
 
void* processRequest(void* data)
{
	request* r = (request*)data;
	std::cout << "Received " << r->buffer << '\n';
	std::stringstream stream;
	stream << r->buffer;
 
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
 
        std::string resultStr = std::to_string(result);
        std::string resultStrSize = std::to_string(resultStr.size());
 
        if (resultStrSize.size() < 4)
            resultStrSize.insert(0, 4 - resultStrSize.size(), '0');
 
        std::string response = resultStrSize + ' ' + resultStr;
 
        std::cout << "Client Name " << clientName << '\n';
        int fd = open(clientName.c_str(), O_WRONLY);
        if ( fd == -1)
        {
                  std::string s =  std::string("Error: ") + std::strerror(errno) + '\n';
                  std::cout << s; 
        }
        write(fd, response.c_str(), response.size());
        close(fd);
        delete r;
        return nullptr;
}
 
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
        if(std::string(sizeBuffer) == "exit"){
        	close(server);
        	unlink(serverName.c_str());
        	break;
        }
 
        std::size_t             bufferSize = std::stoi(sizeBuffer) + 1;
        char* buffer = new char[bufferSize];
        if (read(server, buffer, bufferSize) == 0)
        {
            std::cout << "Error: " << std::strerror(errno) << std::endl;
            continue;
        }
 
        close(server);
 
        std::cout << "Info: Received: " << sizeBuffer << " " << buffer << std::endl;
        pthread_t t;
        request* r = new request;
        r->buffer = buffer;
        pthread_create(&t, nullptr, processRequest, r);
    
    }
 
    return 0;
}