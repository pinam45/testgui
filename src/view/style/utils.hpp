//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

// external
#include <TextEditor.h>
#include <imgui.h>

// C++ standard
#include <cstdint>

namespace style
{
    [[nodiscard, gnu::const]] constexpr uint8_t float_to_byte(float f) noexcept
    {
        return static_cast<uint8_t>(std::clamp(f, 0.f, 1.f) * 255.f + 0.5f);
    }

    [[nodiscard, gnu::const]] constexpr ImU32 ImU32_from_bytes(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept
    {
        ImU32 out = r << IM_COL32_R_SHIFT;
        out |= g << IM_COL32_G_SHIFT;
        out |= b << IM_COL32_B_SHIFT;
        out |= a << IM_COL32_A_SHIFT;
        return out;
    }

    [[nodiscard, gnu::const]] constexpr ImU32 ImU32_from_floats(float r, float g, float b, float a = 1.f) noexcept
    {
        return ImU32_from_bytes(float_to_byte(r), float_to_byte(g), float_to_byte(b), float_to_byte(a));
    }

    [[nodiscard, gnu::const]] constexpr ImU32 ImU32_from_ImVec4(const ImVec4& v) noexcept
    {
        return ImU32_from_floats(v.x, v.y, v.z, v.w);
    }

    [[nodiscard, gnu::const]] constexpr ImVec4 ImVec4_from_bytes(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept
    {
        return {
          static_cast<float>(r) / 255.0f,
          static_cast<float>(g) / 255.0f,
          static_cast<float>(b) / 255.0f,
          static_cast<float>(a) / 255.0f};
    }

    [[nodiscard, gnu::const]] constexpr ImVec4 ImVec4_from_floats(float r, float g, float b, float a = 1.f) noexcept
    {
        return {r, g, b, a};
    }

    [[nodiscard, gnu::const]] constexpr ImVec4 ImVec4_from_ImU32(ImU32 v) noexcept
    {
        return {
          static_cast<float>((v >> IM_COL32_R_SHIFT) & 0xFF) / 255.0f,
          static_cast<float>((v >> IM_COL32_G_SHIFT) & 0xFF) / 255.0f,
          static_cast<float>((v >> IM_COL32_B_SHIFT) & 0xFF) / 255.0f,
          static_cast<float>((v >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f};
    }

    [[nodiscard]] std::string palette_cpp1(const TextEditor::Palette& p) noexcept;
    [[nodiscard]] std::string palette_cpp256(const TextEditor::Palette& p) noexcept;
}// namespace style
