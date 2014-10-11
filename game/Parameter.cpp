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


Parameter_Position::Parameter_Position() : Parameter(ParameterKind_Position)
{
}


Parameter_Position::~Parameter_Position()
{
}

Parameter_Position::Parameter_Position(GameObject* object, MapCell* place, GameMap* map) :Parameter(ParameterKind_Position), m_object(object), m_place(place), m_map(map)
{
}

Parameter_MoveObjectByUnit::Parameter_MoveObjectByUnit() : Parameter(ParameterKind_MoveObjectByUnit)
{
}


Parameter_MoveObjectByUnit::~Parameter_MoveObjectByUnit()
{
}


Parameter_GameObject::Parameter_GameObject() : Parameter(ParameterKind_GameObject)
{
}

Parameter_destination::Parameter_destination() : Parameter(ParameterKind_Destination)
{
}

P_object_owner::P_object_owner(ParameterKind kind) : Parameter(kind)
{
}

P_object::P_object() : Parameter(parameter_kind_object)
{
}

P_unit_interaction::P_unit_interaction() : Parameter(parameter_kind_unit_interaction)
{
}