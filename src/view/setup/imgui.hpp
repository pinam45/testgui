//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

// project
#include <view/setup/window.hpp>

// C++ standard
#include <memory>

struct imgui_context;

using imgui_handle_t = std::shared_ptr<imgui_context>;

namespace setup
{
    imgui_handle_t imgui(const main_window_handle_t& main_window_handle) noexcept;
} // namespace setup

struct imgui_context
{
    friend imgui_handle_t setup::imgui(const main_window_handle_t& main_window_handle) noexcept;

private:
    imgui_context() noexcept = default;

public:
    ~imgui_context() noexcept;
    imgui_context(const imgui_context&) noexcept = delete;
    imgui_context(imgui_context&&) noexcept = default;
    imgui_context& operator=(const imgui_context&) noexcept = delete;
    imgui_context& operator=(imgui_context&&) noexcept = default;
};
