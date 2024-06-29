//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//

// header
#include "font.hpp"

// project
#include <utils/log.hpp>

// external
#include <IconsFontAwesome6.h>
#include <compiled_fonts.h>
#include <imgui.h>

// C++ standard
#include <mutex>
#include <stack>
#include <tuple>
#include <unordered_map>

namespace
{
    struct font_data
    {
        const unsigned int* data = nullptr;
        unsigned int size = 0;
    };
    struct font_id
    {
        font::embedded font = font::DEFAULT_FONT;
        float size = font::DEFAULT_FONT_SIZE;

        font_id(font::embedded font, float size) noexcept
            : font(font)
            , size(size)
        {
        }

        bool operator==(const font_id& rhs) const noexcept
        {
            return std::tie(font, size) == std::tie(rhs.font, rhs.size);
        }

        bool operator!=(const font_id& rhs) const noexcept
        {
            return !(rhs == *this);
        }
    };
    struct font_id_hash
    {
        size_t operator()(const font_id& id) const noexcept
        {
            return std::hash<font::embedded>{}(id.font) ^ std::hash<float>{}(id.size);
        }
    };
    struct font_info
    {
        std::mutex fonts_mutex{};
        std::unordered_map<font_id, ImFont*, font_id_hash> loaded_fonts{};
        std::stack<font_id> fonts_stack{};
    };
    font_info DATA;

    [[nodiscard]] font_data get_data(font::embedded font) noexcept
    {
        switch(font)
        {
            case font::embedded::DROID_SANS_MONO:
                return {DroidSansMono_compressed_data, DroidSansMono_compressed_size};
            case font::embedded::INTEL_ONE_MONO:
                return {IntelOneMono_compressed_data, IntelOneMono_compressed_size};
            case font::embedded::NOTO_SANS_MONO:
                return {NotoSansMono_compressed_data, NotoSansMono_compressed_size};
            case font::embedded::ROBOTO_MONO:
                return {RobotoMono_compressed_data, RobotoMono_compressed_size};
            case font::embedded::COUSINE:
                return {Cousine_compressed_data, Cousine_compressed_size};
            case font::embedded::SOURCE_CODE_PRO:
                return {SourceCodePro_compressed_data, SourceCodePro_compressed_size};
        }

        // unreachable
        assert(false);
    }

    // Warning: not thread safe, doesn't lock m_mutex
    [[nodiscard]] ImFont* load_font(font::embedded font, float size)
    {
        std::shared_ptr<spdlog::logger> logger = logging::get_logger("font");
        ImGuiIO& io = ImGui::GetIO();

        static constexpr ImWchar font_ranges[] = {0x0001, 0xFFFF, 0};
        font_data data = get_data(font);
        ImFont* base_font =
          io.Fonts->AddFontFromMemoryCompressedTTF(
            data.data,
            static_cast<int>(data.size),
            size,
            nullptr,
            font_ranges);
        if(base_font)
        {
            SPDLOG_LOGGER_DEBUG(logger, "Loaded font DroidSans {:.2f}px", size);
        }
        else
        {
            base_font = io.Fonts->AddFontDefault();
            SPDLOG_LOGGER_WARN(logger, "Failed to load font DroidSans: use default font instead");
        }
        io.Fonts->Build();

        const float icon_size = size * 0.9f;
        static constexpr ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.GlyphMinAdvanceX = size;
        ImFont* merged_font =
          io.Fonts->AddFontFromMemoryCompressedTTF(
            FontAwesome6_solid_compressed_data,
            static_cast<int>(FontAwesome6_solid_compressed_size),
            icon_size,
            &icons_config,
            icons_ranges);
        if(merged_font)
        {
            SPDLOG_LOGGER_DEBUG(logger, "Loaded font FontAwesome6-solid {:.2f}px", icon_size);
        }
        else
        {
            SPDLOG_LOGGER_WARN(logger, "Failed to load FontAwesome6-solid: icons disabled");
            merged_font = base_font;
        }
        io.Fonts->Build();

        DATA.loaded_fonts[{font, size}] = merged_font;
        static bool first_loaded_font = true;
        if(first_loaded_font)
        {
            // first loaded font is default font
            DATA.fonts_stack.emplace(font, size);
            first_loaded_font = false;
        }
        return merged_font;
    }
}// namespace

font::guard::guard(embedded font, float size) noexcept
{
    push(font, size);
}

font::guard::guard(float size) noexcept
{
    push(size);
}

font::guard::~guard() noexcept
{
    pop();
}

void font::preload(embedded font, float size) noexcept
{
    std::lock_guard guard(DATA.fonts_mutex);
    ImFont* imgui_font = DATA.loaded_fonts[{font, size}];
    if(imgui_font != nullptr)
    {
        return;
    }
    imgui_font = load_font(font, size);
    //ImGui::SFML::UpdateFontTexture();
}

void font::push(embedded font, float size) noexcept
{
    std::lock_guard guard(DATA.fonts_mutex);
    ImFont* imgui_font = DATA.loaded_fonts[{font, size}];
    if(imgui_font == nullptr)
    {
        imgui_font = load_font(font, size);
        //ImGui::SFML::UpdateFontTexture();
    }
    DATA.fonts_stack.emplace(font, size);
    ImGui::PushFont(imgui_font);
}

void font::push(float size) noexcept
{
    std::lock_guard guard(DATA.fonts_mutex);
    embedded font = DEFAULT_FONT;
    if(!DATA.fonts_stack.empty())
    {
        font = DATA.fonts_stack.top().font;
    }
    ImFont* imgui_font = DATA.loaded_fonts[{font, size}];
    if(imgui_font == nullptr)
    {
        imgui_font = load_font(font, size);
        //ImGui::SFML::UpdateFontTexture();
    }
    DATA.fonts_stack.emplace(font, size);
    ImGui::PushFont(imgui_font);
}

void font::pop() noexcept
{
    std::lock_guard guard(DATA.fonts_mutex);
    DATA.fonts_stack.pop();
    ImGui::PopFont();
}
