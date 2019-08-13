#include "Game_algorithm.h"

bool Game_algorithm::check_distance(MapCell* a_cell, dimension3_t& a_size, MapCell* b_cell, dimension3_t& b_size)
{
	return (abs(a_cell->x - b_cell->x) < 2
		    || abs(a_cell->x - (b_cell->x + (b_size.dx - 1))) < 2
		    || abs((a_cell->x + (a_size.dx - 1)) - b_cell->x) < 2
		    || abs((a_cell->x + (a_size.dx - 1)) - (b_cell->x + (b_size.dx - 1))) < 2)
		&& (abs(a_cell->y - b_cell->y) < 2
			|| abs(a_cell->y - (b_cell->y - (b_size.dy - 1))) < 2
			|| abs((a_cell->y - (a_size.dy - 1)) - b_cell->y) < 2
			|| abs((a_cell->y - (a_size.dy - 1)) - (b_cell->y - (b_size.dy - 1))) < 2)
		&& (abs(a_cell->z - b_cell->z) < 2
			|| abs(a_cell->z - (b_cell->z + (b_size.dz - 1))) < 2
			|| abs((a_cell->z + (a_size.dz - 1)) - b_cell->z) < 2
			|| abs((a_cell->z + (a_size.dz - 1)) - (b_cell->z + (b_size.dz - 1))) < 2);
}

//bool Game_algorithm::check_distance(position3_t* a_cell, dimension3_t& a_size, position3_t* b_cell, dimension3_t& b_size)
//{
//	return (abs(a_cell->x - b_cell->x) < 2
//		    || abs(a_cell->x - (b_cell->x + (b_size.dx - 1))) < 2
//		    || abs((a_cell->x + (a_size.dx - 1)) - b_cell->x) < 2
//		    || abs((a_cell->x + (a_size.dx - 1)) - (b_cell->x + (b_size.dx - 1))) < 2)
//		&& (abs(a_cell->y - b_cell->y) < 2
//			|| abs(a_cell->y - (b_cell->y - (b_size.dy - 1))) < 2
//			|| abs((a_cell->y - (a_size.dy - 1)) - b_cell->y) < 2
//			|| abs((a_cell->y - (a_size.dy - 1)) - (b_cell->y - (b_size.dy - 1))) < 2)
//		&& (abs(a_cell->z - b_cell->z) < 2
//			|| abs(a_cell->z - (b_cell->z + (b_size.dz - 1))) < 2
//			|| abs((a_cell->z + (a_size.dz - 1)) - b_cell->z) < 2
//			|| abs((a_cell->z + (a_size.dz - 1)) - (b_cell->z + (b_size.dz - 1))) < 2);
//}

object_direction_e Game_algorithm::turn_to_object(GameObject* object, GameObject* turn_to)
{
	f2dvector_t v(turn_to->cell()->x + turn_to->m_active_state->m_size.dx*0.5F - (object->cell()->x + object->m_active_state->m_size.dx*0.5F), turn_to->cell()->y - turn_to->m_active_state->m_size.dy*0.5F - (object->cell()->y - object->m_active_state->m_size.dy*0.5F));
	const auto cs = abs(v.y / sqrt(v.x*v.x+v.y*v.y));
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
	f2dvector_t v(c->x + 0.5F - (object->cell()->x + object->m_active_state->m_size.dx*0.5F), c->y - 0.5F - (object->cell()->y - object->m_active_state->m_size.dy*0.5F));
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

MapCell* Game_algorithm::step_in_direction(GameObject* object, object_direction_e d)
{
	auto result = object->cell();
	switch (d)
	{
	case object_direction_e::down:
	{
		return &result->m_map->get(0,result->y - 1,result->x);
	}
	case object_direction_e::downleft:
	{
		return &result->m_map->get(0,result->y - 1,result->x-1);
	}
	case object_direction_e::downright:
	{
		return &result->m_map->get(0,result->y - 1,result->x + 1);
	}
	case object_direction_e::left:
	{
		return &result->m_map->get(0,result->y,result->x - 1);
	}
	case object_direction_e::right:
	{
		return &result->m_map->get(0,result->y,result->x + 1);
	}
	case object_direction_e::top:
	{
		return &result->m_map->get(0,result->y + 1,result->x);
	}
	case object_direction_e::topleft:
	{
		return &result->m_map->get(0,result->y + 1,result->x - 1);
	}
	case object_direction_e::topright:
	{
		return &result->m_map->get(0,result->y + 1,result->x + 1);
	}
	}
}

int Game_algorithm::get_angle(object_direction_e direction, int angle)
{
	uint result;
	switch (direction)
	{
	case object_direction_e::down:
	{
		result = angle;
		break;
	}
	case object_direction_e::downleft:
	{
		result = angle+45;
		break;
	}
	case object_direction_e::downright:
	{
		result = angle + 315;
		break;
	}
	case object_direction_e::left:
	{
		result = angle + 90;
		break;
	}
	case object_direction_e::right:
	{
		result = angle + 270;
		break;
	}
	case object_direction_e::top:
	{
		result = angle + 180;
		break;
	}
	case object_direction_e::topleft:
	{
		result = angle + 135;
		break;
	}
	case object_direction_e::topright:
	{
		result = angle + 225;
		break;
	}
	}
	if (result > 360)
	{
		result -= 360;
	}
	return result;
}

object_direction_e Game_algorithm::get_direction(object_direction_e direction, int camera_turn)
{
	auto result = static_cast<int>(direction) + camera_turn*2;
	if(result>8)
	{
		result -= 8;
	}
	return static_cast<object_direction_e>(result);
}

rotate_direction_e Game_algorithm::get_rotation(object_direction_e start, object_direction_e end)
{
	const auto a = static_cast<int>(start);
	const auto b = static_cast<int>(end);
	const auto ab = b - a;
	const auto ba = 8 - b + a;
	if (abs(ab) < abs(ba))
	{
		if (a < b) { return rotate_direction_e::clockwise; }
		else { return rotate_direction_e::counterclockwise; }
	}
	else
	{
		if (a > b) { return rotate_direction_e::clockwise; }
		else { return rotate_direction_e::counterclockwise; }
	}
}
