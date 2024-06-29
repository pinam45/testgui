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

struct implot_context;

using implot_handle_t = std::shared_ptr<implot_context>;

namespace setup
{
    implot_handle_t implot() noexcept;
}

struct implot_context
{
    friend implot_handle_t setup::implot() noexcept;

private:
    implot_context() noexcept = default;

public:
    ~implot_context() noexcept;
    implot_context(const implot_context&) noexcept = delete;
    implot_context(implot_context&&) noexcept = default;
    implot_context& operator=(const implot_context&) noexcept = delete;
    implot_context& operator=(implot_context&&) noexcept = default;
};
