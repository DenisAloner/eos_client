#include "GameObjectProperty.h"


GameObjectProperty::GameObjectProperty(property_e _kind = property_e::none) : m_kind(_kind)
{
}


GameObjectProperty::~GameObjectProperty(void)
{
}

GameObjectParameter::GameObjectParameter(property_e kind = property_e::none, float value = 0) :GameObjectProperty(kind), m_value(value){}