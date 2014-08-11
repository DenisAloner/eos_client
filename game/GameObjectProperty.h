#ifndef GAMEOBJECTPROPERTY_H
#define	GAMEOBJECTPROPERTY_H

#include <list>
#include "Definiton.h"


class GameObjectProperty
{
public:
	property_e m_kind;

	GameObjectProperty(property_e _kind);
	~GameObjectProperty(void);
};

#endif //GAMEOBJECTPROPERTY_H