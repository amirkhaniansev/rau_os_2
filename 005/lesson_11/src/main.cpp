/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Test for network manager.
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

#include <stdexcept>
#include <iostream>

int main(int argc, char** argv)
{
    net::url_info url_i;
    url_i.base_url = "mobileapi.fcc.am";
    url_i.api_url = "/FCBank.Mobile.Api/api/PublicInfo/";
    url_i.service = "getrates";
    url_i.port = 80;

    net::nt_man man(url_i);
    
    try
    {
        man.init();
        
        man.request();
        std::string str_result = man.get_result_as_str();
        std::vector<net::currency> res = man.get_result("USD");
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
    }

    man.disconnect();
    
    return 0;
}