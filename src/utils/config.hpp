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

// external
#include <tl/expected.hpp>

// C++ standard
#include <memory>
#include <string>

namespace config
{
    static constexpr std::string_view app_name = "testgui";

    struct settings
    {
        struct exemple
        {
            bool value = false;
        } exemple;

        // TODO
    };

    using settings_t = settings;

    [[nodiscard]] std::string get_settings_path() noexcept;
    [[nodiscard]] tl::expected<settings_t, std::string> from_file(std::string_view path = get_settings_path()) noexcept;
    [[nodiscard]] tl::expected<void, std::string> write_to_file(settings_t settings,
                                                                std::string_view path = get_settings_path()) noexcept;

    namespace imgui
    {
        [[nodiscard]] std::string get_ini_settings_path() noexcept;
    } // namespace imgui
} // namespace config
