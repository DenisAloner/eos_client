#pragma once
#include "GameObject.h"

class MapCell;

class Game_algorithm
{

public:

	//������� ���������, ��������� �� ������� ����� (�.�. ���������� ����� �� ���������� �������� ������ 2)
	static bool check_distance(MapCell* a_cell, game_object_size_t& a_size, MapCell* b_cell, game_object_size_t& b_size);
	static bool check_distance(position_t* a_cell, game_object_size_t& a_size, position_t* b_cell, game_object_size_t& b_size);

};

