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

class GameObjectParameter : public GameObjectProperty
{
public:

	GameObjectParameter(property_e kind,float value);
	float m_value;

};


#endif //GAMEOBJECTPROPERTY_H