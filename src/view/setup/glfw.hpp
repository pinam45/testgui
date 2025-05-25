//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

// C++ standard
#include <memory>

struct glfw_context;

using glfw_handle_t = std::shared_ptr<glfw_context>;

namespace setup
{
    glfw_handle_t glfw() noexcept;
} // namespace setup

struct glfw_context
{
    friend glfw_handle_t setup::glfw() noexcept;

private:
    glfw_context() noexcept = default;

public:
    ~glfw_context() noexcept;
    glfw_context(const glfw_context&) noexcept = delete;
    glfw_context(glfw_context&&) noexcept = default;
    glfw_context& operator=(const glfw_context&) noexcept = delete;
    glfw_context& operator=(glfw_context&&) noexcept = default;
};
