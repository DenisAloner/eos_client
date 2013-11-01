
#ifndef EVENT_BASE_H
#define EVENT_BASE_H

#include "Delegate.h"

class EventBase
{
	friend class RealIterator;

private:

	struct Node
	{
		Node *_next;
		Delegate _data;

		Node(Delegate data, Node *next): _data(data), _next(next)
		{
		}

		~Node()
		{
			if (_next) delete _next;
		}
	};

	Node *_head;


public:

	EventBase()
		: _head(0)
	{
	}

protected:

	class Iterator
	{
	public:
		virtual bool HasNext() const = 0;
		virtual Delegate& GetNext() = 0;
	};

	void Push(const Delegate &data);
	void Remove(const Delegate &data);
	Iterator* GetIterator() const;
};

#endif