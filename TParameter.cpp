#include "TParameter.h"


TParameter::TParameter(ParameterKind _kind) :kind (_kind)
{
}


TParameter::~TParameter()
{
}

Parameter_MapCell::Parameter_MapCell() : TParameter(ParameterKind_MapCell)
{
}

Parameter_MapCell::~Parameter_MapCell()
{
}

Parameter_Position::Parameter_Position() : TParameter(ParameterKind_Position)
{
}

Parameter_Position::~Parameter_Position()
{
}


Parameter_MoveObjectByUnit::Parameter_MoveObjectByUnit() : TParameter(ParameterKind_MoveObjectByUnit)
{
}

Parameter_MoveObjectByUnit::~Parameter_MoveObjectByUnit()
{
}

Parameter_GameObject::Parameter_GameObject() : TParameter(ParameterKind_GameObject)
{
}

Parameter_GameObject::~Parameter_GameObject()
{
}