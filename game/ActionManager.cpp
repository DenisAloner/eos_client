#include "ActionManager.h"
#include "Action.h"

GameTask::GameTask(Action* action = nullptr, Parameter* parameter = nullptr)
{
	m_action=action;
	m_parameter=parameter;
}


ActionManager::ActionManager()
{
}


ActionManager::~ActionManager()
{
}


void ActionManager::add(GameTask* action)
{

	m_items.push_back(action);
	add_item_event(tag_t(action));
}

void ActionManager::add_front(GameTask* action)
{

	m_items.push_front(action);
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

GameTask* ActionManager::get_task()
{
	m_is_remove = true;
	while (!m_items.empty())
	{
		auto parent = m_items.front();
		GameTask* child = nullptr;
		if (parent->m_action->get_child(child))
		{
			if (child)
			{
				m_is_remove = false;
				return child;
			}
			else remove();
		}
		else return parent;
	}
	return nullptr;
}