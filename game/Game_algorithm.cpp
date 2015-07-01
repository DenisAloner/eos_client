#include "Game_algorithm.h"

bool Game_algorithm::check_distance(MapCell* a_cell, game_object_size_t& a_size, MapCell* b_cell, game_object_size_t& b_size)
{
	return ((abs(a_cell->x - b_cell->x) < 2 || abs(a_cell->x - (b_cell->x + (b_size.x - 1))) < 2 || abs((a_cell->x + (a_size.x - 1)) - b_cell->x) < 2 || abs((a_cell->x + (a_size.x - 1)) - (b_cell->x + (b_size.x - 1))) < 2) && (abs(a_cell->y - b_cell->y) < 2 || abs(a_cell->y - (b_cell->y + (b_size.y - 1))) < 2 || abs((a_cell->y + (a_size.y - 1)) - b_cell->y) < 2 || abs((a_cell->y + (a_size.y - 1)) - (b_cell->y + (b_size.y - 1))) < 2));
}

bool Game_algorithm::check_distance(position_t* a_cell, game_object_size_t& a_size, position_t* b_cell, game_object_size_t& b_size)
{
	return ((abs(a_cell->x - b_cell->x) < 2 || abs(a_cell->x - (b_cell->x + (b_size.x - 1))) < 2 || abs((a_cell->x + (a_size.x - 1)) - b_cell->x) < 2 || abs((a_cell->x + (a_size.x - 1)) - (b_cell->x + (b_size.x - 1))) < 2) && (abs(a_cell->y - b_cell->y) < 2 || abs(a_cell->y - (b_cell->y + (b_size.y - 1))) < 2 || abs((a_cell->y + (a_size.y - 1)) - b_cell->y) < 2 || abs((a_cell->y + (a_size.y - 1)) - (b_cell->y + (b_size.y - 1))) < 2));
}