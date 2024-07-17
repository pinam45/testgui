//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//

// header
#include "imgui.hpp"

// project
#include <view/style/colors.hpp>

// external
#include <imgui.h>

namespace
{
    void old_style() noexcept
    {
        ImGui::StyleColorsDark();

        ImGuiStyle* style = &ImGui::GetStyle();
        ImVec4* colors = style->Colors;

        colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
        colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
        colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
        colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
        colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
        colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
        colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
        colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
        colors[ImGuiCol_TabSelected] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
        colors[ImGuiCol_TabDimmed] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
        colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
        colors[ImGuiCol_DockingPreview] = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
        colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

        style->ChildRounding = 0.f;//4.0f;
        style->FrameBorderSize = 1.0f;
        style->FrameRounding = 0.f;//2.0f;
        style->GrabMinSize = 7.0f;
        style->PopupRounding = 0.f;    //2.0f;
        style->ScrollbarRounding = 0.f;//12.0f;
        style->ScrollbarSize = 13.0f;
        style->TabBorderSize = 1.0f;
        style->TabRounding = 0.f;   //0.0f;
        style->WindowRounding = 0.f;//4.0f;
    }
}// namespace

void style::setup_imgui() noexcept
{
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ChildRounding = 0.f;
    style.FrameBorderSize = 1.0f;
    style.FrameRounding = 0.f;
    style.GrabRounding = 0.0f;
    style.GrabMinSize = 7.0f;
    style.PopupRounding = 0.f;
    style.ScrollbarRounding = 0.f;
    style.ScrollbarSize = 13.0f;
    style.TabBorderSize = 1.0f;
    style.TabRounding = 0.f;
    style.WindowRounding = 0.f;

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text] = color::interface::text;
    colors[ImGuiCol_TextDisabled] = color::interface::text_disabled;
    colors[ImGuiCol_WindowBg] = color::interface::background;
    colors[ImGuiCol_ChildBg] = color::interface::background;
    colors[ImGuiCol_PopupBg] = color::interface::background;
    colors[ImGuiCol_Border] = color::interface::border;
    colors[ImGuiCol_BorderShadow] = color::interface::border;
    colors[ImGuiCol_FrameBg] = color::interface::panel;
    colors[ImGuiCol_FrameBgHovered] = color::interface::panel_hover;
    colors[ImGuiCol_FrameBgActive] = color::interface::panel_active;
    colors[ImGuiCol_TitleBg] = color::interface::background;
    colors[ImGuiCol_TitleBgActive] = color::interface::background;
    colors[ImGuiCol_TitleBgCollapsed] = color::interface::background;
    colors[ImGuiCol_MenuBarBg] = color::interface::panel;
    colors[ImGuiCol_ScrollbarBg] = color::interface::panel;
    colors[ImGuiCol_ScrollbarGrab] = color::interface::background_light;
    colors[ImGuiCol_ScrollbarGrabHovered] = color::interface::background_very_light;
    colors[ImGuiCol_ScrollbarGrabActive] = color::interface::background_very_light;
    colors[ImGuiCol_CheckMark] = color::interface::panel_active;
    colors[ImGuiCol_SliderGrab] = color::interface::panel_hover;
    colors[ImGuiCol_SliderGrabActive] = color::interface::panel_active;
    colors[ImGuiCol_Button] = color::interface::panel;
    colors[ImGuiCol_ButtonHovered] = color::interface::panel_hover;
    colors[ImGuiCol_ButtonActive] = color::interface::panel_hover;
    colors[ImGuiCol_Header] = color::interface::background_light;
    colors[ImGuiCol_HeaderHovered] = color::interface::panel_hover;
    colors[ImGuiCol_HeaderActive] = color::interface::panel_active;
    colors[ImGuiCol_Separator] = color::interface::border;
    colors[ImGuiCol_SeparatorHovered] = color::interface::border;
    colors[ImGuiCol_SeparatorActive] = color::interface::border;
    colors[ImGuiCol_ResizeGrip] = color::interface::background;
    colors[ImGuiCol_ResizeGripHovered] = color::interface::panel;
    colors[ImGuiCol_ResizeGripActive] = color::interface::background_light;
    colors[ImGuiCol_Tab] = color::interface::background;
    colors[ImGuiCol_TabHovered] = color::interface::panel_hover;
    colors[ImGuiCol_TabSelected] = color::interface::panel_active;
    colors[ImGuiCol_TabDimmed] = color::interface::background;
    colors[ImGuiCol_TabDimmedSelected] = color::interface::panel_active;
    colors[ImGuiCol_DockingPreview] = color::interface::panel_active;
    //colors[ImGuiCol_DockingEmptyBg] = TODO;
    colors[ImGuiCol_PlotLines] = color::interface::panel_active;
    colors[ImGuiCol_PlotLinesHovered] = color::interface::panel_hover;
    colors[ImGuiCol_PlotHistogram] = color::interface::panel_active;
    colors[ImGuiCol_PlotHistogramHovered] = color::interface::panel_hover;
    //colors[ImGuiCol_TableHeaderBg] = TODO;
    //colors[ImGuiCol_TableBorderStrong] = TODO;
    //colors[ImGuiCol_TableBorderLight] = TODO;
    //colors[ImGuiCol_TableRowBg] = TODO;
    //colors[ImGuiCol_TableRowBgAlt] = TODO;
    colors[ImGuiCol_TextSelectedBg] = color::interface::panel_active;
    colors[ImGuiCol_DragDropTarget] = color::interface::background;
    colors[ImGuiCol_NavHighlight] = color::interface::background;
    //colors[ImGuiCol_NavWindowingHighlight] = TODO;
    //colors[ImGuiCol_NavWindowingDimBg] = TODO;
    colors[ImGuiCol_ModalWindowDimBg] = color::interface::background;
    colors[ImGuiCol_ModalWindowDimBg].w = 0.586f;
}
