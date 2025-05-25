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
#include <utils/config.hpp>
#include <view/style/colors.hpp>
#include <view/style/imgui.hpp>

// external
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <imgui_internal.h>

namespace
{
    std::weak_ptr<imgui_context> imgui_existing_context;
    std::string imgui_ini_filename;
} // namespace

imgui_handle_t setup::imgui(const main_window_handle_t& main_window_handle) noexcept
{
    if(main_window_handle == nullptr)
    {
        return nullptr;
    }

    std::shared_ptr<imgui_context> context = imgui_existing_context.lock();
    if(context)
    {
        return context;
    }

    // Create ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(main_window_handle->glf_window, true);
    static constexpr const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup ImGui
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigDockingWithShift = true; // hold shift to use docking
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // enable docking
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // enable Gamepad Controls

    imgui_ini_filename = config::imgui::get_ini_settings_path();
    io.IniFilename = imgui_ini_filename.c_str();

    // Setup style
    style::setup_imgui();

    // register context
    context.reset(new imgui_context());
    imgui_existing_context = context;

    return context;
}

imgui_context::~imgui_context() noexcept
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
