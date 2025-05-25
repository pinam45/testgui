//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//

// header
#include "ImSpinnerDemo.hpp"

// project
#include <utils/log.hpp>

ImSpinnerDemo::ImSpinnerDemo() noexcept : _logger(logging::get_logger("ImSpinnerDemo"))
{
}

void ImSpinnerDemo::print() noexcept
{
    ImSpinner::demoSpinners();
}
