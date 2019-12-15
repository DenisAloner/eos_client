#include "Parameter.h"
#include "GameObject.h"

void Parameter_argument_t::set(GameObject* value)
{
    if (kind == type_e::gameobject) {
        m_object = value;
    } else {
        Logger::instance().critical("Тип аргумента для Parameter не соотвутствует ожидаемому");
    }
};

void Parameter_argument_t::set(Game_object_owner* value)
{
    switch (kind) {
    case ::type_e::mapcell: {
        if (value->m_kind == entity_e::cell) {
            m_cell = static_cast<MapCell*>(value);
            return;
        }
        break;
    }
    case ::type_e::object_part: {
        if (value->m_kind == entity_e::body_part) {
            m_part = static_cast<ObjectPart*>(value);
            return;
        }
        break;
    }
    case ::type_e::inventory_cell: {
        if (value->m_kind == entity_e::inventory_cell) {
            return;
        }
        break;
    }
    case ::type_e::object_owner: {
        m_owner = value;
        return;
    }
    }
    Logger::instance().critical("Тип аргумента для Parameter не соотвутствует ожидаемому");
};

void Parameter_argument_t::set(object_direction_e value)
{
    if (kind == type_e::direction) {
        m_direction = value;
    } else {
        Logger::instance().critical("Тип аргумента для Parameter не соотвутствует ожидаемому");
    }
};

Parameter_argument_t::operator bool()
{
    switch (kind) {
    case ::type_e::object_owner: {
        return m_owner;
    }
    case ::type_e::mapcell: {
        return m_cell;
    }
    case ::type_e::inventory_cell: {
        return m_owner;
    }
    case ::type_e::object_part: {
        return m_part;
    }
    case ::type_e::gameobject: {
        return m_object;
    }
    //case::type_e::object_part_group:
    //{
    //	return m_part_group;
    //	break;
    //}
    case ::type_e::direction: {
        return m_direction != object_direction_e::none;
    }
    default: {
        Logger::instance().critical("Неверный тип для Parameter_argument_t::operator bool()");
        break;
    }
    }
};

void Parameter_argument_t::init()
{
    switch (kind) {
    case ::type_e::object_owner: {
        m_owner = nullptr;
        break;
    }
    case ::type_e::mapcell: {
        m_cell = nullptr;
        break;
    }
    case ::type_e::inventory_cell: {
        m_owner = nullptr;
        break;
    }
    case ::type_e::object_part: {
        m_part = nullptr;
        break;
    }
    case ::type_e::gameobject: {
        m_object = nullptr;
        break;
    }
    //case::type_e::object_part_group:
    //{
    //	m_part_group = nullptr;
    //	break;
    //}
    case ::type_e::direction: {
        m_direction = object_direction_e::none;
        break;
    }
    }
};

void Parameter_argument_t::reset_serialization_index()
{
    switch (kind) {
    case ::type_e::gameobject: {
        if (m_object) {
            if (m_object->m_serialization_index > 0) {
                m_object->reset_serialization_index();
            }
        }
        break;
    }
    case ::type_e::object_owner: {
        if (m_owner) {
            if (m_owner->m_serialization_index > 0) {
                m_owner->reset_serialization_index();
            }
        }
        break;
    }
    case ::type_e::mapcell: {
        if (m_cell) {
            if (m_cell->m_serialization_index > 0) {
                m_cell->reset_serialization_index();
            }
        }
        break;
    }
    case ::type_e::inventory_cell: {
        if (m_owner) {
            if (m_owner->m_serialization_index > 0) {
                m_owner->reset_serialization_index();
            }
        }
        break;
    }
    case ::type_e::object_part: {
        if (m_part) {
            if (m_part->m_serialization_index > 0) {
                m_part->reset_serialization_index();
            }
        }
        break;
    }
        //case::type_e::object_part_group:
        //{
        //	if (m_part_group)
        //	{
        //		for (auto group = m_part_group->begin(); group != m_part_group->end(); ++group)
        //		{
        //			for (auto part = group->begin(); part != group->end(); ++part)
        //			{
        //				if ((*part)->m_serialization_index > 1)
        //				{
        //					(*part)->reset_serialization_index();
        //				}
        //			}
        //		}
        //	}
        //	break;
        //}
    }
};

Parameter::Parameter(parameter_type_e kind)
    : m_kind(kind)
{
    m_args = nullptr;
    switch (kind) {
    case ::parameter_type_e::object: {
        m_size = 1;
        m_args = new Parameter_argument_t[m_size];
        m_args[0].kind = type_e::gameobject;
        break;
    }
    case ::parameter_type_e::mapcell: {
        m_size = 1;
        m_args = new Parameter_argument_t[m_size];
        m_args[0].kind = type_e::mapcell;
        break;
    }
    case ::parameter_type_e::owner: {
        m_size = 1;
        m_args = new Parameter_argument_t[m_size];
        m_args[0].kind = type_e::object_owner;
        break;
    }
    case ::parameter_type_e::direction: {
        m_size = 2;
        m_args = new Parameter_argument_t[m_size];
        m_args[0].kind = type_e::gameobject;
        m_args[1].kind = type_e::direction;
        break;
    }
    case ::parameter_type_e::position: {
        m_size = 2;
        m_args = new Parameter_argument_t[m_size];
        m_args[0].kind = type_e::gameobject;
        m_args[1].kind = type_e::mapcell;
        break;
    }
    case ::parameter_type_e::unit_interaction: {
        m_size = 2;
        m_args = new Parameter_argument_t[m_size];
        m_args[0].kind = type_e::gameobject;
        m_args[1].kind = type_e::gameobject;
        break;
    }
    case ::parameter_type_e::destination: {
        m_size = 4;
        m_args = new Parameter_argument_t[m_size];
        m_args[0].kind = type_e::gameobject;
        m_args[1].kind = type_e::gameobject;
        m_args[2].kind = type_e::object_owner;
        m_args[3].kind = type_e::object_part;
        break;
    }
    case ::parameter_type_e::interaction_cell: {
        m_size = 4;
        m_args = new Parameter_argument_t[m_size];
        m_args[0].kind = type_e::gameobject;
        m_args[1].kind = type_e::gameobject;
        m_args[2].kind = type_e::object_part;
        m_args[3].kind = type_e::mapcell;
        break;
    }
    case ::parameter_type_e::bow_shoot: {
        m_size = 5;
        m_args = new Parameter_argument_t[m_size];
        m_args[0].kind = type_e::gameobject;
        m_args[1].kind = type_e::gameobject;
        m_args[2].kind = type_e::object_part;
        m_args[3].kind = type_e::object_part;
        m_args[4].kind = type_e::mapcell;
        break;
    }
    }
    if (m_args) {
        for (std::size_t i = 0; i < m_size; i++) {
            m_args[i].init();
        }
    }
}

IJSONSERIALIZABLE_IMPL(Parameter);

void Parameter::reset_serialization_index()
{
    m_serialization_index = 0;
    for (int i = 0; i < m_size; i++) {
        m_args[i].reset_serialization_index();
    }
}

iPacker& Parameter::get_packer()
{
    return Packer<Parameter>::instance();
}

Parameter* Parameter::clone()
{
    Parameter* result_p = new Parameter(m_kind);
    Parameter& result_r = *result_p;
    for (int i = 0; i < m_size; i++) {
        result_r[i] = m_args[i];
    }
    return result_p;
}

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
