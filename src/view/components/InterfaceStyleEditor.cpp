//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//

// header
#include "InterfaceStyleEditor.hpp"

// project
#include <utils/log.hpp>
#include <view/style/colors.hpp>

InterfaceStyleEditor::InterfaceStyleEditor() noexcept : _logger(logging::get_logger("InterfaceStyleEditor"))
{
}

void InterfaceStyleEditor::print() noexcept
{
    ImGui::ShowStyleEditor();
}
