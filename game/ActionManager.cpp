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


void ActionManager::add(GameTask* item)
{
	m_items.push_back(item);
	add_item_event(item);
}


void ActionManager::remove()
{
	if (!m_items.empty())
	{
		remove_item_event(m_items.front());
		m_items.pop_front();
	}
}


void ActionManager::remove(GameTask* item)
{
	remove_item_event(item);
	m_items.remove(item);
}
