#ifndef TACTIONMANAGER_H
#define	TACTIONMANAGER_H

#include <list>
#include <map>
#include "Event.h"
#include "GameObject.h"

class Parameter;
class Action;
class GameObject;
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

	Event<tag_t> add_item_event;
	Event<tag_t> remove_item_event;

	void add(GameTask* action);
	void remove();
	void remove(GameTask* item);
};

#endif //TACTIONMANAGER_H