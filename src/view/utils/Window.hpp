//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

// external
#include <imgui.h>

// C++ standard
#include <string>
#include <utility>

template<typename Content, int ImGuiWindowFlags = ImGuiWindowFlags_None>
struct Window
{
    static constexpr int DEFAULT_WIDTH = 900;
    static constexpr int DEFAULT_HEIGHT = 450;

    template<typename... Args>
    explicit Window(std::string name, Args... args);

    void show(bool& open);
    void show();

    const std::string name;
    bool open = true;
    Content content;
};

template<typename Content, int ImGuiWindowFlags>
template<typename... Args>
Window<Content, ImGuiWindowFlags>::Window(std::string name_, Args... args)
    : name(std::move(name_))
    , content(std::forward<Args>(args)...)
{
}

template<typename Content, int ImGuiWindowFlags>
void Window<Content, ImGuiWindowFlags>::show(bool& open)
{
    ImGui::SetNextWindowSize(ImVec2(DEFAULT_WIDTH, DEFAULT_HEIGHT), ImGuiCond_Once);

    if(!ImGui::Begin(name.c_str(), &open, ImGuiWindowFlags))
    {
        ImGui::End();
        return;
    }
    content.print();
    ImGui::End();
}

template<typename Content, int ImGuiWindowFlags>
void Window<Content, ImGuiWindowFlags>::show()
{
    show(open);
}
