#pragma once
#include "ActionsDescription.h"
#include "TParameter.h"


class TParameter;
class TAction;

class GameTask
{
public:

	GameTask(TAction* Action, TParameter* Parameter);

	TAction* m_action;
	TParameter* m_parameter;
};

class TActionManager
{
public:
	
	std::list<GameTask*> m_items;

	TActionManager(void);
	~TActionManager(void);

	Event<GameTask*> add_item_event;
	Event<GameTask*> remove_item_event;

	void add(GameTask* Action);
	void remove();
	void remove(GameTask* item);
};

