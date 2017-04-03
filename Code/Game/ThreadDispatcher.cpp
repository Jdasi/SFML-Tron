#include "ThreadDispatcher.h"

void ThreadDispatcher::postEvent(const std::function<void()>& _method)
{
    std::lock_guard<std::mutex> guard(mutex);
    queue.push(_method);
}



void ThreadDispatcher::executeDispatchedMethods()
{
    auto queue_copy = getQueueCopy();

    while (!queue_copy.empty())
    {
        auto method = queue_copy.front();
        method();
        queue_copy.pop();
    }
}



void ThreadDispatcher::clearQueue()
{
    while (!queue.empty())
    {
        queue.pop();
    }
}



std::queue<std::function<void()>> ThreadDispatcher::getQueueCopy()
{
    std::lock_guard<std::mutex> guard(mutex);

    auto queue_copy = queue;
    clearQueue();

    return queue_copy;
}
