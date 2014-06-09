#pragma once
#include "GameObjectProperty.h"
#include "GameObject.h"
#include <vector>

class GameObject;

class Property_Container :
	public GameObjectProperty
{
public:

	std::vector<GameObject*> m_items;

	dimension_t m_size;

	std::string m_name;

	Property_Container(int _Width, int _Height, std::string _Name);
	~Property_Container();

};

