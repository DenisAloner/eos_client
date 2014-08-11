#ifndef EVENT_H
#define	EVENT_H

#include "Definiton.h"
#include <iostream>
#include <functional>
#include <map>


template<class EventArgType>
class Event
{
public:
	typedef std::function<void(EventArgType const&)> EventSignature;
	typedef int EventHandle;

public:
	Event(): eventHandle(0) {};

	EventHandle AddHandler(EventSignature const& f) {
		functions.insert(std::make_pair(++eventHandle, f));
		return eventHandle;
	}

	void RemoveHandler(EventHandle handle) {
		functions.erase(handle);
	}

	void Clear() {
		functions.clear();
	}

	Event& operator+=(EventSignature const& f) {
		AddHandler(f);
		return *this;
	}

	Event& operator-=(EventHandle handle) {
		RemoveHandler(handle);
		return *this;
	}

	void operator()(EventArgType const& arg) {
		//for each(auto const& f in functions) in Visual Studio
		for(auto const& f : functions) {
			f.second(arg);
		}
	}

private:
	EventHandle eventHandle;
	std::map<int, EventSignature> functions;
};

#endif //EVENT_H