/**
 * @file Logger.hpp
 * @brief generic thread-safe logger writing to a file or any std::ostream
 *
 * @author Seb
 * @date 2026-11-27
**/

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <format>
#include <fstream>
#include <mutex>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>

namespace utils {

class Logger {
    public:
        enum class Level { DEBUG, INFO, WARN, ERROR, NONE };

        // process-wide default instance, logs nothing until a sink is set
        static Logger& get();

        Logger() = default;
        explicit Logger(const std::string& path, Level level = Level::DEBUG);
        explicit Logger(std::ostream& stream, Level level = Level::DEBUG);
        ~Logger() = default;

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        // sinks: a file owned by the logger, or an external stream (must outlive the logger)
        void open(const std::string& path, bool append = false);
        void setStream(std::ostream& stream);
        void close();

        void setLevel(Level level);
        void setName(std::string name);
        [[nodiscard]] bool isEnabled(Level level) const;

        template <typename... Args>
        void log(Level level, std::format_string<Args...> fmt, Args&&... args)
        {
            if (!this->isEnabled(level))
                return;
            this->write(level, std::format(fmt, std::forward<Args>(args)...));
        }

        template <typename... Args>
        void debug(std::format_string<Args...> fmt, Args&&... args) { this->log(Level::DEBUG, fmt, std::forward<Args>(args)...); }
        template <typename... Args>
        void info(std::format_string<Args...> fmt, Args&&... args) { this->log(Level::INFO, fmt, std::forward<Args>(args)...); }
        template <typename... Args>
        void warn(std::format_string<Args...> fmt, Args&&... args) { this->log(Level::WARN, fmt, std::forward<Args>(args)...); }
        template <typename... Args>
        void error(std::format_string<Args...> fmt, Args&&... args) { this->log(Level::ERROR, fmt, std::forward<Args>(args)...); }

    private:
        void write(Level level, std::string_view message);

        mutable std::mutex mutex;
        std::ofstream file;
        std::ostream* sink = nullptr;
        std::string name;
        Level minLevel = Level::DEBUG;
};

} // namespace utils

#endif /* LOGGER_HPP_ */
