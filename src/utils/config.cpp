//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//

// header
#include "config.hpp"

// external
#include <sago/platform_folders.h>

// C++ standard
#include <filesystem>

namespace
{
    constexpr std::string_view settings_filename = "settings.toml";
    constexpr std::string_view imgui_ini_settings_filename = "imgui.ini";

    std::filesystem::path get_config_folder_path() noexcept
    {
        std::filesystem::path path = sago::getConfigHome();
        path.append(config::app_name);

        std::error_code ignored;
        std::filesystem::create_directories(path, ignored);

        return path;
    }
}// namespace

std::string config::get_settings_path() noexcept
{
    std::filesystem::path path = get_config_folder_path();
    path.append(settings_filename);

    return path.generic_string();
}

tl::expected<config::settings_t, std::string> config::from_file(std::string path) noexcept
{
    //TODO
    return tl::make_unexpected("TODO");
}

std::string config::imgui::get_ini_settings_path() noexcept
{
    std::filesystem::path path = get_config_folder_path();
    path.append(imgui_ini_settings_filename);

    return path.generic_string();
}
