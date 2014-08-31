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

P_cell::P_cell() : Parameter(parameter_kind_cell)
{
}

P_object::P_object() : Parameter(parameter_kind_object)
{
}

P_inventory_cell::P_inventory_cell() : Parameter(parameter_kind_inventory_cell)
{
}