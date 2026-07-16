/**
 * @file main.cpp
 * @brief main entrypoint
 *
 * @author Seb
 * @date 2026-07-14
**/

#include "protocol/Protocol.hpp"
#include "constants.hpp"
#include <iostream>
#include <string>

int main (int argc, char *argv[])
{
    try {
        std::string protocolName; 
        std::getline(std::cin, protocolName);
        auto protocol = protocol::ProtocolFactory::createProtocol(protocolName);

        protocol->run();

        return constants::SUCCESS;
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;  
        return constants::FAILURE;
    }
}
