/**
 * @file Protocol.hpp
 * @brief file used to manage any protocol for the engine
 *
 * @author Seb
 * @date 2026-07-14
**/

#ifndef PROTOCOL_HPP_
#define PROTOCOL_HPP_

#include <functional>
#include <memory>
#include <string>
#include <iostream>

namespace protocol {

using Handler = std::function<void(std::string&)>;

class IProtocol {
    public:
        virtual ~IProtocol() = default;

        virtual void run() = 0;
        virtual void processLine(std::string& line) = 0;
};

class AProtocol : public IProtocol {
    public:
        AProtocol() = default;
        ~AProtocol() override = default;

        void run() final;

    protected:
        void processLine(std::string& line) final; 

        bool running = true;
        std::unordered_map<std::string, Handler> protocolMap;
};

class UCI : public AProtocol {
    public:
        UCI();
        ~UCI() = default;
};

class ProtocolFactory {
    public:
        static std::unique_ptr<IProtocol> createProtocol(std::string& firstInput);

        class FactoryException : public std::exception {
            protected:
                std::string _msg;
            public:
                FactoryException(std::string msg) : _msg(msg) {}

                virtual const char* what() const noexcept { return this->_msg.c_str(); }
        };

        class UnknownProtocol : public FactoryException {
            public:
                UnknownProtocol(std::string& p) : FactoryException("Unknown protocol : '" + p + "'") {}
        };
};
    
}

#endif /* PROTOCOL_HPP_ */

