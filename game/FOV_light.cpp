#include "FOV_light.h"

static int multipliers1[4][8] = {
		{ 1, 0, 0, -1, -1, 0, 0, 1 },
		{ 0, 1, -1, 0, 0, -1, 1, 0 },
		{ 0, 1, 1, 0, 0, -1, -1, 0 },
		{ 1, 0, 0, 1, -1, 0, 0, -1 }
};


FOV_light::FOV_light()
{
}

void FOV_light::calculate(int radius, GameObject* unit, GameMap* map)
{
	m_radius = radius;
	optical_properties_t* op;
	for (int y = 0; y <m_max_size; y++)
	{
		for (int x = 0; x <m_max_size; x++)
		{
			m_map[y][x].opaque = false;
			m_map[y][x].damping = RGB_t(0.0, 0.0, 0.0);
			m_map[y][x].attenuation_constant = RGB_t(0.0, 0.0, 0.0);
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
						op = (*obj)->m_active_state->m_optical;
						if (op)
						{
							m_map[m_middle + (y - unit->cell()->y)][m_middle + (x - unit->cell()->x)].opaque = true;
							m_map[m_middle + (y - unit->cell()->y)][m_middle + (x - unit->cell()->x)].attenuation_constant = op->attenuation;
						}
					}
				}
			}
		}
	}
	do_fov(m_middle, m_middle, m_radius);
}

void FOV_light::cast_light(uint x, uint y, uint radius, uint row, float start_slope, float end_slope, uint xx, uint xy, uint yx, uint yy, RGB_t damp)
{
	if (start_slope < end_slope) {
		return;
	}
	float next_start_slope = start_slope;
	for (uint i = row; i <= radius; i++) {
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
			if ((uint)(dx * dx + dy * dy) < radius2) {
				m_map[ay][ax].damping = damp;
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
				if (damp.R != 0 && damp.G != 0 && damp.B != 0)
				{
					cast_light(x, y, radius, i + 1, l_slope, r_slope, xx, xy, yx, yy, RGB_t(damp.R*m_map[ay][ax].attenuation_constant.R, damp.G*m_map[ay][ax].attenuation_constant.G, damp.B*m_map[ay][ax].attenuation_constant.B));
				}
				
				cast_light(x, y, radius, i + 1, start_slope, l_slope, xx, xy, yx, yy, damp);
			}
		}
		if (blocked) {
			break;
		}
	}
}

void FOV_light::do_fov(uint x, uint y, uint radius)
{
	for (uint i = 0; i < 8; i++) {
		cast_light(x, y, radius, 1, 1.0, 0.0, multipliers1[0][i],multipliers1[1][i], multipliers1[2][i], multipliers1[3][i],RGB_t(1.0,1.0,1.0));
	}
	m_map[m_middle][m_middle].damping = RGB_t(1.0, 1.0, 1.0);
}