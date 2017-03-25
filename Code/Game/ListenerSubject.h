#pragma once
#include <vector>

/* Base class for the subject of an observer pattern (listener class).
 * The template keeps the base class generic, allowing for specialised behaviour
 * to be implemented by derived types.
 *
 * It's up to the implemented class to iterate over the listeners vector and
 * call the appropriate methods.
 */
template <typename T>
class ListenerSubject
{
public:
    ListenerSubject() = default;
    virtual ~ListenerSubject() = default;

    /* Attach a listener/observer to this subject.
     *
     * This does not take ownership of the passed pointer, so it's up to 
     * the caller to keep this pointer alive, and delete it appropriately.
     */
    void attachListener(T* _listener)
    {
        listeners.push_back(_listener);
    }

    /* Detach a listener/observer from this object.
     * If the passed pointer was not previously attached, nothing happens.
     *
     * This will not delete object, just detach it.
     */
    void detachListener(T* _listener)
    {
        listeners.erase(std::remove
            (listeners.begin(), listeners.end(), _listener), listeners.end());
    }

    /* Detatch all attached listeners/observers.
     *
     * This will not delete the objects, just detach them.
     */
    void detachAllListeners()
    {
        listeners.clear();
    }

protected:
    std::vector<T*> listeners;

};
