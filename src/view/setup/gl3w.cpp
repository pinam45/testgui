//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//

// header
#include "gl3w.hpp"

// project
#include <utils/log.hpp>

// external
#include <GL/gl3w.h>

namespace
{
    std::weak_ptr<gl3w_context> gl3w_existing_context;
}// namespace

gl3w_handle_t setup::gl3w() noexcept
{
    std::shared_ptr<gl3w_context> context = gl3w_existing_context.lock();
    if(context)
    {
        return context;
    }

    const std::shared_ptr<spdlog::logger> logger = logging::get_logger("gl3w");

    if(gl3wInit() != 0)
    {
        logger->error("gl3wInit failed");
        return nullptr;
    }

    // register context
    context.reset(new gl3w_context());
    gl3w_existing_context = context;

    return context;
}
