#include "FOV.h"

//static int multipliers1[4][8] = {
//	{ 1, 0, 0, -1, -1, 0, 0, 1 },
//	{ 0, 1, -1, 0, 0, -1, 1, 0 },
//	{ 0, 1, 1, 0, 0, -1, -1, 0 },
//	{ 1, 0, 0, 1, -1, 0, 0, -1 }
//};

//static int multipliers1[4][8] = {
//		{ 0, 0, 0, -1, -1, 0, 0, 1 },
//		{ -1, 1, -1, 0, 0, -1, 1, 0 },
//		{ 1, 1, 1, 0, 0, -1, -1, 0 },
//		{ 0, 0, 0, 1, -1, 0, 0, -1 }
//};

static int multipliers1[4][8] = {
	{ 1, 0,  0,  1, -1,  0,  0, -1 },
	{ 0, 1,  1,  0,  0, -1, -1,  0 },
	{ 0, 1, -1,  0,  0, -1,  1,  0 },
	{ 1, 0,  0, -1, -1,  0,  0,  1 }
};


//float calculate_angle(float x, float y)
//{
//	float angle = std::atan2(y, x) * (180.0F / 3.141592653589793238462643383279502884F);
//	return angle;
//}

//values::values(){
//	for (int y = 0; y < middle + 1; y++)
//	{
//		for (int x = 0; x <middle + 1; x++)
//		{
//			item[y][x].angle = calculate_angle(x, y);
//			item[y][x].shift_x = calculate_angle(x + 0.5, y - 0.5);
//			item[y][x].shift_y = calculate_angle(x - 0.5, y + 0.5);
//			item[y][x].distance = x*x + y*y;
//		}
//	}
//}

//const values FOV::m_values = values();

FOV::FOV()
{
}

//void FOV::quarter(int start_x, int start_y, float start_angle, float end_angle)
//{
//	int z;
//	fov_cell* c;
//	for (int y = start_y; y < m_radius; y++)
//	{
//		for (int x = start_x; x <m_radius; x++)
//		{
//			c = &m_map[middle + y*multipliers[m_quarter][1]][middle + x*multipliers[m_quarter][0]];
//			if (m_values.item[y][x].angle < end_angle || m_values.item[y][x].distance > m_sqr_radius) break;
//			if (m_values.item[y][x].angle <= start_angle)
//			{
//				if (c->opaque)
//				{
//					c->visible = true;
//					quarter(start_x, y + 1, start_angle, m_values.item[y][x].shift_y);
//					for (z = x + 1; z < m_radius; z++)
//					{
//						if (m_values.item[y][z].distance <= m_sqr_radius &&m_values.item[y][z].angle >= end_angle){
//							c = &m_map[middle + y*multipliers[m_quarter][1]][middle + z*multipliers[m_quarter][0]];
//							if (!c->opaque)
//							{
//								x = z - 1;
//								break;
//							}
//							c->visible = true;
//						}
//					}
//					start_angle = m_values.item[y][x].shift_x;
//					start_x = x;
//				}
//				else {
//					c->visible = true;
//				}
//			} else start_x = x;
//		}
//	}
//}
//
//void FOV::cast_shadow(int start_x, int start_y, float start_angle, float end_angle)
//{
//	int z;
//	int xs, ys;
//	fov_cell* c;
//	for (int y = start_y; y < m_radius; y++)
//	{
//		for (int x = start_x; x <m_radius; x++)
//		{
//			c = &m_map[middle + y*multipliers[m_quarter][1]][middle + x*multipliers[m_quarter][0]];
//			if (m_values.item[y][x].angle < end_angle || m_values.item[y][x].distance > m_sqr_radius) break;
//			if (m_values.item[y][x].angle <= start_angle)
//			{
//				c->visible = false;
//			}
//			else start_x = x;
//		}
//	}
//}
//
//void FOV::light_quarter(int start_x, int start_y, float start_angle, float end_angle)
//{
//	int z;
//	int xs, ys;
//	fov_cell* c;
//	for (int y = start_y; y < m_radius; y++)
//	{
//		for (int x = start_x; x <m_radius; x++)
//		{
//			c = &m_map[middle + y*multipliers[m_quarter][1]][middle + x*multipliers[m_quarter][0]];
//			if (m_values.item[y][x].angle < end_angle || m_values.item[y][x].distance > m_sqr_radius) break;
//			if (m_values.item[y][x].angle <= start_angle)
//			{
//				if (c->opaque)
//				{
//					c->visible = true;
//					ys = y;
//					for (z = y + 1; z < m_radius; z++)
//					{
//						if (m_values.item[z][x].distance <= m_sqr_radius &&m_values.item[z][x].angle >= end_angle){
//							c = &m_map[middle + z*multipliers[m_quarter][1]][middle + y*multipliers[m_quarter][0]];
//							if (!c->opaque)
//							{
//								break;
//							}
//							ys = z;
//							c->visible = true;
//						}
//					}
//					xs = x;
//					for (z = x + 1; z < m_radius; z++)
//					{
//						if (m_values.item[y][z].distance <= m_sqr_radius &&m_values.item[y][z].angle >= end_angle){
//							c = &m_map[middle + y*multipliers[m_quarter][1]][middle + z*multipliers[m_quarter][0]];
//							if (!c->opaque)
//							{
//								break;
//							}
//							xs = z;
//							c->visible = true;
//						}
//					}
//					light_quarter(start_x, y + 1, start_angle, m_values.item[y][x].shift_y);
//					cast_shadow(x, y + 1, m_values.item[ys][x].shift_y, m_values.item[y][xs].shift_x);
//					start_angle = m_values.item[y][xs].shift_x;
//					start_x = x;
//				}
//				else {
//					c->visible = true;
//				}
//			}
//			else start_x = x;
//		}
//	}
//}


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
			float l_slope = (dx - 0.5) / (dy + 0.5);
			float r_slope = (dx + 0.5) / (dy - 0.5);
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

void FOV::calculate_FOV(GameObject* object, GameMap* map)
{
	for (int y = 0; y < m_max_size; y++)
	{
		for (int x = 0; x < m_max_size; x++)
		{
			m_map[y][x].opaque = false;
			m_map[y][x].visible = false;
		}
	}

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

	AI_FOV current;
	for (auto item = vl->m_effect.begin(); item != vl->m_effect.end(); item++)
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
			for (auto obj = map->m_items[y][x]->m_items.begin(); obj != map->m_items[y][x]->m_items.end(); obj++)
			{
				m_map[yf][xf].opaque = false;
				if ((*obj) != unit&&qualifier((*obj)))
				{
					m_map[yf][xf].opaque = true;
				}
			}
		}
	}

	do_fov(m_view.l, m_view.d, fov.radius, Game_algorithm::get_angle(unit, fov.start_angle), Game_algorithm::get_angle(unit, fov.end_angle));
}