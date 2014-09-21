#include "FOV.h"

float calculate_angle(float x, float y)
{
	float angle = std::atan2(y, x) * (180.0F / 3.141592653589793238462643383279502884F);
	return angle;
}

values::values(){
	for (int y = 0; y < middle + 1; y++)
	{
		for (int x = 0; x <middle + 1; x++)
		{
			item[y][x].angle = calculate_angle(x, y);
			item[y][x].shift_x = calculate_angle(x + 0.5, y - 0.5);
			item[y][x].shift_y = calculate_angle(x - 0.5, y + 0.5);
			item[y][x].distance = x*x + y*y;
		}
	}
}

const values FOV::m_values = values();

FOV::FOV()
{
}

void FOV::quarter(int start_x, int start_y, float start_angle, float end_angle)
{
	int z;
	fov_cell* c;
	for (int y = start_y; y < m_radius; y++)
	{
		for (int x = start_x; x <m_radius; x++)
		{
			c = &m_map[middle + y*multipliers[m_quarter][1]][middle + x*multipliers[m_quarter][0]];
			if (m_values.item[y][x].distance <= m_sqr_radius && (m_values.item[y][x].angle <= start_angle) && (m_values.item[y][x].angle >= end_angle))
			{
				if (c->opaque)
				{
					c->visible = true;
					quarter(start_x, y + 1, start_angle, m_values.item[y][x].shift_y);
					for (z = x + 1; z < m_radius; z++)
					{
						c = &m_map[middle + y*multipliers[m_quarter][1]][middle + z*multipliers[m_quarter][0]];
						if (!c->opaque)
						{
							x = z - 1;
							break;
						}
						c->visible = true;
					}
					start_angle = m_values.item[y][x].shift_x;
					start_x = x;
				}
				else {
					c->visible = true;
				}
			}
		}
	}
}

void FOV::calculate(int radius,GameObject* unit, GameMap* map)
{
	m_radius = radius;
	m_sqr_radius = radius*radius;
	for (int y = 0; y <max_size; y++)
	{
		for (int x = 0; x <max_size; x++)
		{
			m_map[y][x].opaque = false;
			m_map[y][x].visible = false;
		}
	}
	for (int y = unit->cell()->y - radius; y < unit->cell()->y + radius+1; y++)
	{
		if (!((y<0) || (y>map->m_size.h - 1)))
		{
			for (int x = unit->cell()->x - radius; x < unit->cell()->x + radius + 1; x++)
			{
				if (!((x<0) || (x>map->m_size.w - 1)))
				{
					for (std::list<GameObject*>::iterator obj = map->m_items[y][x]->m_items.begin(); obj != map->m_items[y][x]->m_items.end(); obj++)
					{
						//if ((!(*obj)->m_active_state->find_property(property_e::permit_move))&&(*obj)!=unit)
						if (((*obj)->m_name=="wall")&&(*obj)!=unit)
						{
							m_map[middle + (y - unit->cell()->y)][middle + (x - unit->cell()->x)].opaque = true;
						}
					}
				}
			}
		}
	}
	m_quarter = 0;
	quarter(0, 0, 180, -180);
	m_quarter = 1;
	quarter(0, 0, 180, -180);
	m_quarter = 2;
	quarter(0, 0, 180, -180);
	m_quarter = 3;
	quarter(0, 0, 180, -180);
}