#pragma once
#include <cmath>
#include "GameObject.h"
#include "GameMap.h"
typedef unsigned int uint;

class GameObject;
class GameMap;

class FOV_light
{
public:

	struct cell
	{
		bool opaque;
		light_t light;
		RGB_t damping;
		RGB_t attenuation_constant;
	};

	static const int m_max_size = 41;
	static const int m_middle = (m_max_size - 1) >> 1;

	cell m_map[m_max_size][m_max_size];
	int m_radius;

	FOV_light();
	void calculate(int radius, GameObject* unit, GameMap* map);
	void cast_light(uint x, uint y, uint radius, uint row, float start_slope, float end_slope, uint xx, uint xy, uint yx, uint yy,RGB_t damp);
	void do_fov(uint x, uint y, uint radius);

};

