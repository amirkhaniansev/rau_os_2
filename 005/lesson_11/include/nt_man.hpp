/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Network Manager header file.
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

#ifndef __NT_MAN_HPP__
#define __NT_MAN_HPP__

#include "url_info.hpp"
#include "currency.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <string>
#include <vector>

namespace net { 
    const int RECV_BUFFER_SIZE = 4096;
    
    class nt_man {
        public:
            nt_man(net::url_info url);
            void init();
            void disconnect();
            void request();
            std::string get_result_as_str();
            std::vector<currency> get_result(std::string currencyId);
        private:
            int s_fd;
            sockaddr* addr; 
            url_info url_i; 
            std::string result;          
    };
};

#endif