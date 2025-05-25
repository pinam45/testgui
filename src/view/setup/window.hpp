//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

// project
#include <view/setup/glfw.hpp>

// external
#include <glad/gl.h>
// glad before glfw
#include <GLFW/glfw3.h>

// C++ standard
#include <memory>
#include <string_view>

struct main_window_context;

using main_window_handle_t = std::shared_ptr<main_window_context>;

namespace setup
{
    main_window_handle_t main_window(const glfw_handle_t& glfw_handle,
                                     std::string_view name,
                                     size_t width = 1280,
                                     size_t height = 720) noexcept;
} // namespace setup

struct main_window_context
{
    friend main_window_handle_t
    setup::main_window(const glfw_handle_t& glfw_handle, std::string_view name, size_t width, size_t height) noexcept;

private:
    main_window_context() noexcept = default;

public:
    GLFWwindow* glf_window = nullptr;

    ~main_window_context() noexcept;
    main_window_context(const main_window_context&) noexcept = delete;
    main_window_context(main_window_context&&) noexcept = default;
    main_window_context& operator=(const main_window_context&) noexcept = delete;
    main_window_context& operator=(main_window_context&&) noexcept = default;
};
