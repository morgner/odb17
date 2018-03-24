//
// odbcli
// ~~~~~~~~~~~~~~~
// Partial
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
// #include <istream>
// #include <ostream>
#include <string>
#include <asio/ts/internet.hpp>

using asio::ip::tcp;

int main(int argc, char* argv[])
    {
    try
        {
        if (argc != 3)
            {
            std::cout << "Usage: " << argv[0] << " <server> <path>\n";
            std::cout << "Example:\n";
            std::cout << "  " << argv[0] << " localhost 1025\n";
            return 1;
            }
        for (;;)
            {
            asio::ip::tcp::iostream s;
//          s.expires_after(std::chrono::seconds(60));
            s.connect(argv[1], argv[2]);
            if (!s)
                {
                std::cout << "Unable to connect: " << s.error().message() << "\n";
                return 1;
                }

            std::cout << "< ";
            std::string sInput;
            std::getline(std::cin, sInput);
            s << sInput + "\n";

            std::string sOutput;
            do
                {
                std::getline(s, sOutput);
                std::cout << sOutput << "\n";
                } while ( (sOutput != ".") && (sOutput != "") );

            // Write the remaining data to output.
//              std::cout << s.rdbuf();
//              std::cout << "+\n";
            }
        }
    catch (std::exception& e)
        {
        std::cout << "Exception: " << e.what() << "\n";
        }

    return 0;
    }


