/**
 * @file main.cpp
 * @brief main entrypoint
 *
 * @author Seb
 * @date 2026-07-14
**/

#include "protocol/Protocol.hpp"
#include "constants.hpp"
#include "utils/Logger.hpp"
#include <iostream>
#include <string>

int main ()
{
    try {
        utils::Logger::get().open(constants::LOG_FILE);
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
