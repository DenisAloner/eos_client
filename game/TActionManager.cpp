#include "TActionManager.h"

GameTask::GameTask(TAction* Action = nullptr, TParameter* Parameter = nullptr)
{
	m_action=Action;
	m_parameter=Parameter;
}

TActionManager::TActionManager(void)
{
}

TActionManager::~TActionManager(void)
{
}

void TActionManager::add(GameTask* item)
{
	m_items.push_back(item);
	add_item_event(item);
}

void TActionManager::remove()
{
	if (!m_items.empty())
	{
		remove_item_event(m_items.front());
		m_items.pop_front();
	}
}

void TActionManager::remove(GameTask* item)
{
	remove_item_event(item);
	m_items.remove(item);
}