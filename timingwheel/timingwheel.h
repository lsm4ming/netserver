#pragma once

#include <functional>
#include <unistd.h>
#include <ranges>
#include <thread>
#include <condition_variable>
#include <list>

template <typename T>
concept TaskJob =
    requires(T job) {
        { job() } -> std::same_as<void>;
    } ||
    requires(T job) {
        { job.run() } -> std::same_as<void>;
    } ||
    requires(T job) {
        { job.doWork() } -> std::same_as<void>;
    };

template <TaskJob T>
static void executeTask(T task)
{
    if constexpr (requires { task(); })
    {
        task();
    }
    else if constexpr (requires { task.run(); })
    {
        task.run();
    }
    else if constexpr (requires { task.doWork(); })
    {
        task.doWork();
    }
}

template <TaskJob T>
class TimingWheel
{
private:
    int slots_{};
    int interval_{};
    int current_slot_{0};
    bool running_{false};
    std::thread worker_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::vector<std::list<T>> wheel_;

public:
    TimingWheel(int slots = 16, int interval = 1) : slots_(slots), interval_(interval)
    {
        for (size_t i = 0; i < this->slots_; i++)
        {
            wheel_.push_back(std::list<T>());
        }
    };

    ~TimingWheel() = default;

    void addTaskJob(const T job, int delay)
    {
        int ticks = delay / interval_;
        int slot = (current_slot_ + ticks) % slots_;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            wheel_[slot].push_back(job);
        }
    }

    void start()
    {
        this->running_ = true;
        worker_ = std::thread([this]
                              { this->run(); });
    }

    void stop()
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            this->running_ = false;
        }
        this->cv_.notify_all();
        if (this->worker_.joinable())
        {
            this->worker_.join();
        }
    }

private:
    void run()
    {
        while (true)
        {
            {
                std::unique_lock<std::mutex> lock(mutex_);
                if (!running_)
                    break;
            }

            std::this_thread::sleep_for(std::chrono::seconds(interval_));

            std::list<T> tasks_to_run;
            {
                std::lock_guard<std::mutex> lock(mutex_);
                if (!running_)
                    break;

                tasks_to_run.swap(wheel_[current_slot_]);
                current_slot_ = (current_slot_ + 1) % slots_;
                std::cout << "Slot " << current_slot_ << " executed.\n";
            }

            for (auto &task : tasks_to_run)
            {
                executeTask(task);
            }
        }
    }
};