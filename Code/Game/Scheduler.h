#pragma once
#include "SimpleTimer.h"

#include <functional>
#include <list>

class ScheduledTask
{
public:
    ScheduledTask(std::function<void()> _method, const double _timeout)
        : method(_method)
        , timeout(_timeout)
    {
    }

    ~ScheduledTask() = default;

    std::function<void()> method;
    double timeout;
};

class Scheduler
{
public:
    Scheduler();
    ~Scheduler() = default;

    void update();

    void invoke(std::function<void()> _method, double _time);

private:
    void executeScheduledTasks();

    std::list<ScheduledTask> scheduled_tasks;
    SimpleTimer timer;
    double total_time;

};
