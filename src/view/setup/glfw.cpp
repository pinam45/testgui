//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//

// header
#include "glfw.hpp"

// project
#include <utils/log.hpp>

// external
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace
{
    std::weak_ptr<glfw_context> glfw_existing_context;
}

static void glfw_error_callback(int error, const char* description)
{
    const std::shared_ptr<spdlog::logger> logger = logging::get_logger("glfw");
    logger->error("glfw error {}: {}", error, description);
}

glfw_handle_t setup::glfw() noexcept
{
    std::shared_ptr<glfw_context> context = glfw_existing_context.lock();
    if(context)
    {
        return context;
    }

    const std::shared_ptr<spdlog::logger> logger = logging::get_logger("glfw");

    glfwSetErrorCallback(glfw_error_callback);
    if(!glfwInit())
    {
        logger->error("glfwInit failed");
        return nullptr;
    }

    // glsl version 330
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // register context
    context.reset(new glfw_context());
    glfw_existing_context = context;

    return context;
}

glfw_context::~glfw_context() noexcept
{
    glfwTerminate();
}
