#include "FOV.h"

static int multipliers1[4][8] = {
	{ 1, 0,  0,  1, -1,  0,  0, -1 },
	{ 0, 1,  1,  0,  0, -1, -1,  0 },
	{ 0, 1, -1,  0,  0, -1,  1,  0 },
	{ 1, 0,  0, -1, -1,  0,  0,  1 }
};

FOV::FOV(){}

void FOV::cast_light(uint x, uint y, uint radius, uint row, float start_slope, float end_slope, uint xx, uint xy, uint yx, uint yy)
{
	//LOG(INFO) << "    Вызов функции";
	if (start_slope <= end_slope) {
		return;
	}
	float next_start_slope = start_slope;
	for (uint i = row; i <= radius; i++)
	{
		bool blocked = false;
		for (int dx = -i, dy = -i; dx <= 0; dx++) {
			float l_slope = (dx - 0.5F) / (dy + 0.5F);
			float r_slope = (dx + 0.5F) / (dy - 0.5F);
			if (start_slope < r_slope) {
				continue;
			}
			else if (end_slope > l_slope) {
				break;
			}
			int sax = dx * xx + dy * xy;
			int say = dx * yx + dy * yy;
			if ((sax < 0 && (uint)std::abs(sax) > x) ||
				(say < 0 && (uint)std::abs(say) > y)) {
				continue;
			}
			uint ax = x + sax;
			uint ay = y + say;
			if (ax >= m_max_size || ay >= m_max_size) {
				continue;
			}

			uint radius2 = radius * radius;
			if ((uint)(dx * dx + dy * dy) < radius2)
			{
				m_map[ay][ax].visible = true;
			}

			if (blocked) {
				if (m_map[ay][ax].opaque) {
					next_start_slope = r_slope;
					continue;
				}
				else {
					blocked = false;
					start_slope = next_start_slope;
				}
			}
			else if (m_map[ay][ax].opaque) {
				blocked = true;
				next_start_slope = r_slope;
				cast_light(x, y, radius, i + 1, start_slope, l_slope, xx, xy, yx, yy);
			}
		}
		if (blocked) {
			break;
		}
	}
}

void set_slopes(uint start_octant, uint end_octant, uint current_octant, int start_angle, int end_angle, float& start_slope, float& end_slope)
{
	start_slope = 1.0;
	end_slope = 0.0;
	if (current_octant == start_octant)
	{
		switch (current_octant)
		{
		case 0:
		{
			end_slope = std::tan(start_angle*Pi / 180.0F);
			break;
		}
		case 1:
		{
			start_slope = std::tan((90 - start_angle)*Pi / 180.0F);
			break;
		}
		case 2:
		{
			end_slope = std::tan((start_angle - 90)*Pi / 180.0F);
			break;
		}
		case 3:
		{
			start_slope = std::tan((180 - start_angle)*Pi / 180.0F);
			break;
		}
		case 4:
		{
			end_slope = std::tan((start_angle - 180)*Pi / 180.0F);
			break;
		}
		case 5:
		{
			start_slope = std::tan((270 - start_angle)*Pi / 180.0F);
			break;
		}
		case 6:
		{
			end_slope = std::tan((start_angle - 270)*Pi / 180.0F);
			break;
		}
		case 7:
		{
			start_slope = std::tan((360 - start_angle)*Pi / 180.0F);
			break;
		}
		}
	}
	if (current_octant == end_octant)
	{
		switch (current_octant)
		{
		case 0:
		{
			start_slope = std::tan(end_angle*Pi / 180.0F);
			break;
		}
		case 1:
		{
			end_slope = std::tan((90 - end_angle)*Pi / 180.0F);
			break;
		}
		case 2:
		{
			start_slope = std::tan((end_angle - 90)*Pi / 180.0F);
			break;
		}
		case 3:
		{
			end_slope = std::tan((180 - end_angle)*Pi / 180.0F);
			break;
		}
		case 4:
		{
			start_slope = std::tan((end_angle - 180)*Pi / 180.0F);
			break;
		}
		case 5:
		{
			end_slope = std::tan((270 - end_angle)*Pi / 180.0F);
			break;
		}
		case 6:
		{
			start_slope = std::tan((end_angle - 270)*Pi / 180.0F);
			break;
		}
		case 7:
		{
			end_slope = std::tan((360 - end_angle)*Pi / 180.0F);
			break;
		}
		}
	}
}

int shift_x(const int& x,const int& octant)
{
	return x + (octant >> 2);
}

int shift_y(const int& y, const int& octant)
{
	switch(octant >> 1)
	{
	case 0: return y-1;
	case 1: return y;
	case 2: return y;
	case 3: return y-1;
	}
}


void FOV::do_fov(uint x, uint y, uint radius,int start_angle,int end_angle)
{
	if (start_angle == end_angle)
	{
		if (m_fold)
		{
			for (uint i = 0; i <8; i++)
			{
				cast_light(shift_x(x, i), shift_y(y, i), radius, 1, 1.0, 0.0, multipliers1[0][i], multipliers1[1][i], multipliers1[2][i], multipliers1[3][i]);
			}
		}
		else
		{
			for (uint i = 0; i <8; i++)
			{
				cast_light(x, y, radius, 1, 1.0, 0.0, multipliers1[0][i], multipliers1[1][i], multipliers1[2][i], multipliers1[3][i]);
			}
		}
		return;
	}
	uint start_octant = start_angle / 45;
	uint end_octant = end_angle / 45;
	float start_slope;
	float end_slope;
	if (!m_fold)
	{
		if (start_octant <= end_octant)
		{
			for (uint i = start_octant; i <end_octant + 1; i++)
			{
				set_slopes(start_octant, end_octant, i, start_angle, end_angle, start_slope, end_slope);
				cast_light(x, y, radius, 1, start_slope, end_slope, multipliers1[0][i], multipliers1[1][i], multipliers1[2][i], multipliers1[3][i]);
			}
		}
		else
		{
			for (uint i = start_octant; i < 8; i++)
			{
				set_slopes(start_octant, 7, i, start_angle, 0, start_slope, end_slope);
				cast_light(x, y, radius, 1, start_slope, end_slope, multipliers1[0][i], multipliers1[1][i], multipliers1[2][i], multipliers1[3][i]);
			}
			for (uint i = 0; i < end_octant + 1; i++)
			{
				set_slopes(0, end_octant, i, 0, end_angle, start_slope, end_slope);
				cast_light(x, y, radius, 1, start_slope, end_slope, multipliers1[0][i], multipliers1[1][i], multipliers1[2][i], multipliers1[3][i]);
			}
		}
	}
	else
	{
		if (start_octant <= end_octant)
		{
			for (uint i = start_octant; i <end_octant + 1; i++)
			{
				set_slopes(start_octant, end_octant, i, start_angle, end_angle, start_slope, end_slope);
				cast_light(shift_x(x,i), shift_y(y, i), radius, 1, start_slope, end_slope, multipliers1[0][i], multipliers1[1][i], multipliers1[2][i], multipliers1[3][i]);
			}
		}
		else
		{
			for (uint i = start_octant; i < 8; i++)
			{
				set_slopes(start_octant, 7, i, start_angle, 0, start_slope, end_slope);
				cast_light(shift_x(x, i), shift_y(y, i), radius, 1, start_slope, end_slope, multipliers1[0][i], multipliers1[1][i], multipliers1[2][i], multipliers1[3][i]);
			}
			for (uint i = 0; i < end_octant + 1; i++)
			{
				set_slopes(0, end_octant, i, 0, end_angle, start_slope, end_slope);
				cast_light(shift_x(x, i), shift_y(y, i), radius, 1, start_slope, end_slope, multipliers1[0][i], multipliers1[1][i], multipliers1[2][i], multipliers1[3][i]);
			}
		}
	}
}

void FOV::calculate_FOV(GameObject* object, GameMap* map, object_direction_e direction)
{
	if (direction == object_direction_e::none) { m_direction = object->m_direction; } else { m_direction = direction; }
	Vision_list* vl = static_cast<Vision_list*>(object->m_active_state->get_list(interaction_e::vision));
	int radius = vl->m_max_radius;

	int dx = object->m_active_state->m_size.x;
	int dy = object->m_active_state->m_size.y;

	
	m_fold = (dx >> 1 == (dx + 1) >> 1);

	int xs = ((dx - 1) >> 1);
	int ys = ((dy - 1) >> 1);

	m_map_center = position_t(object->cell()->x + xs, object->cell()->y - ys);

	if (m_map_center.x - radius <0)
	{
		m_view.l = m_map_center.x;
	}
	else
	{
		m_view.l = radius;
	}

	if (m_map_center.x+radius > map->m_size.w - 1)
	{
		m_view.r = map->m_size.w - 1 - m_map_center.x;
	}
	else
	{
		m_view.r = radius;
	}

	if (m_map_center.y - radius < 0)
	{
		m_view.d = m_map_center.y;
	}
	else
	{
		m_view.d = radius;
	}

	if (m_map_center.y + radius > map->m_size.h - 1)
	{
		m_view.u = map->m_size.h - 1 - m_map_center.y;
	}
	else
	{
		m_view.u = radius;
	}

	m_view_center = position_t(m_view.l, m_view.d);

	for (int y = 0; y < m_view.d + m_view.u + 1; y++)
	{
		for (int x = 0; x < m_view.l + m_view.r + 1; x++)
		{
			m_map[y][x].visible = false;
		}
	}

	AI_FOV current;
	for (auto item = vl->m_items.begin(); item != vl->m_items.end(); ++item)
	{
		current = static_cast<Vision_component*>(*item)->m_value;
		calculate(object, map, current);
	}

	for (int y = 0; y < dy; y++)
	{
		for (int x = 0; x < dx; x++)
		{
			m_map[y + m_view.d-(dx>>1)][x - (xs - m_view.l)].visible = true;
		}
	}
}

void FOV::calculate(GameObject* unit, GameMap* map, AI_FOV& fov)
{
	std::function<bool(GameObject*)> qualifier = fov.qualifier->predicat;

	view_t view(min(m_view.l, fov.radius), min(m_view.r, fov.radius), min(m_view.u, fov.radius), min(m_view.d, fov.radius));

	for (int y = m_map_center.y - view.d, yf = m_view_center.y - view.d; y < m_map_center.y + view.u + 1; y++, yf++)
	{
		for (int x = m_map_center.x - view.l, xf = m_view_center.x - view.l; x < m_map_center.x + view.r + 1; x++, xf++)
		{
			for (auto obj = map->get(y,x).m_items.begin(); obj != map->get(y, x).m_items.end(); ++obj)
			{
				m_map[yf][xf].opaque = false;
				if ((*obj) != unit&&qualifier((*obj)))
				{
					m_map[yf][xf].opaque = true;
				}
			}
		}
	}

	do_fov(m_view.l, m_view.d, fov.radius, Game_algorithm::get_angle(m_direction, fov.start_angle), Game_algorithm::get_angle(m_direction, fov.end_angle));
}

FOV_help::FOV_help(){}

void FOV_help::calculate(GameObject* unit, GameMap* map, AI_FOV& fov)
{
	std::function<bool(GameObject*)> qualifier = fov.qualifier->predicat;

	view_t view(min(m_view.l, fov.radius), min(m_view.r, fov.radius), min(m_view.u, fov.radius), min(m_view.d, fov.radius));

	for (int y = m_map_center.y - view.d, yf = m_view_center.y - view.d; y < m_map_center.y + view.u + 1; y++, yf++)
	{
		for (int x = m_map_center.x - view.l, xf = m_view_center.x - view.l; x < m_map_center.x + view.r + 1; x++, xf++)
		{
			m_map[yf][xf].opaque = false;
			if (map->get(y, x).m_notable == true)
			{
				for (auto obj = map->get(y, x).m_items.begin(); obj != map->get(y, x).m_items.end(); ++obj)
				{
					if ((*obj) != unit&&qualifier((*obj)))
					{
						m_map[yf][xf].opaque = true;
					}
				}
			}
		}
	}

	do_fov(m_view.l, m_view.d, fov.radius, Game_algorithm::get_angle(m_direction, fov.start_angle), Game_algorithm::get_angle(m_direction, fov.end_angle));
}