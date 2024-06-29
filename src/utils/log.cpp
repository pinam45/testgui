//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//

// header
#include "log.hpp"

// project
#include <utils/store_sink.hpp>

// external
#include <spdlog/sinks/null_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// C++ standard
#include <cassert>
#include <iostream>

namespace
{
    constexpr bool enable_console_log = true;
}// namespace

const std::shared_ptr<spdlog::logger> NULL_LOGGER =
  std::make_shared<spdlog::logger>("null", std::make_shared<spdlog::sinks::null_sink_mt>());

const std::shared_ptr<store_sink_mt> STORED_LOGS = std::make_shared<store_sink_mt>();

namespace
{
    constexpr const char* LOG_FILENAME = "log.txt";
    constexpr std::size_t LOG_MAX_SIZE = 1024 * 1024 * 5;
    constexpr std::size_t LOG_MAX_FILES = 3;

    std::vector<spdlog::sink_ptr> sinks;
}// namespace

bool logging::init_logger() noexcept
{
    assert(NULL_LOGGER != nullptr);
    try
    {
        // Store sink
        assert(STORED_LOGS != nullptr);
        STORED_LOGS->set_level(spdlog::level::trace);
        sinks.push_back(STORED_LOGS);

        // File sink
        std::shared_ptr<spdlog::sinks::sink> file_sink =
          std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            LOG_FILENAME, LOG_MAX_SIZE, LOG_MAX_FILES);
        assert(file_sink != nullptr);
        file_sink->set_level(spdlog::level::trace);
        sinks.push_back(file_sink);

        // Console sink
        if constexpr(enable_console_log)
        {
            std::shared_ptr<spdlog::sinks::sink> console_sink =
              std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            assert(console_sink != nullptr);
            console_sink->set_level(spdlog::level::warn);
            sinks.push_back(console_sink);
        }

        get_logger()->info("Loggers initialised");
    }
    catch(const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Logger initialization failed: " << ex.what() << std::endl;
        return false;
    }

    return true;
}

std::shared_ptr<spdlog::logger> logging::get_logger(const std::string& name) noexcept
{
    std::shared_ptr<spdlog::logger> logger = spdlog::get(name);
    if(logger)
    {
        return logger;
    }

    logger = std::make_shared<spdlog::logger>(name, sinks.cbegin(), sinks.cend());
    assert(logger != nullptr);
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::err);
    spdlog::register_logger(logger);

    return logger;
}
