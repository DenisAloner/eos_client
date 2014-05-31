#pragma once
#include <list>
#include "Definiton.h"

class GameObjectProperty
{
public:
	
	PropertyKind m_kind;

	GameObjectProperty(PropertyKind _kind);
	~GameObjectProperty(void);

};

