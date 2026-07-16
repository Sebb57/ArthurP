/**
 * @file Protocol.hpp
 * @brief file used to manage any protocol for the engine
 *
 * @author Seb
 * @date 2026-07-14
**/

#ifndef PROTOCOL_HPP_
#define PROTOCOL_HPP_

#include "engine/Engine.hpp"
#include <functional>
#include <memory>
#include <string>
#include <iostream>

namespace protocol {

constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

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

        engine::Engine engine;
        std::unordered_map<std::string, Handler> protocolMap;
        bool running = true;
        bool debugRun = false;
};

class UCI : public AProtocol {
    public:
        UCI();
        ~UCI() = default;

    private:
        void debug(std::string&);
        void setoption(std::string&);
        void ucinewgame(std::string&);
        void position(std::string&);
        void go(std::string&);
        void stop(std::string&);
        void ponderhit(std::string&);
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

