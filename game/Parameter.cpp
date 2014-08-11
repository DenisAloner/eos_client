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


Parameter_MoveObjectByUnit::Parameter_MoveObjectByUnit() : Parameter(ParameterKind_MoveObjectByUnit)
{
}


Parameter_MoveObjectByUnit::~Parameter_MoveObjectByUnit()
{
}


Parameter_GameObject::Parameter_GameObject() : Parameter(ParameterKind_GameObject)
{
}


Parameter_GameObject::~Parameter_GameObject()
{
}
