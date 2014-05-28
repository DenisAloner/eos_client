#include "Property_Container.h"


Property_Container::Property_Container(int _Width, int _Height, std::string _Name) : GameObjectProperty(PropertyKind_Container), Width(_Width), Height(_Height), Name(_Name)
{
	Items.resize(Height*Width, nullptr);
}

Property_Container::~Property_Container()
{
}
