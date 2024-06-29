//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

// external
#include <imgui.h>
#include <spdlog/logger.h>

class LogViewer
{
public:
    LogViewer() noexcept;

    LogViewer(const LogViewer&) = default;
    LogViewer(LogViewer&&) noexcept = default;
    LogViewer& operator=(const LogViewer&) = default;
    LogViewer& operator=(LogViewer&&) noexcept = default;

    ~LogViewer() noexcept = default;

    void print() noexcept;

private:
    std::unordered_map<spdlog::level::level_enum, ImVec4> _logs_colors;
    spdlog::level::level_enum _log_level;

    bool _auto_scroll;
    bool _wrap_lines;

    std::shared_ptr<spdlog::logger> _logger;
};
