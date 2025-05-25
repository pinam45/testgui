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
#include <fmt/compile.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <sago/platform_folders.h>
#include <toml++/toml.hpp>

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

    template<typename T>
    tl::expected<void, std::string> get_from_table(const toml::table& table, std::string_view path, T& value) noexcept
    {
        if(auto node = table.at_path(path))
        {
            if(auto val = node.as<T>())
            {
                value = val;
            }
            else
            {
                return tl::make_unexpected(
                  fmt::format(FMT_COMPILE("{} has invalid type: {}"), path, fmt::streamed(node.type())));
            }
        }
        else
        {
            return tl::make_unexpected(fmt::format(FMT_COMPILE("{} is missing"), path));
        }
        return {};
    }
} // namespace

std::string config::get_settings_path() noexcept
{
    std::filesystem::path path = get_config_folder_path();
    path.append(settings_filename);

    return path.generic_string();
}

tl::expected<config::settings_t, std::string> config::from_file(std::string_view path) noexcept
{
    config::settings_t config;

    if(toml::parse_result res = toml::parse_file(path))
    {
        const auto& table = res.table();
        if(auto val = get_from_table(table, "exemple.value", config.exemple.value); !val)
        {
            return tl::make_unexpected(val.error());
        }
    }
    else
    {
        return tl::make_unexpected(fmt::format(FMT_COMPILE("{}"), fmt::streamed(res.error())));
    }

    return config;
}

tl::expected<void, std::string> config::write_to_file(config::settings_t settings, std::string_view path) noexcept
{
    toml::table table{
      {"exemple", toml::table{{"value", settings.exemple.value}}}
    };

    std::error_code ignored;
    if(std::filesystem::exists(path, ignored))
    {
        const std::string path_old = std::string(path) + ".old";
        if(std::filesystem::exists(path_old, ignored))
        {
            std::filesystem::remove(path_old, ignored);
        }
        std::filesystem::rename(path, path_old, ignored);
    }
    if(auto file = std::ofstream(std::filesystem::path(path)))
    {
        file << table << std::endl;
    }
    else
    {
        return tl::make_unexpected(fmt::format(FMT_COMPILE("failed to open {}"), path));
    }

    return {};
}

std::string config::imgui::get_ini_settings_path() noexcept
{
    std::filesystem::path path = get_config_folder_path();
    path.append(imgui_ini_settings_filename);

    return path.generic_string();
}
