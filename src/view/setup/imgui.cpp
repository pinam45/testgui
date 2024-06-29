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
#include <view/style/imgui.hpp>

// external
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <imgui_internal.h>

namespace
{
    std::weak_ptr<imgui_context> imgui_existing_context;
}

imgui_handle_t setup::imgui(main_window_handle_t main_window_handle, gl3w_handle_t gl3w_handle) noexcept
{
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
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;// Enable docking
    io.ConfigDockingWithShift = true;                // hold shift to use docking
    io.IniFilename = nullptr;                        // disable .ini saving
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

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
