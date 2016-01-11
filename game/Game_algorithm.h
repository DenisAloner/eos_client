#pragma once
#include "Definiton.h"
#include "GameObject.h"

class MapCell;

class Game_algorithm
{

public:

	//������� ���������, ��������� �� ������� ����� (�.�. ���������� ����� �� ���������� �������� ������ 2)
	static bool check_distance(MapCell* a_cell, game_object_size_t& a_size, MapCell* b_cell, game_object_size_t& b_size);
	static bool check_distance(position_t* a_cell, game_object_size_t& a_size, position_t* b_cell, game_object_size_t& b_size);

	//������� ���������� ����������� �������, ������� ����� ��������� �� ������������ ������
	static object_direction_e turn_to_object(GameObject* object, GameObject* turn_to);

	static object_direction_e turn_to_cell(GameObject* object, MapCell* c);
	static MapCell* step_in_direction(GameObject* object, object_direction_e d);
};

