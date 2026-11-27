#ifndef UCI_HPP_
#define UCI_HPP_

#include "protocol/AProtocol.hpp"
#include <string>
#include <thread>

namespace protocol {

class UCI final : public AProtocol {
    public:
        UCI();
        ~UCI() final;

    private:
        void debug(std::string&);
        void setoption(std::string&);
        void ucinewgame(std::string&);
        void position(std::string&);
        void go(std::string&);
        void stop(std::string&);
        void quit(std::string&);
        void ponderhit(std::string&);

        void waitForSearch();

        std::jthread searchThread;
};

} // namespace protocol

#endif /* UCI_HPP_ */
