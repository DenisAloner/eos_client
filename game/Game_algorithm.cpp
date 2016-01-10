#include "Game_algorithm.h"

bool Game_algorithm::check_distance(MapCell* a_cell, game_object_size_t& a_size, MapCell* b_cell, game_object_size_t& b_size)
{
	return ((abs(a_cell->x - b_cell->x) < 2 || abs(a_cell->x - (b_cell->x + (b_size.x - 1))) < 2 || abs((a_cell->x + (a_size.x - 1)) - b_cell->x) < 2 || abs((a_cell->x + (a_size.x - 1)) - (b_cell->x + (b_size.x - 1))) < 2) && (abs(a_cell->y - b_cell->y) < 2 || abs(a_cell->y - (b_cell->y - (b_size.y - 1))) < 2 || abs((a_cell->y - (a_size.y - 1)) - b_cell->y) < 2 || abs((a_cell->y - (a_size.y - 1)) - (b_cell->y - (b_size.y - 1))) < 2));
}

bool Game_algorithm::check_distance(position_t* a_cell, game_object_size_t& a_size, position_t* b_cell, game_object_size_t& b_size)
{
	return ((abs(a_cell->x - b_cell->x) < 2 || abs(a_cell->x - (b_cell->x + (b_size.x - 1))) < 2 || abs((a_cell->x + (a_size.x - 1)) - b_cell->x) < 2 || abs((a_cell->x + (a_size.x - 1)) - (b_cell->x + (b_size.x - 1))) < 2) && (abs(a_cell->y - b_cell->y) < 2 || abs(a_cell->y - (b_cell->y - (b_size.y - 1))) < 2 || abs((a_cell->y - (a_size.y - 1)) - b_cell->y) < 2 || abs((a_cell->y - (a_size.y - 1)) - (b_cell->y - (b_size.y - 1))) < 2));
}

object_direction_e Game_algorithm::turn_to_object(GameObject* object, GameObject* turn_to)
{
	f2dvector_t v(turn_to->cell()->x + turn_to->m_active_state->m_size.x*0.5F - (object->cell()->x + object->m_active_state->m_size.x*0.5F), turn_to->cell()->y - turn_to->m_active_state->m_size.y*0.5F - (object->cell()->y - object->m_active_state->m_size.y*0.5F));
	float cs = abs(v.y / sqrt(v.x*v.x+v.y*v.y));
	if (v.y > 0.0F)
	{
		if (v.x > 0.0F)
		{
			if (cs > cos22_5)
			{
				return object_direction_e::top;
			}
			if (cs < cos67_5)
			{
				return object_direction_e::right;
			}
			else
			{
				return object_direction_e::topright;
			}
		}
		else
		{
			if (cs > cos22_5)
			{
				return object_direction_e::top;
			}
			if (cs < cos67_5)
			{
				return object_direction_e::left;
			}
			else
			{
				return object_direction_e::topleft;
			}
		}
	}
	else
	{
		if (v.x > 0.0F)
		{
			if (cs > cos22_5)
			{
				return object_direction_e::down;
			}
			if (cs < cos67_5)
			{
				return object_direction_e::right;
			}
			else
			{
				return object_direction_e::downright;
			}
		}
		else
		{
			if (cs > cos22_5)
			{
				return object_direction_e::down;
			}
			if (cs < cos67_5)
			{
				return object_direction_e::left;
			}
			else
			{
				return object_direction_e::downleft;
			}
		}
	}
}

object_direction_e Game_algorithm::turn_to_cell(GameObject* object, MapCell* c)
{
	f2dvector_t v(c->x + 0.5F - (object->cell()->x + object->m_active_state->m_size.x*0.5F), c->y - 0.5F - (object->cell()->y - object->m_active_state->m_size.y*0.5F));
	float cs = abs(v.y / sqrt(v.x*v.x + v.y*v.y));
	if (v.y > 0.0F)
	{
		if (v.x > 0.0F)
		{
			if (cs > cos22_5)
			{
				return object_direction_e::top;
			}
			if (cs < cos67_5)
			{
				return object_direction_e::right;
			}
			else
			{
				return object_direction_e::topright;
			}
		}
		else
		{
			if (cs > cos22_5)
			{
				return object_direction_e::top;
			}
			if (cs < cos67_5)
			{
				return object_direction_e::left;
			}
			else
			{
				return object_direction_e::topleft;
			}
		}
	}
	else
	{
		if (v.x > 0.0F)
		{
			if (cs > cos22_5)
			{
				return object_direction_e::down;
			}
			if (cs < cos67_5)
			{
				return object_direction_e::right;
			}
			else
			{
				return object_direction_e::downright;
			}
		}
		else
		{
			if (cs > cos22_5)
			{
				return object_direction_e::down;
			}
			if (cs < cos67_5)
			{
				return object_direction_e::left;
			}
			else
			{
				return object_direction_e::downleft;
			}
		}
	}
}