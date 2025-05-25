//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

// project
#include <utils/path_utils.hpp>
#include <utils/store_sink.hpp>

// external
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/null_sink.h>
#include <spdlog/spdlog.h>

extern const std::shared_ptr<spdlog::logger> NULL_LOGGER;
extern const std::shared_ptr<store_sink_mt> STORED_LOGS;

inline std::ostream& operator<<(std::ostream& os, const std::filesystem::path& p)
{
    return os << '\"' << path_to_generic_utf8_string(p) << '\"';
}

namespace logging
{
    [[nodiscard]] bool init_logger() noexcept;
    [[nodiscard]] std::shared_ptr<spdlog::logger> get_logger(const std::string& name = "general") noexcept;
} // namespace logging
