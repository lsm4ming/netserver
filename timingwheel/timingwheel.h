#pragma once

#include <functional>
#include <unistd.h>
#include <ranges>
#include <thread>
#include <condition_variable>
#include <list>

template <typename T>
concept TaskJob = requires(T job) {
    { job() } -> std::same_as<void>;
    { job.run() } -> std::same_as<void>;
    { job.doWork() } -> std::same_as<void>;
};
class TimingWheel
{
private:
    int slots_;
    int interval_;
    int current_slot_;
    bool running_;
    std::thread worker_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::vector<std::list<std::function<void()>>> wheel_;

public:
    TimingWheel() = default;
    ~TimingWheel() = default;

    template <TaskJob T>
    void addTaskJob(const T &job, int delay)
    {
    }

private:
    void run();
};

// template <typename T>
// void clever_close(T &obj)
// {
//     if constexpr (requires() { obj(); })
//     {
//         obj();
//         return;
//     }
//     if constexpr (requires() { obj.close(); })
//     {
//         obj.close();
//         return;
//     }
//     if constexpr (requires() { obj.fd; })
//     {
//         ::close(obj.fd);
//         return;
//     }
//     ::close(obj);
// }