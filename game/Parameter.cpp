#include "Parameter.h"


Parameter::Parameter(ParameterKind _kind) :m_kind (_kind)
{
}


Parameter::~Parameter()
{
}


Parameter_MapCell::Parameter_MapCell() : Parameter(ParameterKind_MapCell)
{

}


Parameter_MapCell::~Parameter_MapCell()
{
}

void Parameter_MapCell::save()
{
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::parameter_mapcell;
	fwrite(&t, sizeof(type_e), 1, file);
	Serialization_manager::instance().serialize(m_place);
}

void Parameter_MapCell::load()
{
	FILE* file = Serialization_manager::instance().m_file;
	m_place = dynamic_cast<MapCell*>(Serialization_manager::instance().deserialize());
}

Parameter_Position::Parameter_Position() : Parameter(ParameterKind_Position)
{
}


Parameter_Position::~Parameter_Position()
{
}

Parameter_Position::Parameter_Position(GameObject* object, MapCell* place, GameMap* map) :Parameter(ParameterKind_Position), m_object(object), m_place(place), m_map(map)
{
}

void Parameter_Position::reset_serialization_index()
{
	m_serialization_index = 0;
	if (m_object)
	{
		if (m_object->m_serialization_index > 1)
		{
			m_object->reset_serialization_index();
		}
	}
}

void Parameter_Position::save()
{
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::parameter_position;
	fwrite(&t, sizeof(type_e), 1, file);
	Serialization_manager::instance().serialize(m_object);
	Serialization_manager::instance().serialize(m_place);
	Serialization_manager::instance().serialize(m_map);
}

void Parameter_Position::load()
{
	FILE* file = Serialization_manager::instance().m_file;
	m_object = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
	m_place = dynamic_cast<MapCell*>(Serialization_manager::instance().deserialize());
	m_map = dynamic_cast<GameMap*>(Serialization_manager::instance().deserialize());
}


Parameter_MoveObjectByUnit::Parameter_MoveObjectByUnit() : Parameter(ParameterKind_MoveObjectByUnit)
{
}


Parameter_MoveObjectByUnit::~Parameter_MoveObjectByUnit()
{
}

void Parameter_MoveObjectByUnit::reset_serialization_index()
{
	m_serialization_index = 0;
	if (m_unit)
	{
		if (m_unit->m_serialization_index > 1)
		{
			m_unit->reset_serialization_index();
		}
	}
	if (m_object)
	{
		if (m_object->m_serialization_index > 1)
		{
			m_object->reset_serialization_index();
		}
	}
}

void Parameter_MoveObjectByUnit::save()
{
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::parameter_movegameobject;
	fwrite(&t, sizeof(type_e), 1, file);
	Serialization_manager::instance().serialize(m_unit);
	Serialization_manager::instance().serialize(m_object);
	Serialization_manager::instance().serialize(m_place);
	Serialization_manager::instance().serialize(m_map);
}

void Parameter_MoveObjectByUnit::load()
{
	FILE* file = Serialization_manager::instance().m_file;
	m_unit = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
	m_object = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
	m_place = dynamic_cast<MapCell*>(Serialization_manager::instance().deserialize());
	m_map = dynamic_cast<GameMap*>(Serialization_manager::instance().deserialize());
}


Parameter_destination::Parameter_destination() : Parameter(ParameterKind_Destination)
{
}

void Parameter_destination::reset_serialization_index()
{
	m_serialization_index = 0;
	if (m_unit)
	{
		if (m_unit->m_serialization_index > 1)
		{
			m_unit->reset_serialization_index();
		}
	}
	if (m_object)
	{
		if (m_object->m_serialization_index > 1)
		{
			m_object->reset_serialization_index();
		}
	}
	if (m_owner)
	{
		if (m_object->m_serialization_index > 1)
		{
			m_object->reset_serialization_index();
		}
	}
}

void Parameter_destination::save()
{
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::parameter_destination;
	fwrite(&t, sizeof(type_e), 1, file);
	Serialization_manager::instance().serialize(m_unit);
	Serialization_manager::instance().serialize(m_object);
	Serialization_manager::instance().serialize(m_owner);
}

void Parameter_destination::load()
{
	FILE* file = Serialization_manager::instance().m_file;
	m_unit = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
	m_object = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
	m_owner = dynamic_cast<Game_object_owner*>(Serialization_manager::instance().deserialize());
}

P_object::P_object() : Parameter(parameter_kind_object)
{
}

void P_object::reset_serialization_index()
{
	m_serialization_index = 0;
	if (m_object)
	{
		if (m_object->m_serialization_index > 1)
		{
			m_object->reset_serialization_index();
		}
	}
}

void P_object::save()
{
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::parameter_gameobject;
	fwrite(&t, sizeof(type_e), 1, file);
	Serialization_manager::instance().serialize(m_object);
}

void P_object::load()
{
	FILE* file = Serialization_manager::instance().m_file;
	m_object = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
}

P_object_owner::P_object_owner() : Parameter(parameter_kind_owner)
{
}

void P_object_owner::reset_serialization_index()
{
	if (m_cell)
	{
		if (m_cell->m_serialization_index > 1)
		{
			m_cell->reset_serialization_index();
		}
	}
}

void P_object_owner::save()
{
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::parameter_object_owner;
	fwrite(&t, sizeof(type_e), 1, file);
	Serialization_manager::instance().serialize(m_cell);
}

void P_object_owner::load()
{
	FILE* file = Serialization_manager::instance().m_file;
	m_cell = dynamic_cast<Game_object_owner*>(Serialization_manager::instance().deserialize());
}

P_unit_interaction::P_unit_interaction() : Parameter(parameter_kind_unit_interaction)
{
}

void P_unit_interaction::reset_serialization_index()
{
	m_serialization_index = 0;
	if (m_unit)
	{
		if (m_unit->m_serialization_index > 1)
		{
			m_unit->reset_serialization_index();
		}
	}
	if (m_object)
	{
		if (m_object->m_serialization_index > 1)
		{
			m_object->reset_serialization_index();
		}
	}
	if (m_unit_body_part)
	{
		if (m_unit_body_part->m_serialization_index > 1)
		{
			m_unit_body_part->reset_serialization_index();
		}
	}
}

void P_unit_interaction::save()
{
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::parameter_unit_interaction;
	fwrite(&t, sizeof(type_e), 1, file);
	Serialization_manager::instance().serialize(m_unit);
	Serialization_manager::instance().serialize(m_object);
	Serialization_manager::instance().serialize(m_unit_body_part);
}

void P_unit_interaction::load()
{
	FILE* file = Serialization_manager::instance().m_file;
	m_unit = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
	m_object = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
	m_unit_body_part = dynamic_cast<Object_part*>(Serialization_manager::instance().deserialize());
}

P_interaction_cell::P_interaction_cell() : Parameter(parameter_interaction_cell)
{
	m_unit=nullptr;
	m_object = nullptr;
	m_unit_body_part = nullptr;
	m_cell = nullptr;
}

void P_interaction_cell::reset_serialization_index()
{
	m_serialization_index = 0;
	if (m_unit)
	{
		if (m_unit->m_serialization_index > 1)
		{
			m_unit->reset_serialization_index();
		}
	}
	if (m_object)
	{
		if (m_object->m_serialization_index > 1)
		{
			m_object->reset_serialization_index();
		}
	}
	if (m_unit_body_part)
	{
		if (m_unit_body_part->m_serialization_index > 1)
		{
			m_unit_body_part->reset_serialization_index();
		}
	}
}

void P_interaction_cell::save()
{
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::parameter_interaction_cell;
	fwrite(&t, sizeof(type_e), 1, file);
	Serialization_manager::instance().serialize(m_unit);
	Serialization_manager::instance().serialize(m_object);
	Serialization_manager::instance().serialize(m_unit_body_part);
	Serialization_manager::instance().serialize(m_cell);
}

void P_interaction_cell::load()
{
	FILE* file = Serialization_manager::instance().m_file;
	m_unit = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
	m_object = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
	m_unit_body_part = dynamic_cast<Object_part*>(Serialization_manager::instance().deserialize());
	m_cell = dynamic_cast<MapCell*>(Serialization_manager::instance().deserialize());
}