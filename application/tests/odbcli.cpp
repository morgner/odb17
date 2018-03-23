//
// blocking_tcp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include "asio.hpp"

using asio::ip::tcp;

enum { nBufferSize = 1024 };

int main(int argc, char* argv[])
    {
    try
        {
        if (argc != 3)
            {
            std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
            return 1;
            }

        asio::io_context io_context;

        tcp::socket s(io_context);
        tcp::resolver resolver(io_context);
        asio::connect(s, resolver.resolve(argv[1], argv[2]));

        std::cout << "> ";
        std::string sRequest;
        std::getline (std::cin, sRequest);
        asio::write(s, asio::buffer(sRequest, sRequest.length()));
        while (true)
            {
            char acBuffer[nBufferSize];
            size_t nReceiveLength = asio::read(s, asio::buffer(acBuffer, nBufferSize));
            std::cout << "< " << nReceiveLength << ", ";
            std::cout << std::endl;
            std::cout.write(acBuffer, nReceiveLength);
            std::cout << "\n";
            }
        }
    catch (std::exception& e)
        {
        std::cerr << "Exception: " << e.what() << "\n";
        }
    return 0;
    }
