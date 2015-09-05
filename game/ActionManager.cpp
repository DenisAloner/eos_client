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


void ActionManager::add(GameTask* action)
{

	m_items.push_back(action);
	add_item_event(tag_t(action));
}


void ActionManager::remove()
{
	if (!m_items.empty())
	{
		remove_item_event(tag_t(m_items.front()));
		m_items.pop_front();
	}
}


void ActionManager::remove(GameTask* item)
{
	remove_item_event(tag_t(item));
	m_items.remove(item);
}
