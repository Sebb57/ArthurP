/**
 * @file Logger.cpp
 * @brief generic thread-safe logger writing to a file or any std::ostream
 *
 * @author Seb
 * @date 2026-11-27
**/

#include "utils/Logger.hpp"
#include <chrono>
#include <format>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view levelToText(utils::Logger::Level level)
{
    switch (level) {
        case utils::Logger::Level::DEBUG: return "DEBUG";
        case utils::Logger::Level::INFO: return "INFO ";
        case utils::Logger::Level::WARN: return "WARN ";
        case utils::Logger::Level::ERROR: return "ERROR";
        case utils::Logger::Level::NONE: break;
    }
    return "?????";
}

} // namespace

utils::Logger& utils::Logger::get()
{
    static Logger instance;
    return instance;
}

utils::Logger::Logger(const std::string& path, Level level)
    : minLevel(level)
{
    this->open(path);
}

utils::Logger::Logger(std::ostream& stream, Level level)
    : sink(&stream), minLevel(level)
{
}

void utils::Logger::open(const std::string& path, bool append)
{
    const std::scoped_lock lock(this->mutex);

    if (this->file.is_open())
        this->file.close();
    this->file.open(path, std::ios::out | (append ? std::ios::app : std::ios::trunc));
    this->sink = this->file.is_open() ? &this->file : nullptr;
}

void utils::Logger::setStream(std::ostream& stream)
{
    const std::scoped_lock lock(this->mutex);

    if (this->file.is_open())
        this->file.close();
    this->sink = &stream;
}

void utils::Logger::close()
{
    const std::scoped_lock lock(this->mutex);

    if (this->file.is_open())
        this->file.close();
    this->sink = nullptr;
}

void utils::Logger::setLevel(Level level)
{
    const std::scoped_lock lock(this->mutex);
    this->minLevel = level;
}

void utils::Logger::setName(std::string newName)
{
    const std::scoped_lock lock(this->mutex);
    this->name = std::move(newName);
}

bool utils::Logger::isEnabled(Level level) const
{
    const std::scoped_lock lock(this->mutex);
    return this->sink != nullptr && level != Level::NONE && level >= this->minLevel;
}

void utils::Logger::write(Level level, std::string_view message)
{
    const std::scoped_lock lock(this->mutex);

    if (this->sink == nullptr)
        return;
    const auto now = std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now());
    if (this->name.empty())
        *this->sink << std::format("[{:%T}] [{}] {}\n", now, levelToText(level), message);
    else
        *this->sink << std::format("[{:%T}] [{}] [{}] {}\n", now, levelToText(level), this->name, message);
    // flush every line so nothing is lost on crashes
    this->sink->flush();
}
