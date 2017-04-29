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

std::u16string Impact_copy_chance::get_description()
{
	return u"Impact_copy_chance";
}

void Impact_copy_chance::save()
{
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::impact_copy_chance;
	fwrite(&t, sizeof(type_e), 1, file);
	fwrite(&m_chance, sizeof(int), 1, file);
	Serialization_manager::instance().serialize(m_value);
}

void Impact_copy_chance::load()
{
	FILE* file = Serialization_manager::instance().m_file;
	fread(&m_chance, sizeof(int), 1, file);
	m_value = dynamic_cast<Object_interaction*>(Serialization_manager::instance().deserialize());
}