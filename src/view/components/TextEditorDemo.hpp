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
#include <spdlog/logger.h>

class TextEditorDemo
{
public:
    TextEditorDemo() noexcept;

    TextEditorDemo(const TextEditorDemo&) = default;
    TextEditorDemo(TextEditorDemo&&) noexcept = default;
    TextEditorDemo& operator=(const TextEditorDemo&) = default;
    TextEditorDemo& operator=(TextEditorDemo&&) noexcept = default;

    ~TextEditorDemo() noexcept = default;

    void print() noexcept;

    void set_palette(const TextEditor::Palette& palette) noexcept;

private:
    TextEditor _editor;

    std::shared_ptr<spdlog::logger> _logger;
};
