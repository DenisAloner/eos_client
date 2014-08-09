#include "Property_Container.h"


Property_Container::Property_Container(int _Width, int _Height, std::string _Name) : GameObjectProperty(property_e::container),m_size(dimension_t(_Width,_Height)), m_name(_Name)
{
	m_items.resize(m_size.w*m_size.h, nullptr);
}

Property_Container::~Property_Container()
{
}
