//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//

// header
#include "LogViewer.hpp"

// project
#include <utils/log.hpp>
#include <view/style/colors.hpp>

// external
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

// C++ standard
#include <cassert>
#include <utility>

namespace
{
    constexpr spdlog::level::level_enum SPDLOG_LEVELS[]{
      spdlog::level::trace,
      spdlog::level::debug,
      spdlog::level::info,
      spdlog::level::warn,
      spdlog::level::err,
      spdlog::level::critical,
      spdlog::level::off,
    };
} // namespace

LogViewer::LogViewer() noexcept
    : _logs_colors()
    , _log_level(spdlog::level::trace)
    , _auto_scroll(true)
    , _wrap_lines(false)
    , _logger(logging::get_logger("LogViewer"))
{
    assert(STORED_LOGS != nullptr);

    _logs_colors[spdlog::level::trace] = style::color::log::trace;
    _logs_colors[spdlog::level::debug] = style::color::log::debug;
    _logs_colors[spdlog::level::info] = style::color::log::info;
    _logs_colors[spdlog::level::warn] = style::color::log::warn;
    _logs_colors[spdlog::level::err] = style::color::log::err;
    _logs_colors[spdlog::level::critical] = style::color::log::critical;
    _logs_colors[spdlog::level::off] = style::color::log::off;
}

void LogViewer::print() noexcept
{
    const ImGuiStyle& style = GImGui->Style;
    ImGui::PushItemWidth(ImGui::CalcTextSize(spdlog::level::to_string_view(spdlog::level::critical).data()).x
                         + ImGui::GetFrameHeight() + 2 * style.ItemInnerSpacing.x);
    if(ImGui::BeginCombo("display level", spdlog::level::to_string_view(_log_level).data()))
    {
        for(spdlog::level::level_enum level: SPDLOG_LEVELS)
        {
            const bool selected = (_log_level == level);
            if(ImGui::Selectable(spdlog::level::to_string_view(level).data(), selected))
            {
                _log_level = level;
                SPDLOG_LOGGER_TRACE(_logger, "log level set to {}", spdlog::level::to_string_view(_log_level));
            }
            if(selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();
    if(ImGui::Checkbox("Scroll to bottom", &_auto_scroll))
    {
        SPDLOG_LOGGER_TRACE(_logger, "scroll to bottom ", _auto_scroll ? "enabled" : "disabled");
    }
    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();
    if(ImGui::Checkbox("Wrap lines", &_wrap_lines))
    {
        SPDLOG_LOGGER_TRACE(_logger, "line wrapping ", _auto_scroll ? "enabled" : "disabled");
    }
    ImGui::Separator();

    if(ImGui::BeginChild("Logs",
                         ImVec2(ImGui::GetContentRegionAvail().x, 0),
                         false,
                         _wrap_lines ? ImGuiWindowFlags_None : ImGuiWindowFlags_HorizontalScrollbar))
    {
        if(_wrap_lines)
        {
            ImGui::PushTextWrapPos();
        }
        STORED_LOGS->iterate_on_logs(
          [this](const store_sink_mt::store_log& log)
          {
              if(log.level < _log_level)
              {
                  return true;
              }
              if(log.color_range_start < log.color_range_end)
              {
                  ImGui::TextUnformatted(log.txt.c_str(), log.txt.c_str() + log.color_range_start);
                  ImGui::SameLine(0, 0);
                  ImGui::PushStyleColor(ImGuiCol_Text, _logs_colors[log.level]);
                  ImGui::TextUnformatted(log.txt.c_str() + log.color_range_start,
                                         log.txt.c_str() + log.color_range_end);
                  ImGui::PopStyleColor();
                  ImGui::SameLine(0, 0);

                  // cut text for better alignment when line wrapping is enabled
                  const char* pos = log.txt.c_str() + log.color_range_end;
                  ImGui::TextUnformatted(pos, pos + 2);
                  if(pos + 2 < log.txt.c_str() + log.txt.size())
                  {
                      ImGui::SameLine(0, 0);
                      ImGui::TextUnformatted(pos + 2, log.txt.c_str() + log.txt.size());
                  }
              }
              else
              {
                  ImGui::TextUnformatted(log.txt.c_str());
              }
              return true;
          });
        if(_wrap_lines)
        {
            ImGui::PopTextWrapPos();
        }
    }
    if(_auto_scroll)
    {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();
}
