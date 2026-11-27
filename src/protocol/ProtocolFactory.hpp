#ifndef PROTOCOLFACTORY_HPP_
#define PROTOCOLFACTORY_HPP_

#include "protocol/IProtocol.hpp"
#include <exception>
#include <memory>
#include <string>
#include <utility>

namespace protocol {

class ProtocolFactory {
    public:
        static std::unique_ptr<IProtocol> createProtocol(std::string& firstInput);

        class FactoryException : public std::exception {
            protected:
                std::string _msg;
            public:
                explicit FactoryException(std::string msg) : _msg(std::move(msg)) {}

                [[nodiscard]] const char* what() const noexcept override { return this->_msg.c_str(); }
        };

        class UnknownProtocol : public FactoryException {
            public:
                explicit UnknownProtocol(const std::string& p) : FactoryException("Unknown protocol : '" + p + "'") {}
        };
};

} // namespace protocol

#endif /* PROTOCOLFACTORY_HPP_ */
