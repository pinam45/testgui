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

struct gl3w_context;

using gl3w_handle_t = std::shared_ptr<gl3w_context>;

namespace setup
{
    gl3w_handle_t gl3w() noexcept;
}

struct gl3w_context
{
    friend gl3w_handle_t setup::gl3w() noexcept;

private:
    gl3w_context() noexcept = default;

public:
    ~gl3w_context() noexcept = default;
    gl3w_context(const gl3w_context&) noexcept = delete;
    gl3w_context(gl3w_context&&) noexcept = default;
    gl3w_context& operator=(const gl3w_context&) noexcept = delete;
    gl3w_context& operator=(gl3w_context&&) noexcept = default;
};
