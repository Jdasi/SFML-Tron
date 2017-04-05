#include "ThreadDispatcher.h"

// Posts an event to the dispatcher's queue.
void ThreadDispatcher::postEvent(const std::function<void()>& _method)
{
    std::lock_guard<std::mutex> guard(mutex);
    queue.push(_method);
}



/* Executes all of the posted events after making a copy of the queue.
 * This allows for events to be posted even while the dispatcher is
 * working its way through the list.
 */
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



// Returns a copy of the queue.
std::queue<std::function<void()>> ThreadDispatcher::getQueueCopy()
{
    std::lock_guard<std::mutex> guard(mutex);

    auto queue_copy = queue;
    clearQueue();

    return queue_copy;
}
