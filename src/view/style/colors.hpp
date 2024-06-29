//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

// project
#include <view/style/utils.hpp>

// external
#include <TextEditor.h>
#include <imgui.h>

namespace style
{
    namespace color
    {
        namespace interface
        {
            static constexpr ImVec4 background = ImVec4_from_bytes(37, 37, 38);
            static constexpr ImVec4 background_light = ImVec4_from_bytes(82, 82, 85);
            static constexpr ImVec4 background_very_light = ImVec4_from_bytes(90, 90, 95);

            static constexpr ImVec4 panel = ImVec4_from_bytes(51, 51, 55);
            static constexpr ImVec4 panel_hover = ImVec4_from_bytes(29, 151, 236);
            static constexpr ImVec4 panel_active = ImVec4_from_bytes(0, 119, 200);

            static constexpr ImVec4 text = ImVec4_from_bytes(255, 255, 255);
            static constexpr ImVec4 text_disabled = ImVec4_from_bytes(151, 151, 151);

            static constexpr ImVec4 border = ImVec4_from_bytes(78, 78, 78);
        }// namespace interface

        namespace text_editor
        {
            static constexpr std::array<std::string_view, static_cast<size_t>(TextEditor::PaletteIndex::Max)> palette_color_name{
              "Default",
              "Keyword",
              "Number",
              "String",
              "Char literal",
              "Punctuation",
              "Preprocessor",
              "Identifier",
              "Known identifier",
              "Preproc identifier",
              "Comment (single line)",
              "Comment (multi line)",
              "Background",
              "Cursor",
              "Selection",
              "ErrorMarker",
              "Breakpoint",
              "Breakpoint outline",
              "Current line indicator",
              "Current line indicator outline",
              "Line number",
              "Current line fill",
              "Current line fill (inactive)",
              "Current line edge",
              "Error message",
              "BreakpointDisabled",
              "UserFunction",
              "UserType",
              "UniformType",
              "GlobalVariable",
              "LocalVariable",
              "FunctionArgument"};

            static constexpr TextEditor::Palette palette = {
              ImU32_from_bytes(255, 255, 255, 255),// Default
              ImU32_from_bytes(249, 38, 114, 255), // Keyword
              ImU32_from_bytes(174, 129, 255, 255),// Number
              ImU32_from_bytes(230, 219, 116, 255),// String
              ImU32_from_bytes(230, 219, 116, 255),// Char literal
              ImU32_from_bytes(255, 255, 255, 255),// Punctuation
              ImU32_from_bytes(249, 134, 72, 255), // Preprocessor
              ImU32_from_bytes(255, 255, 255, 255),// Identifier
              ImU32_from_bytes(166, 226, 46, 255), // Known identifier
              ImU32_from_bytes(174, 129, 255, 255),// Preproc identifier
              ImU32_from_bytes(128, 128, 128, 255),// Comment (single line)
              ImU32_from_bytes(128, 128, 128, 255),// Comment (multi line)
              ImU32_from_bytes(30, 31, 34, 255),   // Background
              ImU32_from_bytes(187, 187, 187, 255),// Cursor
              ImU32_from_bytes(33, 66, 131, 255),  // Selection
              ImU32_from_bytes(105, 30, 28, 255),  // ErrorMarker
              ImU32_from_bytes(255, 0, 0, 255),    // Breakpoint
              ImU32_from_bytes(255, 255, 255, 255),// Breakpoint outline
              ImU32_from_bytes(255, 216, 29, 255), // Current line indicator
              ImU32_from_bytes(105, 105, 105, 255),// Current line indicator outline
              ImU32_from_bytes(96, 99, 102, 255),  // Line number
              ImU32_from_bytes(50, 50, 50, 255),   // Current line fill
              ImU32_from_bytes(50, 50, 50, 255),   // Current line fill (inactive)
              ImU32_from_bytes(50, 50, 50, 255),   // Current line edge
              ImU32_from_bytes(255, 255, 51, 255), // Error message
              ImU32_from_bytes(255, 255, 255, 255),// BreakpointDisabled
              ImU32_from_bytes(255, 198, 109, 255),// UserFunction
              ImU32_from_bytes(102, 217, 239, 255),// UserType
              ImU32_from_bytes(209, 79, 249, 255), // UniformType
              ImU32_from_bytes(166, 226, 46, 255), // GlobalVariable
              ImU32_from_bytes(255, 255, 255, 255),// LocalVariable
              ImU32_from_bytes(204, 120, 50, 255), // FunctionArgument
            };
        }// namespace text_editor

        namespace log
        {
            static constexpr ImVec4 trace = ImVec4_from_floats(1.0f, 1.0f, 1.0f, 1.0f);
            static constexpr ImVec4 debug = ImVec4_from_floats(0.557f, 0.808f, 1.0f, 1.0f);
            static constexpr ImVec4 info = ImVec4_from_floats(0.786f, 1.0f, 0.660f, 1.0f);
            static constexpr ImVec4 warn = ImVec4_from_floats(1.0f, 0.969f, 0.118f, 1.0f);
            static constexpr ImVec4 err = ImVec4_from_floats(1.0f, 0.42f, 0.408f, 1.0f);
            static constexpr ImVec4 critical = ImVec4_from_floats(0.893f, 0.694f, 1.0f, 1.0f);
            static constexpr ImVec4 off = ImVec4_from_floats(1.0f, 1.0f, 1.0f, 1.0f);
        }// namespace log
    }// namespace color
}// namespace style
