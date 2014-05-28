#pragma once
#include "GameObjectProperty.h"
#include "GameObject.h"#
#include <vector>

class GameObject;

class Property_Container :
	public GameObjectProperty
{
public:

	std::vector<GameObject*> Items;

	int Width;
	int Height;

	std::string Name;

	Property_Container(int _Width, int _Height, std::string _Name);
	~Property_Container();

};

