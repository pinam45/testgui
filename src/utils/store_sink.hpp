//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

// external
#include <spdlog/details/null_mutex.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// C++ standard
#include <mutex>
#include <thread>

template<typename Mutex>
class store_sink final : public spdlog::sinks::base_sink<Mutex>
{
public:
    struct store_log final
    {
        spdlog::level::level_enum level;
        std::string txt;
        size_t color_range_start;
        size_t color_range_end;

        store_log(spdlog::level::level_enum level,
                  const std::string& txt,
                  size_t color_range_start,
                  size_t color_range_end) noexcept;
        store_log(spdlog::level::level_enum level,
                  std::string&& txt,
                  size_t color_range_start,
                  size_t color_range_end) noexcept;
    };

    store_sink() = default;
    store_sink(const store_sink&) = delete;
    store_sink(store_sink&&) = delete;
    store_sink& operator=(const store_sink&) = delete;
    store_sink& operator=(store_sink&&) = delete;
    ~store_sink() = default;

    template<typename T>
    void iterate_on_logs(T callable);

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override;
    void flush_() override;

private:
    std::vector<store_log> logs;
    std::unique_ptr<spdlog::pattern_formatter> formatter = std::make_unique<spdlog::pattern_formatter>();
};

using store_sink_mt = store_sink<std::mutex>;
using store_sink_st = store_sink<spdlog::details::null_mutex>;

template<typename Mutex>
store_sink<Mutex>::store_log::store_log(spdlog::level::level_enum level_,
                                        const std::string& txt_,
                                        size_t color_range_start_,
                                        size_t color_range_end_) noexcept
    : level(level_)
    , txt(txt_)
    , color_range_start(color_range_start_)
    , color_range_end(color_range_end_)
{
}

template<typename Mutex>
store_sink<Mutex>::store_log::store_log(spdlog::level::level_enum level_,
                                        std::string&& txt_,
                                        size_t color_range_start_,
                                        size_t color_range_end_) noexcept
    : level(level_)
    , txt(txt_)
    , color_range_start(color_range_start_)
    , color_range_end(color_range_end_)
{
}

template<typename Mutex>
template<typename T>
void store_sink<Mutex>::iterate_on_logs(T callable)
{
    std::lock_guard<Mutex> lock(spdlog::sinks::base_sink<Mutex>::mutex_);
    for(const auto& log: logs)
    {
        if(!callable(log))
        {
            break;
        }
    }
}

template<typename Mutex>
void store_sink<Mutex>::sink_it_(const spdlog::details::log_msg& msg)
{
    spdlog::memory_buf_t formatted;
    formatter->format(msg, formatted);
    logs.emplace_back(
      msg.level, std::string(formatted.data(), formatted.size()), msg.color_range_start, msg.color_range_end);
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
}

template<typename Mutex>
void store_sink<Mutex>::flush_()
{
}
