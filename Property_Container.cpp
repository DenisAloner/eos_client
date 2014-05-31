#include "Property_Container.h"


Property_Container::Property_Container(int _Width, int _Height, std::string _Name) : GameObjectProperty(PropertyKind_Container),m_size(GSize(_Width,_Height)), m_name(_Name)
{
	m_items.resize(m_size.x*m_size.y, nullptr);
}

Property_Container::~Property_Container()
{
}
