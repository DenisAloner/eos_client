#include "FOV_light.h"

static int multipliers1[4][8] = {
		{ 1, 0, 0, -1, -1, 0, 0, 1 },
		{ 0, 1, -1, 0, 0, -1, 1, 0 },
		{ 0, 1, 1, 0, 0, -1, -1, 0 },
		{ 1, 0, 0, 1, -1, 0, 0, -1 }
};


FOV_light::cell& FOV_light::get(int z, int y, int x)
{
	return m_map[z*(m_max_size*m_max_size) + y * m_max_size + x];
}

FOV_light::FOV_light()
{
	m_map.resize(m_max_size*m_max_size*m_max_size);
}

void FOV_light::calculate(int radius, GameObject* unit, GameMap* map)
{
	m_radius = radius;
	optical_properties_t* op;

	for (int z = 0; z < m_max_size; ++z)
	{
		for (int y = 0; y < m_max_size; ++y)
		{
			for (int x = 0; x < m_max_size; ++x)
			{
				get(z, y, x).opaque = false;
				get(z, y, x).visible = false;
			}
		}
	}
	
	for (int z = unit->cell()->z - radius; z < unit->cell()->z + radius + 1; ++z)
	{
		if (!((z < 0) || (z > map->m_size.dz - 1)))
		{
			//LOG(INFO) <<"Выполнено для уровня: " <<std::to_string(z);
			for (int y = unit->cell()->y - radius; y < unit->cell()->y + radius + 1; ++y)
			{
				if (!((y < 0) || (y > map->m_size.dy - 1)))
				{
					for (int x = unit->cell()->x - radius; x < unit->cell()->x + radius + 1; ++x)
					{
						if (!((x < 0) || (x > map->m_size.dx - 1)))
						{
							for (auto obj = map->get(z,y, x).m_items.begin(); obj != map->get(z,y, x).m_items.end(); ++obj)
							{
								op = (*obj)->m_active_state->m_optical;
								if (op)
								{
									get(m_middle + (z - unit->cell()->z),m_middle + (y - unit->cell()->y),m_middle + (x - unit->cell()->x)).opaque = true;
								}
							}
						}
					}
				}
			}
		}
	}

	/*for (int x = 0; x < m_max_size; x++)
	{
		LOG(INFO) << std::to_string(x);
		for (int z = m_max_size-1; z>-1 ; --z)
		{
			std::string v = "";
			for (int y = m_max_size-1; y >-1 ; --y)
			{
				v += std::to_string(get(z, y, x).opaque);
			}
			LOG(INFO) << v;
		}
	}*/

	
	for (int z = 0; z < m_max_size; z++)
	{
		for (int y = 0; y < m_max_size; y++)
		{
			for (int x = 0; x < m_max_size; x++)
			{
				m_xy[y][x].opaque = m_xy[y][x].opaque||get(z,y,x).opaque;
				m_xy[y][x].visible = false;
			}
		}
	}

	for (int x = 0; x < m_max_size; x++)
	{
		for (int z = 0; z < m_max_size; z++)
		{
			for (int y = 0; y < m_max_size; y++)
			{
				m_yz[z][y].opaque = false;
				m_yz[z][y].visible = false;
			}
		}
	}

	for (int x = 0; x < m_max_size; x++)
	{
		for (int z = 0; z < m_max_size; z++)
		{
			for (int y = 0; y < m_max_size; y++)
			{
				m_yz[z][y].opaque = m_yz[z][y].opaque || get(z, y, x).opaque;
				m_yz[z][y].visible = false;
			}
		}
	}

	for (int z = 0; z < m_max_size; z++)
	{
		std::string v="";
		for (int y = 0; y < m_max_size; y++)
		{
			v += std::to_string(m_yz[z][y].opaque);
		}
		//LOG(INFO) << v;
	}

	for (int y = 0; y < m_max_size; y++)
	{
		for (int z = 0; z < m_max_size; z++)
		{
			for (int x = 0; x < m_max_size; x++)
			{
				m_xz[z][x].opaque = m_xz[z][x].opaque || get(z, y, x).opaque;
				m_xz[z][x].visible = false;
			}
		}
	}

	do_fov(m_middle, m_middle, m_radius);
}

void FOV_light::cast_light_xy(uint x, uint y, uint radius, uint row, float start_slope, float end_slope, uint xx, uint xy, uint yx, uint yy)
{
	if (start_slope < end_slope) {
		return;
	}
	float next_start_slope = start_slope;
	for (uint i = row; i <= radius; i++) {
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
			if ((uint)(dx * dx + dy * dy) < radius2) {
				m_xy[ay][ax].visible = true;
			}

			if (blocked) {
				if (m_xy[ay][ax].opaque) {
					next_start_slope = r_slope;
					continue;
				}
				else {
					blocked = false;
					start_slope = next_start_slope;
				}
			}
			else if (m_xy[ay][ax].opaque) {
				blocked = true;
				next_start_slope = r_slope;
				cast_light_xy(x, y, radius, i + 1, start_slope, l_slope, xx, xy, yx, yy);
			}
		}
		if (blocked) {
			break;
		}
	}
}

void FOV_light::cast_light_yz(uint x, uint y, uint radius, uint row, float start_slope, float end_slope, uint xx, uint xy, uint yx, uint yy)
{
	if (start_slope < end_slope) {
		return;
	}
	float next_start_slope = start_slope;
	for (uint i = row; i <= radius; i++) {
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
			if ((uint)(dx * dx + dy * dy) < radius2) {
				m_yz[ay][ax].visible = true;
			}

			if (blocked) {
				if (m_yz[ay][ax].opaque) {
					next_start_slope = r_slope;
					continue;
				}
				else {
					blocked = false;
					start_slope = next_start_slope;
				}
			}
			else if (m_yz[ay][ax].opaque) {
				blocked = true;
				next_start_slope = r_slope;
				cast_light_yz(x, y, radius, i + 1, start_slope, l_slope, xx, xy, yx, yy);
			}
		}
		if (blocked) {
			break;
		}
	}
}

void FOV_light::cast_light_xz(uint x, uint y, uint radius, uint row, float start_slope, float end_slope, uint xx, uint xy, uint yx, uint yy)
{
	if (start_slope < end_slope) {
		return;
	}
	float next_start_slope = start_slope;
	for (uint i = row; i <= radius; i++) {
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
			if ((uint)(dx * dx + dy * dy) < radius2) {
				m_xz[ay][ax].visible = true;
			}

			if (blocked) {
				if (m_xz[ay][ax].opaque) {
					next_start_slope = r_slope;
					continue;
				}
				else {
					blocked = false;
					start_slope = next_start_slope;
				}
			}
			else if (m_xz[ay][ax].opaque) {
				blocked = true;
				next_start_slope = r_slope;
				cast_light_xz(x, y, radius, i + 1, start_slope, l_slope, xx, xy, yx, yy);
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
		cast_light_xy(x, y, radius, 1, 1.0, 0.0, multipliers1[0][i],multipliers1[1][i], multipliers1[2][i], multipliers1[3][i]);
	}
	for (uint i = 0; i < 8; i++) {
		cast_light_yz(x, y, radius, 1, 1.0, 0.0, multipliers1[0][i], multipliers1[1][i], multipliers1[2][i], multipliers1[3][i]);
	}
	for (uint i = 0; i < 8; i++) {
		cast_light_xz(x, y, radius, 1, 1.0, 0.0, multipliers1[0][i], multipliers1[1][i], multipliers1[2][i], multipliers1[3][i]);
	}

	//LOG(INFO) << "After Shadowcasting";
	for (int z = 0; z < m_max_size; z++)
	{
		std::string v = "";
		for (int y = 0; y < m_max_size; y++)
		{
			if(z==20&&y==20)
			{
				v += "x";
			}
			else
			{
				v += std::to_string(m_yz[z][y].visible);
			}
			
		}
		//LOG(INFO) << v;
	}

	for (int z = 0; z < m_max_size; z++)
	{
		for (int y = 0; y < m_max_size; y++)
		{
			for (int x = 0; x < m_max_size; x++)
			{
				get(z, y, x).visible = m_xy[y][x].visible;
			}
		}
	}

	for (int x = 0; x < m_max_size; x++)
	{
		for (int z = 0; z < m_max_size; z++)
		{
			for (int y = 0; y < m_max_size; y++)
			{
				get(z, y, x).visible = m_yz[z][y].visible||get(z, y, x).visible;
			}
		}
	}

	for (int y = 0; y < m_max_size; y++)
	{
		for (int z = 0; z < m_max_size; z++)
		{
			for (int x = 0; x < m_max_size; x++)
			{
				get(z, y, x).visible = m_xz[z][x].visible||get(z, y, x).visible;
			}
		}
	}

	//get(m_middle, m_middle, m_middle).damping = RGB_t(1.0, 1.0, 1.0);
}