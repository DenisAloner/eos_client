#include "Impact_copy_chance.h"


Impact_copy_chance::Impact_copy_chance()
{
}


Impact_copy_chance::~Impact_copy_chance()
{
}

Object_interaction* Impact_copy_chance::clone()
{
	if (rand() % 100 < m_chance)
	{
		return m_value->clone();
	}
	return nullptr;
}

void Impact_copy_chance::description(std::list<std::string>* info, int level)
{
	/*info->push_back(std::string(level, '.') + "<тип параметра:" + Application::instance().m_game_object_manager->get_effect_string(m_subtype) + ">:");
	m_value->description(info, level + 1);*/
}

std::string Impact_copy_chance::get_description()
{
	return "Impact_copy_chance";
}
