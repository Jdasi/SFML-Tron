#pragma once
#include <functional>

struct ScheduledTask
{
    ScheduledTask(const std::function<void()>& _method, const double _timeout)
        : method(_method)
        , timeout(_timeout)
    {
    }

    std::function<void()> method;
    double timeout;
};
