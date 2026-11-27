#include "protocol/ProtocolFactory.hpp"
#include "protocol/UCI.hpp"
#include <memory>
#include <string>

std::unique_ptr<protocol::IProtocol> protocol::ProtocolFactory::createProtocol(std::string& firstInput)
{
    if (firstInput == "uci")
        return std::make_unique<protocol::UCI>();
    throw UnknownProtocol(firstInput);
}
