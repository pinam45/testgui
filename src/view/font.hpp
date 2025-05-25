//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

// external
#include <spdlog/spdlog.h>

namespace font
{
    constexpr float DEFAULT_FONT_SIZE = 15.0f;
    constexpr float LARGE_FONT_SIZE = 22.0f;

    enum class embedded
    {
        DROID_SANS_MONO,
        INTEL_ONE_MONO,
        NOTO_SANS_MONO,
        ROBOTO_MONO,
        COUSINE,
        SOURCE_CODE_PRO
    };
    constexpr embedded DEFAULT_FONT = embedded::DROID_SANS_MONO;

    struct guard
    {
        explicit guard(embedded font, float size = DEFAULT_FONT_SIZE) noexcept;
        explicit guard(float size) noexcept;
        ~guard() noexcept;
    };

    // first loaded font will be default font (when no font is pushed)
    void preload(embedded font, float size = DEFAULT_FONT_SIZE) noexcept;

    void push(embedded font, float size = DEFAULT_FONT_SIZE) noexcept;
    void push(float size) noexcept;
    void pop() noexcept;
} // namespace font
