#include "ActionManager.h"


GameTask::GameTask(Action* Action = nullptr, Parameter* Parameter = nullptr)
{
	m_action=Action;
	m_parameter=Parameter;
}


ActionManager::ActionManager(void)
{
}


ActionManager::~ActionManager(void)
{
}


void ActionManager::add(GameObject* unit, GameTask* action)
{

	m_items[unit].push_back(action);
	add_item_event(tag_t(unit,action));
}


void ActionManager::remove(GameObject* unit)
{
	if (!m_items[unit].empty())
	{
		remove_item_event(tag_t(unit,m_items[unit].front()));
		m_items[unit].pop_front();
	}
}


void ActionManager::remove(GameObject* unit, GameTask* item)
{
	remove_item_event(tag_t(unit,item));
	m_items[unit].remove(item);
}
