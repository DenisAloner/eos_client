#pragma once
#include "GameObject.h"

class MapCell;

class Game_algorithm
{

public:

	//Функция проверяет, находятся ли объекты рядом (т.е. расстояние между их ближайшими клетками меньше 2)
	static bool check_distance(MapCell* a_cell, game_object_size_t& a_size, MapCell* b_cell, game_object_size_t& b_size);

};

