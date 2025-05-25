//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//

// header
#include "utils.hpp"

// external
#include <TextEditor.h>
#include <fmt/core.h>

namespace
{
    std::string colortxt1(const ImVec4& color) noexcept
    {
        return fmt::format("{}, {}, {}, {}", color.x, color.y, color.z, color.w);
    }

    std::string colortxt1(ImU32 color) noexcept
    {
        return colortxt1(style::ImVec4_from_ImU32(color));
    }

    std::string colortxt256(const ImVec4& color) noexcept
    {
        return fmt::format("{}, {}, {}, {}",
                           style::byte_from_float(color.x),
                           style::byte_from_float(color.y),
                           style::byte_from_float(color.z),
                           style::byte_from_float(color.w));
    }

    std::string colortxt256(ImU32 color) noexcept
    {
        return colortxt256(style::ImVec4_from_ImU32(color));
    }
} // namespace

std::string style::palette_cpp1(const TextEditor::Palette& p) noexcept
{
    return fmt::format(R"(static constexpr TextEditor::Palette palette = {{
    style::ImVec4_from_bytes({}), // Default
    style::ImVec4_from_bytes({}), // Keyword
    style::ImVec4_from_bytes({}), // Number
    style::ImVec4_from_bytes({}), // String
    style::ImVec4_from_bytes({}), // Char literal
    style::ImVec4_from_bytes({}), // Punctuation
    style::ImVec4_from_bytes({}), // Preprocessor
    style::ImVec4_from_bytes({}), // Identifier
    style::ImVec4_from_bytes({}), // Known identifier
    style::ImVec4_from_bytes({}), // Preproc identifier
    style::ImVec4_from_bytes({}), // Comment (single line)
    style::ImVec4_from_bytes({}), // Comment (multi line)
    style::ImVec4_from_bytes({}), // Background
    style::ImVec4_from_bytes({}), // Cursor
    style::ImVec4_from_bytes({}), // Selection
    style::ImVec4_from_bytes({}), // ErrorMarker
    style::ImVec4_from_bytes({}), // Breakpoint
    style::ImVec4_from_bytes({}), // Breakpoint outline
    style::ImVec4_from_bytes({}), // Current line indicator
    style::ImVec4_from_bytes({}), // Current line indicator outline
    style::ImVec4_from_bytes({}), // Line number
    style::ImVec4_from_bytes({}), // Current line fill
    style::ImVec4_from_bytes({}), // Current line fill (inactive)
    style::ImVec4_from_bytes({}), // Current line edge
    style::ImVec4_from_bytes({}), // Error message
    style::ImVec4_from_bytes({}), // BreakpointDisabled
    style::ImVec4_from_bytes({}), // UserFunction
    style::ImVec4_from_bytes({}), // UserType
    style::ImVec4_from_bytes({}), // UniformType
    style::ImVec4_from_bytes({}), // GlobalVariable
    style::ImVec4_from_bytes({}), // LocalVariable
    style::ImVec4_from_bytes({}), // FunctionArgument
}};
)",
                       colortxt1(p[0]),
                       colortxt1(p[1]),
                       colortxt1(p[2]),
                       colortxt1(p[3]),
                       colortxt1(p[4]),
                       colortxt1(p[5]),
                       colortxt1(p[6]),
                       colortxt1(p[7]),
                       colortxt1(p[8]),
                       colortxt1(p[9]),
                       colortxt1(p[10]),
                       colortxt1(p[11]),
                       colortxt1(p[12]),
                       colortxt1(p[13]),
                       colortxt1(p[14]),
                       colortxt1(p[15]),
                       colortxt1(p[16]),
                       colortxt1(p[17]),
                       colortxt1(p[18]),
                       colortxt1(p[19]),
                       colortxt1(p[20]),
                       colortxt1(p[21]),
                       colortxt1(p[22]),
                       colortxt1(p[23]),
                       colortxt1(p[24]),
                       colortxt1(p[25]),
                       colortxt1(p[26]),
                       colortxt1(p[27]),
                       colortxt1(p[28]),
                       colortxt1(p[29]),
                       colortxt1(p[30]),
                       colortxt1(p[31]));
}

std::string style::palette_cpp256(const TextEditor::Palette& p) noexcept
{
    return fmt::format(R"(static constexpr TextEditor::Palette palette = {{
    style::ImU32_from_bytes({}), // Default
    style::ImU32_from_bytes({}), // Keyword
    style::ImU32_from_bytes({}), // Number
    style::ImU32_from_bytes({}), // String
    style::ImU32_from_bytes({}), // Char literal
    style::ImU32_from_bytes({}), // Punctuation
    style::ImU32_from_bytes({}), // Preprocessor
    style::ImU32_from_bytes({}), // Identifier
    style::ImU32_from_bytes({}), // Known identifier
    style::ImU32_from_bytes({}), // Preproc identifier
    style::ImU32_from_bytes({}), // Comment (single line)
    style::ImU32_from_bytes({}), // Comment (multi line)
    style::ImU32_from_bytes({}), // Background
    style::ImU32_from_bytes({}), // Cursor
    style::ImU32_from_bytes({}), // Selection
    style::ImU32_from_bytes({}), // ErrorMarker
    style::ImU32_from_bytes({}), // Breakpoint
    style::ImU32_from_bytes({}), // Breakpoint outline
    style::ImU32_from_bytes({}), // Current line indicator
    style::ImU32_from_bytes({}), // Current line indicator outline
    style::ImU32_from_bytes({}), // Line number
    style::ImU32_from_bytes({}), // Current line fill
    style::ImU32_from_bytes({}), // Current line fill (inactive)
    style::ImU32_from_bytes({}), // Current line edge
    style::ImU32_from_bytes({}), // Error message
    style::ImU32_from_bytes({}), // BreakpointDisabled
    style::ImU32_from_bytes({}), // UserFunction
    style::ImU32_from_bytes({}), // UserType
    style::ImU32_from_bytes({}), // UniformType
    style::ImU32_from_bytes({}), // GlobalVariable
    style::ImU32_from_bytes({}), // LocalVariable
    style::ImU32_from_bytes({}), // FunctionArgument
}};
)",
                       colortxt256(p[0]),
                       colortxt256(p[1]),
                       colortxt256(p[2]),
                       colortxt256(p[3]),
                       colortxt256(p[4]),
                       colortxt256(p[5]),
                       colortxt256(p[6]),
                       colortxt256(p[7]),
                       colortxt256(p[8]),
                       colortxt256(p[9]),
                       colortxt256(p[10]),
                       colortxt256(p[11]),
                       colortxt256(p[12]),
                       colortxt256(p[13]),
                       colortxt256(p[14]),
                       colortxt256(p[15]),
                       colortxt256(p[16]),
                       colortxt256(p[17]),
                       colortxt256(p[18]),
                       colortxt256(p[19]),
                       colortxt256(p[20]),
                       colortxt256(p[21]),
                       colortxt256(p[22]),
                       colortxt256(p[23]),
                       colortxt256(p[24]),
                       colortxt256(p[25]),
                       colortxt256(p[26]),
                       colortxt256(p[27]),
                       colortxt256(p[28]),
                       colortxt256(p[29]),
                       colortxt256(p[30]),
                       colortxt256(p[31]));
}
