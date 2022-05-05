/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Network Manager implementation.
 * Copyright (C) <2019>
 *      Authors: <amirkhaniansev>  <amirkhanyan.sevak@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Full notice : https://github.com/amirkhaniansev/scrapy/tree/master/LICENSE
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#include "../include/nt_man.hpp"
#include "../include/url_info.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#include <stdexcept>
#include <iostream>
#include <string>

net::nt_man::nt_man(net::url_info url) 
{
    this->url_i = url;

    addrinfo** addrinfo;
    int result = getaddrinfo(url.base_url.c_str(), NULL, NULL, addrinfo);
    if(result != 0)
        throw std::runtime_error(strerror(errno));
    
    std::cerr << "ADDRESS : " << addrinfo[0]->ai_addr << std::endl;
    this->addr = addrinfo[0]->ai_addr;
}

void net::nt_man::init()
{
    this->s_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(this->s_fd == -1)
        std::runtime_error(strerror(errno));


    sockaddr_in* s_in = (sockaddr_in*)this->addr;
    in_port_t port = htons(this->url_i.port);
    if(port < (in_port_t)0)
        throw std::runtime_error(strerror(errno));

    std::cerr << "Connecting..." << std::endl;
    s_in->sin_port = port;
    if(connect(this->s_fd, this->addr, sizeof(sockaddr_in)) != 0)
        throw std::runtime_error(strerror(errno));

    std::cerr << "Connected... " << std::endl;
}

void net::nt_man::request()
{
    std::string path = "GET " + 
            this->url_i.api_url + 
            this->url_i.service + 
            " HTTP/1.1\r\n " + 
            "Host: mobileapi.fcc.am\r\n" +
            "Connection : close\r\n\r\n";
    void* buffer = (void*)path.c_str();

    std::cerr << "Sending..." << std::endl;
    if(send(this->s_fd, buffer, path.length(), 0) != path.length())
        throw std::runtime_error(strerror(errno));
    std::cerr << "Sent..." << std::endl;

    this->result.clear();
    char recv_buffer[net::RECV_BUFFER_SIZE];

    ssize_t read_count;
    while (true)
    {
        bzero(recv_buffer, net::RECV_BUFFER_SIZE);
        std::cerr << "Receiving chunk..." << std::endl;
        read_count = recv(this->s_fd, recv_buffer, net::RECV_BUFFER_SIZE, 0);
        std::cerr << "Read count : " << read_count << std::endl;
        if(read_count <= 0)
            break;

        std::cerr << "RECV buffer :" << recv_buffer << std::endl;
        this->result += std::string(recv_buffer, read_count);
    }
}

std::string net::nt_man::get_result_as_str()
{
    return this->result;
}

std::vector<net::currency> net::nt_man::get_result(std::string currencyId)
{
    std::vector<net::currency> v;

    std::string str_res = this->get_result_as_str();
    std::size_t index = str_res.find("Rates");
    if(index == std::string::npos)
        return v;
    
    return v;
}

void net::nt_man::disconnect()
{
    if(shutdown(this->s_fd, SHUT_RDWR) != 0)
        std::cerr << "SHUTDOWN error " << strerror(errno) << std::endl;

    if(close(this->s_fd) != 0)
        std::cerr << "CLOSE erro" << strerror(errno) << std::endl;
}