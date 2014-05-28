#pragma once
#include "ActionsDescription.h"
#include "TParameter.h"


class TParameter;
class TAction;

class GameTask
{
public:

	GameTask(TAction* Action, TParameter* Parameter);

	TAction* Action;
	TParameter* Parameter;
};

class TActionManager
{
public:
	
	std::list<GameTask*> Actions;

	TActionManager(void);
	~TActionManager(void);

	Event<GameTask*> AddEvent;
	Event<GameTask*> RemoveEvent;

	void Add(GameTask* Action);
	void Remove();
	void Remove(GameTask* item);
};

