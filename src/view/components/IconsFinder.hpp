//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

// external
#include <imspinner.h>
#include <spdlog/logger.h>

class IconsFinder
{
public:
    explicit IconsFinder() noexcept;

    IconsFinder(const IconsFinder&) = default;
    IconsFinder(IconsFinder&&) noexcept = default;
    IconsFinder& operator=(const IconsFinder&) = default;
    IconsFinder& operator=(IconsFinder&&) noexcept = default;

    ~IconsFinder() noexcept = default;

    void print() noexcept;

private:
    ImGuiTextFilter filter;

    std::shared_ptr<spdlog::logger> _logger;
};
