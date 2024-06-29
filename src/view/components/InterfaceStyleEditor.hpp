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

class InterfaceStyleEditor
{
public:
    explicit InterfaceStyleEditor() noexcept;

    InterfaceStyleEditor(const InterfaceStyleEditor&) = default;
    InterfaceStyleEditor(InterfaceStyleEditor&&) noexcept = default;
    InterfaceStyleEditor& operator=(const InterfaceStyleEditor&) = default;
    InterfaceStyleEditor& operator=(InterfaceStyleEditor&&) noexcept = default;

    ~InterfaceStyleEditor() noexcept = default;

    void print() noexcept;

private:
    std::shared_ptr<spdlog::logger> _logger;
};
