#ifndef GAME_ALGORITHM_H
#define	GAME_ALGORITHM_H

#include "Definiton.h"
#include "GameObject.h"

class MapCell;

class Game_algorithm
{

public:

	//Функция проверяет, находятся ли объекты рядом (т.е. расстояние между их ближайшими клетками меньше 2)
	static bool check_distance(MapCell* a_cell, dimension3_t& a_size, MapCell* b_cell, dimension3_t& b_size);
	//static bool check_distance(position3_t* a_cell, dimension3_t& a_size, position3_t* b_cell, dimension3_t& b_size);

	//функция возвращает направление объекта, которое будет указывать на передаваемый объект
	static object_direction_e turn_to_object(GameObject* object, GameObject* turn_to);

	static object_direction_e turn_to_cell(GameObject* object, MapCell* c);
	static MapCell* step_in_direction(GameObject* object, object_direction_e d);
	static int get_angle(object_direction_e direction, int angle);

	//Функция возвращает минимальное количество поворотов между двумя ориентациями с учетов направления (по часовой стрелке для положительного результата)
	//static int get_rotation(object_direction_e start, object_direction_e end);
	static rotate_direction_e get_rotation(object_direction_e start, object_direction_e end);
};

#endif //GAME_ALGORITHM_H
