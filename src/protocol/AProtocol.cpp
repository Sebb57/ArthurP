/**
 * @file AProtocol.cpp
 * @brief file handling protocol main loop and parsing
 *
 * @author Seb
 * @date 2026-07-14
**/

#include "protocol/AProtocol.hpp"
#include <iostream>
#include <sstream>
#include <string>

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
