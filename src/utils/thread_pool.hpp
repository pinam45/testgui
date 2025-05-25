//
// Copyright (c) 2024 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

// C++ standard
#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

class thread_pool
{
public:
    thread_pool(const thread_pool&) noexcept = delete;
    thread_pool(thread_pool&&) noexcept = delete;
    thread_pool& operator=(const thread_pool&) noexcept = delete;
    thread_pool& operator=(thread_pool&&) noexcept = delete;

    explicit thread_pool(size_t thread_number = std::thread::hardware_concurrency()) noexcept;
    ~thread_pool() noexcept;

    [[nodiscard]] size_t tasks_queued() const noexcept;
    [[nodiscard]] size_t tasks_running() const noexcept;
    [[nodiscard]] size_t tasks_total() const noexcept;
    [[nodiscard]] size_t thread_number() const noexcept;

    template<typename Func, typename... Args>
    void exec(Func&& task, Args&&... args) noexcept;

    template<typename Func,
             typename... Args,
             typename Res = std::invoke_result_t<std::decay_t<Func>, std::decay_t<Args>...>>
    [[nodiscard]] std::future<Res> submit(Func&& task, Args&&... args) noexcept;

    void wait() noexcept;

    template<typename R, typename P>
    bool wait_for(const std::chrono::duration<R, P>& duration) noexcept;

private:
    std::atomic<bool> _running = false;

    std::condition_variable _task_available{};
    std::condition_variable _task_done{};

    std::queue<std::function<void()>> _tasks{};
    size_t _tasks_count = 0;
    mutable std::mutex _tasks_mutex{};

    std::vector<std::thread> _threads{};

    void worker() noexcept;
};

inline thread_pool::thread_pool(size_t thread_number) noexcept
{
    _running = true;
    _threads.resize(std::max(static_cast<size_t>(1ul), thread_number));
    for(std::thread& thread: _threads)
    {
        thread = std::thread(&thread_pool::worker, this);
    }
}

inline thread_pool::~thread_pool() noexcept
{
    // end all tasks
    {
        std::unique_lock<std::mutex> tasks_lock(_tasks_mutex);
        _task_done.wait(tasks_lock, [this]() { return (_tasks_count == 0); });
    }

    // end threads
    _running = false;
    _task_available.notify_all();
    for(std::thread& thread: _threads)
    {
        thread.join();
    }
}

inline size_t thread_pool::tasks_queued() const noexcept
{
    const std::scoped_lock tasks_lock(_tasks_mutex);
    return _tasks.size();
}

inline size_t thread_pool::tasks_running() const noexcept
{
    const std::scoped_lock tasks_lock(_tasks_mutex);
    return _tasks_count - _tasks.size();
}

inline size_t thread_pool::tasks_total() const noexcept
{
    return _tasks_count;
}

inline size_t thread_pool::thread_number() const noexcept
{
    return _threads.size();
}

template<typename Func, typename... Args, typename Res>
std::future<Res> thread_pool::submit(Func&& task, Args&&... args) noexcept
{
    std::function<Res()> task_function = std::bind(std::forward<Func>(task), std::forward<Args>(args)...);
    std::shared_ptr<std::promise<Res>> promise = std::make_shared<std::promise<Res>>();
    std::future<Res> future = promise->get_future();
    exec(
      [task_function = std::move(task_function), promise = std::move(promise)]() noexcept
      {
          try
          {
              if constexpr(std::is_void_v<Res>)
              {
                  std::invoke(task_function);
                  promise->set_value();
              }
              else
              {
                  auto tmp = std::invoke(task_function);
                  promise->set_value(tmp);
              }
          }
          catch(...)
          {
              try
              {
                  promise->set_exception(std::current_exception());
              }
              catch(...)
              {
              }
          }
      });
    return future;
}

template<typename Func, typename... Args>
void thread_pool::exec(Func&& task, Args&&... args) noexcept
{
    std::function<void()> task_function = std::bind(std::forward<Func>(task), std::forward<Args>(args)...);
    {
        const std::scoped_lock tasks_lock(_tasks_mutex);
        _tasks.push(std::move(task_function));
        ++_tasks_count;
    }
    _task_available.notify_one();
}

inline void thread_pool::wait() noexcept
{
    std::unique_lock<std::mutex> tasks_lock(_tasks_mutex);
    _task_done.wait(tasks_lock, [this]() { return (_tasks_count == 0); });
}

template<typename R, typename P>
bool thread_pool::wait_for(const std::chrono::duration<R, P>& duration) noexcept
{
    std::unique_lock<std::mutex> tasks_lock(_tasks_mutex);
    return _task_done.wait_for(tasks_lock, duration, [this]() { return (_tasks_count == 0); });
}

inline void thread_pool::worker() noexcept
{
    while(_running)
    {
        std::function<void()> task;
        std::unique_lock<std::mutex> tasks_lock(_tasks_mutex);
        _task_available.wait(tasks_lock, [this]() { return !_tasks.empty() || !_running; });
        if(_running)
        {
            task = std::move(_tasks.front());
            _tasks.pop();
            tasks_lock.unlock();
            task();
            tasks_lock.lock();
            --_tasks_count;
            _task_done.notify_all();
        }
    }
}
