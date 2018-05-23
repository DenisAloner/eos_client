#ifndef ACTIONMANAGER_H
#define	ACTIONMANAGER_H

#include <list>
#include <map>
#include "Event.h"

class Parameter;
class Action;

class GameTask
{
public:
	GameTask(Action* Action, Parameter* Parameter);

	Action* m_action;
	Parameter* m_parameter;
};

class ActionManager
{
public:

	bool m_is_remove;

	std::list<GameTask*> m_items;

	ActionManager(void);
	~ActionManager(void);

	Event<tag_t> add_item_event;
	Event<tag_t> remove_item_event;

	void add(GameTask* action);
	void add_front(GameTask* action);
	void remove();
	void remove(GameTask* item);
	GameTask* get_task();
};

#endif //ACTIONMANAGER_H