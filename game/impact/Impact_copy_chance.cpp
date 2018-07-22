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

Packer_generic& Impact_copy_chance::get_packer()
{
	return Packer<Impact_copy_chance>::Instance();
}
