#include "TActionManager.h"

GameTask::GameTask(TAction* Action = nullptr, TParameter* Parameter = nullptr)
{
	this->Action=Action;
	this->Parameter=Parameter;
}

TActionManager::TActionManager(void)
{
}

TActionManager::~TActionManager(void)
{
}

void TActionManager::Add(GameTask* item)
{
	Actions.push_back(item);
	AddEvent(item);
}

void TActionManager::Remove()
{
	if (!Actions.empty())
	{
		RemoveEvent(Actions.front());
		Actions.pop_front();
	}
}

void TActionManager::Remove(GameTask* item)
{
	RemoveEvent(item);
	Actions.remove(item);
}