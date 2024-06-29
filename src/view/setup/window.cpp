//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//

// header
#include "window.hpp"

// project
#include <utils/log.hpp>
#include <view/setup/glfw.hpp>

// external
#include <GLFW/glfw3.h>

namespace
{
    std::weak_ptr<main_window_context> main_window_existing_context;
}

main_window_handle_t setup::main_window(
  [[maybe_unused]] glfw_handle_t glfw_handle,
  std::string_view name,
  size_t width,
  size_t height) noexcept
{
    std::shared_ptr<main_window_context> context = main_window_existing_context.lock();
    if(context)
    {
        return context;
    }

    const std::shared_ptr<spdlog::logger> logger = logging::get_logger("glfw");

    context.reset(new main_window_context());
    context->glf_window = glfwCreateWindow(width, height, name.data(), nullptr, nullptr);
    if(context->glf_window == nullptr)
    {
        logger->error("glfwCreateWindow failed");
        return nullptr;
    }

    glfwMakeContextCurrent(context->glf_window);
    glfwSwapInterval(1);// Enable vsync

    // register context
    main_window_existing_context = context;

    return context;
}

main_window_context::~main_window_context() noexcept
{
    if(glf_window != nullptr)
    {
        glfwDestroyWindow(glf_window);
    }
}
