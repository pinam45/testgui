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
#include <glad/gl.h>
// glad before glfw
#include <GLFW/glfw3.h>

namespace
{
    std::weak_ptr<main_window_context> main_window_existing_context;
} // namespace

main_window_handle_t
setup::main_window(const glfw_handle_t& glfw_handle, std::string_view name, size_t width, size_t height) noexcept
{
    if(glfw_handle == nullptr)
    {
        return nullptr;
    }

    std::shared_ptr<main_window_context> context = main_window_existing_context.lock();
    if(context)
    {
        return context;
    }

    {
        std::shared_ptr<spdlog::logger> glfw_logger = logging::get_logger("glfw");

        context.reset(new main_window_context());
        context->glf_window =
          glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), name.data(), nullptr, nullptr);
        if(context->glf_window == nullptr)
        {
            glfw_logger->error("glfwCreateWindow failed");
            return nullptr;
        }

        glfwMakeContextCurrent(context->glf_window);
        glfwSwapInterval(1); // Enable vsync

        glfw_logger->debug("Created window with size {} x {}", width, height);
    }

    {
        std::shared_ptr<spdlog::logger> glad_logger = logging::get_logger("glad");

        // Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
        const int version = gladLoadGL(glfwGetProcAddress);
        if(version == 0)
        {
            glad_logger->error("gladLoadGL failed");
            return nullptr;
        }

        glad_logger->debug("Loaded OpenGL {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    }

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
