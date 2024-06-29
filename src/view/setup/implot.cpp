//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//

// header
#include "implot.hpp"

// external
#include <implot.h>

namespace
{
    std::weak_ptr<implot_context> implot_existing_context;
}

implot_handle_t setup::implot() noexcept
{
    std::shared_ptr<implot_context> context = implot_existing_context.lock();
    if(context)
    {
        return context;
    }

    // Create implot context
    ImPlot::CreateContext();

    // register context
    context.reset(new implot_context());
    implot_existing_context = context;

    return context;
}

implot_context::~implot_context() noexcept
{
    ImPlot::DestroyContext();
}
