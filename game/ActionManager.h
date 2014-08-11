#ifndef TACTIONMANAGER_H
#define	TACTIONMANAGER_H

#include <list>
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
	std::list<GameTask*> m_items;

	ActionManager(void);
	~ActionManager(void);

	Event<GameTask*> add_item_event;
	Event<GameTask*> remove_item_event;

	void add(GameTask* Action);
	void remove();
	void remove(GameTask* item);
};

#endif //TACTIONMANAGER_H