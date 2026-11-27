#ifndef IPROTOCOL_HPP_
#define IPROTOCOL_HPP_

#include <string>

namespace protocol {

class IProtocol {
    public:
        virtual ~IProtocol() = default;

        virtual void run() = 0;
        virtual void processLine(std::string& line) = 0;
};

} // namespace protocol

#endif /* IPROTOCOL_HPP_ */
