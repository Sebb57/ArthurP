#ifndef APROTOCOL_HPP_
#define APROTOCOL_HPP_

#include "engine/Engine.hpp"
#include "protocol/IProtocol.hpp"
#include <functional>
#include <string>
#include <unordered_map>

namespace protocol {

using Handler = std::function<void(std::string&)>;

template <typename T, auto Member>
auto bindMember(T* obj)
{
    return [obj](std::string& arg) {
        (obj->*Member)(arg);
    };
}

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

} // namespace protocol

#endif /* APROTOCOL_HPP_ */
