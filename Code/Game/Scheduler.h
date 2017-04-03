#pragma once
#include <list>

#include "ScheduledTask.h"
#include "SimpleTimer.h"

class Scheduler
{
public:
    Scheduler();
    ~Scheduler() = default;

    void update();
    void invoke(const std::function<void()>& _method, const double _time);

private:
    void executeScheduledTasks();

    std::list<ScheduledTask> scheduled_tasks;
    SimpleTimer timer;
    double total_time;

};
