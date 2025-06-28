//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

// external
#include <TextDiff.h>
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

    // file releated functions
    void new_file();
    void open_file();
    void open_file(const std::string& path);
    void save_file();

    // manage program exit
    void try_to_quit();

    bool is_done() const
    {
        return done;
    }

    void set_palette(const TextEditor::Palette& palette) noexcept;

    void print() noexcept;

private:
    // private functions
    void print_menu_bar();
    void print_status_bar();

    void show_diff();
    void show_file_open();
    void show_save_file_as();
    void show_confirm_close(std::function<void()> callback);
    void show_confirm_quit();
    void show_error(const std::string& message);

    void render_diff();
    void render_file_open();
    void render_save_as();
    void render_confirm_close();
    void render_confirm_quit();
    void render_confirm_error();

    inline bool is_dirty() const
    {
        return editor.GetUndoIndex() != version;
    }

    inline bool is_savable() const
    {
        return is_dirty() && filename != "untitled";
    }

    // properties
    std::string original_text;
    TextEditor editor;
    TextDiff diff;
    std::string filename;
    size_t version;
    bool done = false;
    std::string error_message;
    std::function<void()> on_confirm_close;

    // editor state
    enum class State
    {
        edit,
        diff,
        newFile,
        openFile,
        saveFileAs,
        confirmClose,
        confirmQuit,
        confirmError
    } state = State::edit;

    std::shared_ptr<spdlog::logger> _logger;
};
