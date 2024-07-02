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

class ImSpinnerDemo
{
public:
    explicit ImSpinnerDemo() noexcept;

    ImSpinnerDemo(const ImSpinnerDemo&) = default;
    ImSpinnerDemo(ImSpinnerDemo&&) noexcept = default;
    ImSpinnerDemo& operator=(const ImSpinnerDemo&) = default;
    ImSpinnerDemo& operator=(ImSpinnerDemo&&) noexcept = default;

    ~ImSpinnerDemo() noexcept = default;

    void print() noexcept;

private:
    std::shared_ptr<spdlog::logger> _logger;
};
