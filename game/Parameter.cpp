#include "Parameter.h"

void Parameter_argument_t::set(GameObject* value)
{
	if (kind == type_e::gameobject)
	{
		m_object = value;
	}
	else
	{
		LOG(FATAL) << "“ип аргумента дл€ Parameter не соотвутствует ожидаемому";
	}
};

void Parameter_argument_t::set(GameMap* value)
{
	if (kind == type_e::gamemap)
	{
		m_map = value;
	}
	else
	{
		LOG(FATAL) << "“ип аргумента дл€ Parameter не соотвутствует ожидаемому";
	}
};

void Parameter_argument_t::set(Game_object_owner* value)
{
	switch (kind)
	{
	case::type_e::mapcell:
	{
		if (value->m_kind == entity_e::cell)
		{
			m_cell = static_cast<MapCell*>(value);
			return;
		}
		break;
	}
	case::type_e::object_part:
	{
		if (value->m_kind == entity_e::body_part)
		{
			m_part = static_cast<Object_part*>(value);
			return;
		}
		break;
	}
	case::type_e::inventory_cell:
	{
		if (value->m_kind == entity_e::inventory_cell)
		{
			return;
		}
		break;
	}
	case::type_e::object_owner:
	{
		m_owner = value;
		return;
	}
	}
	LOG(FATAL) << "“ип аргумента дл€ Parameter не соотвутствует ожидаемому";
};

void Parameter_argument_t::set(object_direction_e value)
{
	if (kind == type_e::direction)
	{
		m_direction = value;
	}
	else
	{
		LOG(FATAL) << "“ип аргумента дл€ Parameter не соотвутствует ожидаемому";
	}
};

void Parameter_argument_t::reset_serialization_index()
{
	switch (kind)
	{
	case::type_e::gameobject:
	{
		if (m_object)
		{
			if (m_object->m_serialization_index > 1)
			{
				m_object->reset_serialization_index();
			}
		}
		break;
	}
	case::type_e::object_owner:
	{
		if (m_owner)
		{
			if (m_owner->m_serialization_index > 1)
			{
				m_owner->reset_serialization_index();
			}
		}
		break;
	}
	case::type_e::mapcell:
	{
		if (m_cell)
		{
			if (m_cell->m_serialization_index > 1)
			{
				m_cell->reset_serialization_index();
			}
		}
		break;
	}
	case::type_e::inventory_cell:
	{
		if (m_owner)
		{
			if (m_owner->m_serialization_index > 1)
			{
				m_owner->reset_serialization_index();
			}
		}
		break;
	}
	case::type_e::object_part:
	{
		if (m_part)
		{
			if (m_part->m_serialization_index > 1)
			{
				m_part->reset_serialization_index();
			}
		}
		break;
	}
	}
};

void Parameter_argument_t::save()
{
	switch (kind)
	{
	case::type_e::object_owner:
	{
		Serialization_manager::instance().serialize(m_owner);
		break;
	}
	case::type_e::mapcell:
	{
		Serialization_manager::instance().serialize(m_cell);
		break;
	}
	case::type_e::inventory_cell:
	{
		Serialization_manager::instance().serialize(m_owner);
		break;
	}
	case::type_e::object_part:
	{
		Serialization_manager::instance().serialize(m_part);
		break;
	}
	case::type_e::gameobject:
	{
		Serialization_manager::instance().serialize(m_object);
		break;
	}
	case::type_e::gamemap:
	{
		Serialization_manager::instance().serialize(m_map);
		break;
	}
	case::type_e::direction:
	{
		FILE* file = Serialization_manager::instance().m_file;
		fwrite(&m_direction, sizeof(object_direction_e), 1, file);
		break;
	}
	}
};

Parameter_argument_t::operator bool()
{ 
	switch (kind)
	{
	case::type_e::object_owner:
	{
		return m_owner;
		break;
	}
	case::type_e::mapcell:
	{
		return m_cell;
		break;
	}
	case::type_e::inventory_cell:
	{
		return m_owner;
		break;
	}
	case::type_e::object_part:
	{
		return m_part;
		break;
	}
	case::type_e::gameobject:
	{
		return m_object;
		break;
	}
	case::type_e::gamemap:
	{
		return m_map;
		break;
	}
	case::type_e::direction:
	{
		return m_direction != object_direction_e::none;
		break;
	}
	default:
	{
		LOG(FATAL) << "Ќеверный тип дл€ Parameter_argument_t::operator bool()";
		break;
	}
	}
};

void Parameter_argument_t::init()
{
	switch (kind)
	{
	case::type_e::object_owner:
	{
		m_owner = nullptr;
		break;
	}
	case::type_e::mapcell:
	{
		m_cell = nullptr;
		break;
	}
	case::type_e::inventory_cell:
	{
		m_owner = nullptr;
		break;
	}
	case::type_e::object_part:
	{
		m_part = nullptr;
		break;
	}
	case::type_e::gameobject:
	{
		m_object = nullptr;
		break;
	}
	case::type_e::gamemap:
	{
		m_map = nullptr;
		break;
	}
	case::type_e::direction:
	{
		m_direction = object_direction_e::none;
		break;
	}
	}
};

void Parameter_argument_t::load()
{
	switch (kind)
	{
	case::type_e::object_owner:
	{
		m_owner = dynamic_cast<Game_object_owner*>(Serialization_manager::instance().deserialize());
		break;
	}
	case::type_e::mapcell:
	{
		m_cell = dynamic_cast<MapCell*>(Serialization_manager::instance().deserialize());
		break;
	}
	case::type_e::inventory_cell:
	{
		m_owner = dynamic_cast<Game_object_owner*>(Serialization_manager::instance().deserialize());
		break;
	}
	case::type_e::object_part:
	{
		m_part = dynamic_cast<Object_part*>(Serialization_manager::instance().deserialize());
		break;
	}
	case::type_e::gameobject:
	{
		m_object = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
		break;
	}
	case::type_e::gamemap:
	{
		m_map = dynamic_cast<GameMap*>(Serialization_manager::instance().deserialize());
		break;
	}
	case::type_e::direction:
	{
		FILE* file = Serialization_manager::instance().m_file;
		fread(&m_direction, sizeof(object_direction_e), 1, file);
		break;
	}
	}
};

Parameter::Parameter(parameter_type_e kind) :m_kind(kind)
{
	m_args = nullptr;
	switch (kind)
	{
	case::parameter_type_e::parameter_kind_object:
	{
		m_size = 1;
		m_args = new Parameter_argument_t[m_size];
		m_args[0].kind = type_e::gameobject;
		break;
	}
	case::parameter_type_e::ParameterKind_MapCell:
	{  
		m_size = 1;
		m_args = new Parameter_argument_t[m_size];
		m_args[0].kind = type_e::mapcell;
		break;
	}
	case::parameter_type_e::parameter_kind_owner:
	{
		m_size = 1;
		m_args = new Parameter_argument_t[m_size];
		m_args[0].kind = type_e::object_owner;
		break;
	}
	case::parameter_type_e::ParameterKind_Position:
	{
		m_size = 3;
		m_args = new Parameter_argument_t[m_size];
		m_args[0].kind = type_e::gameobject;
		m_args[1].kind = type_e::mapcell;
		m_args[2].kind = type_e::gamemap;
		break;
	}
	case::parameter_type_e::ParameterKind_Destination:
	{
		m_size = 3;
		m_args = new Parameter_argument_t[m_size];
		m_args[0].kind = type_e::gameobject;
		m_args[1].kind = type_e::gameobject;
		m_args[2].kind = type_e::object_owner;
		break;
	}
	case::parameter_type_e::parameter_interaction_cell:
	{
		m_size = 4;
		m_args = new Parameter_argument_t[m_size];
		m_args[0].kind = type_e::gameobject;
		m_args[1].kind = type_e::gameobject;
		m_args[2].kind = type_e::object_part;
		m_args[3].kind = type_e::mapcell;
		break;
	}
	}
	if (m_args)
	{
		for (std::size_t i = 0; i < m_size; i++)
		{
			m_args[i].init();
		}
	}
}

void Parameter::reset_serialization_index()
{
	m_serialization_index = 0;
	for (int i = 0; i < m_size; i++)
	{
		m_args[i].reset_serialization_index();
	}
}

void Parameter::save()
{
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::parameters;
	fwrite(&t, sizeof(type_e), 1, file);
	fwrite(&m_kind, sizeof(parameter_type_e), 1, file);
	for (int i = 0; i < m_size; i++)
	{
		m_args[i].save();
	}
}

void Parameter::load()
{
	for (int i = 0; i < m_size; i++)
	{
		m_args[i].load();
	}
}

Parameter* Parameter::clone()
{
	Parameter* result_p = new Parameter(m_kind);
	Parameter& result_r = *result_p;
	for (int i = 0; i < m_size; i++)
	{
		result_r[i] = m_args[i];
	}
	return result_p;
}

//Parameter::Parameter(ParameterKind _kind) :m_kind (_kind)
//{
//}
//
//
//Parameter::~Parameter()
//{
//}
//
//
//Parameter_MapCell::Parameter_MapCell() : Parameter(ParameterKind_MapCell),m_place(nullptr)
//{
//
//}
//
//
//Parameter_MapCell::~Parameter_MapCell()
//{
//}
//
//void Parameter_MapCell::save()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	type_e t = type_e::parameter_mapcell;
//	fwrite(&t, sizeof(type_e), 1, file);
//	Serialization_manager::instance().serialize(m_place);
//}
//
//void Parameter_MapCell::load()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	m_place = dynamic_cast<MapCell*>(Serialization_manager::instance().deserialize());
//}
//
//Parameter_Position::Parameter_Position() : Parameter(ParameterKind_Position)
//{
//	m_object = nullptr;
//	m_place = nullptr;
//	m_map = nullptr;
//}
//
//
//Parameter_Position::~Parameter_Position()
//{
//}
//
//Parameter_Position::Parameter_Position(GameObject* object, MapCell* place, GameMap* map) :Parameter(ParameterKind_Position), m_object(object), m_place(place), m_map(map)
//{
//}
//
//void Parameter_Position::reset_serialization_index()
//{
//	m_serialization_index = 0;
//	if (m_object)
//	{
//		if (m_object->m_serialization_index > 1)
//		{
//			m_object->reset_serialization_index();
//		}
//	}
//}
//
//void Parameter_Position::save()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	type_e t = type_e::parameter_position;
//	fwrite(&t, sizeof(type_e), 1, file);
//	Serialization_manager::instance().serialize(m_object);
//	Serialization_manager::instance().serialize(m_place);
//	Serialization_manager::instance().serialize(m_map);
//}
//
//void Parameter_Position::load()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	m_object = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
//	m_place = dynamic_cast<MapCell*>(Serialization_manager::instance().deserialize());
//	m_map = dynamic_cast<GameMap*>(Serialization_manager::instance().deserialize());
//}
//
//
//Parameter_MoveObjectByUnit::Parameter_MoveObjectByUnit() : Parameter(ParameterKind_MoveObjectByUnit)
//{
//}
//
//
//Parameter_MoveObjectByUnit::~Parameter_MoveObjectByUnit()
//{
//}
//
//void Parameter_MoveObjectByUnit::reset_serialization_index()
//{
//	m_serialization_index = 0;
//	if (m_unit)
//	{
//		if (m_unit->m_serialization_index > 1)
//		{
//			m_unit->reset_serialization_index();
//		}
//	}
//	if (m_object)
//	{
//		if (m_object->m_serialization_index > 1)
//		{
//			m_object->reset_serialization_index();
//		}
//	}
//}
//
//void Parameter_MoveObjectByUnit::save()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	type_e t = type_e::parameter_movegameobject;
//	fwrite(&t, sizeof(type_e), 1, file);
//	Serialization_manager::instance().serialize(m_unit);
//	Serialization_manager::instance().serialize(m_object);
//	Serialization_manager::instance().serialize(m_place);
//	Serialization_manager::instance().serialize(m_map);
//}
//
//void Parameter_MoveObjectByUnit::load()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	m_unit = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
//	m_object = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
//	m_place = dynamic_cast<MapCell*>(Serialization_manager::instance().deserialize());
//	m_map = dynamic_cast<GameMap*>(Serialization_manager::instance().deserialize());
//}
//
//
//Parameter_destination::Parameter_destination() : Parameter(ParameterKind_Destination), m_unit(nullptr), m_owner(nullptr), m_object(nullptr) {}
//
//void Parameter_destination::reset_serialization_index()
//{
//	m_serialization_index = 0;
//	if (m_unit)
//	{
//		if (m_unit->m_serialization_index > 1)
//		{
//			m_unit->reset_serialization_index();
//		}
//	}
//	if (m_object)
//	{
//		if (m_object->m_serialization_index > 1)
//		{
//			m_object->reset_serialization_index();
//		}
//	}
//	if (m_owner)
//	{
//		if (m_object->m_serialization_index > 1)
//		{
//			m_object->reset_serialization_index();
//		}
//	}
//}
//
//void Parameter_destination::save()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	type_e t = type_e::parameter_destination;
//	fwrite(&t, sizeof(type_e), 1, file);
//	Serialization_manager::instance().serialize(m_unit);
//	Serialization_manager::instance().serialize(m_object);
//	Serialization_manager::instance().serialize(m_owner);
//}
//
//void Parameter_destination::load()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	m_unit = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
//	m_object = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
//	m_owner = dynamic_cast<Game_object_owner*>(Serialization_manager::instance().deserialize());
//}
//
//P_object::P_object() : Parameter(parameter_kind_object),m_object(nullptr)
//{
//}
//
//void P_object::reset_serialization_index()
//{
//	m_serialization_index = 0;
//	if (m_object)
//	{
//		if (m_object->m_serialization_index > 1)
//		{
//			m_object->reset_serialization_index();
//		}
//	}
//}
//
//void P_object::save()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	type_e t = type_e::parameter_gameobject;
//	fwrite(&t, sizeof(type_e), 1, file);
//	Serialization_manager::instance().serialize(m_object);
//}
//
//void P_object::load()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	m_object = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
//}
//
//P_object_owner::P_object_owner() : Parameter(parameter_kind_owner),m_cell(nullptr)
//{
//}
//
//void P_object_owner::reset_serialization_index()
//{
//	if (m_cell)
//	{
//		if (m_cell->m_serialization_index > 1)
//		{
//			m_cell->reset_serialization_index();
//		}
//	}
//}
//
//void P_object_owner::save()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	type_e t = type_e::parameter_object_owner;
//	fwrite(&t, sizeof(type_e), 1, file);
//	Serialization_manager::instance().serialize(m_cell);
//}
//
//void P_object_owner::load()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	m_cell = dynamic_cast<Game_object_owner*>(Serialization_manager::instance().deserialize());
//}
//
//P_unit_interaction::P_unit_interaction() : Parameter(parameter_kind_unit_interaction)
//{
//	m_unit = nullptr;
//	m_object = nullptr;
//	m_unit_body_part = nullptr;
//}
//
//void P_unit_interaction::reset_serialization_index()
//{
//	m_serialization_index = 0;
//	if (m_unit)
//	{
//		if (m_unit->m_serialization_index > 1)
//		{
//			m_unit->reset_serialization_index();
//		}
//	}
//	if (m_object)
//	{
//		if (m_object->m_serialization_index > 1)
//		{
//			m_object->reset_serialization_index();
//		}
//	}
//	if (m_unit_body_part)
//	{
//		if (m_unit_body_part->m_serialization_index > 1)
//		{
//			m_unit_body_part->reset_serialization_index();
//		}
//	}
//}
//
//void P_unit_interaction::save()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	type_e t = type_e::parameter_unit_interaction;
//	fwrite(&t, sizeof(type_e), 1, file);
//	Serialization_manager::instance().serialize(m_unit);
//	Serialization_manager::instance().serialize(m_object);
//	Serialization_manager::instance().serialize(m_unit_body_part);
//}
//
//void P_unit_interaction::load()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	m_unit = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
//	m_object = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
//	m_unit_body_part = dynamic_cast<Object_part*>(Serialization_manager::instance().deserialize());
//}
//
//P_interaction_cell::P_interaction_cell() : Parameter(parameter_interaction_cell)
//{
//	m_unit=nullptr;
//	m_object = nullptr;
//	m_unit_body_part = nullptr;
//	m_cell = nullptr;
//}
//
//void P_interaction_cell::reset_serialization_index()
//{
//	m_serialization_index = 0;
//	if (m_unit)
//	{
//		if (m_unit->m_serialization_index > 1)
//		{
//			m_unit->reset_serialization_index();
//		}
//	}
//	if (m_object)
//	{
//		if (m_object->m_serialization_index > 1)
//		{
//			m_object->reset_serialization_index();
//		}
//	}
//	if (m_unit_body_part)
//	{
//		if (m_unit_body_part->m_serialization_index > 1)
//		{
//			m_unit_body_part->reset_serialization_index();
//		}
//	}
//}
//
//void P_interaction_cell::save()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	type_e t = type_e::parameter_interaction_cell;
//	fwrite(&t, sizeof(type_e), 1, file);
//	Serialization_manager::instance().serialize(m_unit);
//	Serialization_manager::instance().serialize(m_object);
//	Serialization_manager::instance().serialize(m_unit_body_part);
//	Serialization_manager::instance().serialize(m_cell);
//}
//
//void P_interaction_cell::load()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	m_unit = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
//	m_object = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
//	m_unit_body_part = dynamic_cast<Object_part*>(Serialization_manager::instance().deserialize());
//	m_cell = dynamic_cast<MapCell*>(Serialization_manager::instance().deserialize());
//}
//
//P_bow_shoot::P_bow_shoot() : Parameter(parameter_bow_shoot)
//{
//	m_unit = nullptr;
//	m_object = nullptr;
//	m_unit_body_part = nullptr;
//	m_cell = nullptr;
//}
//
//void P_bow_shoot::reset_serialization_index()
//{
//	m_serialization_index = 0;
//	if (m_unit)
//	{
//		if (m_unit->m_serialization_index > 1)
//		{
//			m_unit->reset_serialization_index();
//		}
//	}
//	if (m_object)
//	{
//		if (m_object->m_serialization_index > 1)
//		{
//			m_object->reset_serialization_index();
//		}
//	}
//	if (m_unit_body_part)
//	{
//		if (m_unit_body_part->m_serialization_index > 1)
//		{
//			m_unit_body_part->reset_serialization_index();
//		}
//	}
//}
//
//void P_bow_shoot::save()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	type_e t = type_e::parameter_bow_shoot;
//	fwrite(&t, sizeof(type_e), 1, file);
//	Serialization_manager::instance().serialize(m_unit);
//	Serialization_manager::instance().serialize(m_object);
//	Serialization_manager::instance().serialize(m_unit_body_part);
//	Serialization_manager::instance().serialize(m_cell);
//}
//
//void P_bow_shoot::load()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	m_unit = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
//	m_object = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
//	m_unit_body_part = dynamic_cast<Object_part*>(Serialization_manager::instance().deserialize());
//	m_cell = dynamic_cast<MapCell*>(Serialization_manager::instance().deserialize());
//}
//
//Parameter_direction::Parameter_direction() : Parameter(parameter_direction), m_object(nullptr), m_direction(object_direction_e::none) {};
//
//Parameter_direction::Parameter_direction(GameObject* object, object_direction_e direction) : Parameter(parameter_direction), m_object(object), m_direction(direction) {};
//
//void Parameter_direction::reset_serialization_index()
//{
//	m_serialization_index = 0;
//	if (m_object)
//	{
//		if (m_object->m_serialization_index > 1)
//		{
//			m_object->reset_serialization_index();
//		}
//	}
//}
//
//void Parameter_direction::save()
//{
//	LOG(INFO) << "«апись параметра направлени€";
//	FILE* file = Serialization_manager::instance().m_file;
//	type_e t = type_e::parameter_direction;
//	fwrite(&t, sizeof(type_e), 1, file);
//	Serialization_manager::instance().serialize(m_object);
//	fwrite(&m_direction, sizeof(object_direction_e), 1, file);
//	LOG(INFO) << " онец записи параметра направлени€";
//}
//
//void Parameter_direction::load()
//{
//	FILE* file = Serialization_manager::instance().m_file;
//	LOG(INFO) << "in";
//	m_object = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
//	LOG(INFO) << "out";
//	fread(&m_direction, sizeof(object_direction_e), 1, file);
//}