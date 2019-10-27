#ifndef EVENT_H
#define EVENT_H

#include "Definiton.h"
#include <functional>
#include <map>

template <class EventArgType>
class Event {
public:
    typedef std::function<void(EventArgType const&)> event_signature;
    typedef int event_handle;

    Event()
        : event_handle_(0) {};

    event_handle add_handler(event_signature const& f)
    {
        functions_.insert(std::make_pair(++event_handle_, f));
        return event_handle_;
    }

    void remove_handler(event_handle handle)
    {
        functions_.erase(handle);
    }

    void clear()
    {
        functions_.clear();
    }

    Event& operator+=(event_signature const& f)
    {
        add_handler(f);
        return *this;
    }

    Event& operator-=(const event_handle handle)
    {
        remove_handler(handle);
        return *this;
    }

    void operator()(EventArgType const& arg)
    {
        for (auto const& f : functions_) {
            f.second(arg);
        }
    }

private:
    event_handle event_handle_;
    std::map<int, event_signature> functions_;
};

class GUI_connectable_i {
public:
    Event<VoidEventArgs> event_update;
    virtual void get_actions_list(std::list<Action_helper_t>& value) = 0;
};

#endif //EVENT_H