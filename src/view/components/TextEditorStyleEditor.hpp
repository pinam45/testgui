//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

// project
#include <view/style/colors.hpp>

// external
#include <TextEditor.h>
#include <sigslot/signal.hpp>
#include <spdlog/logger.h>

class TextEditorStyleEditor
{
public:
    struct style_info
    {
        TextEditor::Palette palette;
    };

    sigslot::signal<style_info> style_changed;

    explicit TextEditorStyleEditor(
    const TextEditor::Palette& text_editor_palette = style::color::text_editor::palette) noexcept;
    // explicit TextEditorStyleEditor(
    //   const TextEditor::Palette& text_editor_palette = TextEditor::GetDarkPalette()) noexcept;

    TextEditorStyleEditor(const TextEditorStyleEditor&) = delete;
    TextEditorStyleEditor(TextEditorStyleEditor&&) noexcept = default;
    TextEditorStyleEditor& operator=(const TextEditorStyleEditor&) = delete;
    TextEditorStyleEditor& operator=(TextEditorStyleEditor&&) noexcept = default;

    ~TextEditorStyleEditor() noexcept = default;

    void print() noexcept;

private:
    TextEditor::Palette _base_text_editor_palette;
    TextEditor::Palette _text_editor_palette;
    TextEditor _example_editor;
    int _export_dest = 0;
    int _export_style = 0;

    std::shared_ptr<spdlog::logger> _logger;
};
