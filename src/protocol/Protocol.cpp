/**
 * @file Protocol.cpp
 * @brief file handling protocol main loop and parsing
 *
 * @author Seb
 * @date 2026-07-14
**/

#include "Protocol.hpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

std::unique_ptr<protocol::IProtocol> protocol::ProtocolFactory::createProtocol(std::string& firstInput)
{
    if (firstInput == "uci")
        return std::make_unique<protocol::UCI>();
    throw UnknownProtocol(firstInput);
}

void protocol::AProtocol::processLine(std::string& line)
{
    std::string command, args;
    std::istringstream iss(line);

    iss >> command;
    std::getline(iss, args);
    if (!args.empty() && args.front() == ' ')
        args.erase(0, 1);

    if (command.empty())
        return;

    auto it = this->protocolMap.find(command);
    if (it != this->protocolMap.end())
        it->second(args);
    else 
        std::cout << "Unknown command" << std::endl;
        
}

void protocol::AProtocol::run()
{
    std::string line;

    while (running && std::getline(std::cin, line)) {
        processLine(line);
    }
}
