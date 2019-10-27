#ifndef FOV_LIGHT_H
#define	FOV_LIGHT_H
#include "Definiton.h"

typedef unsigned int uint;

class GameObject;
class GameMap;

class FOV_light
{
public:

	struct cell
	{
		bool opaque;
		bool visible;
		rgb_t<int> light;
	};

	static const int m_max_size = 41;
	static const int m_middle = (m_max_size - 1) >> 1;

	std::vector<cell> m_map;
	cell m_xy[m_max_size][m_max_size];
	cell m_yz[m_max_size][m_max_size];
	cell m_xz[m_max_size][m_max_size];
	int m_radius;

	cell& get(int z,int y,int x);

	FOV_light();
	void calculate(int radius, GameObject* unit, GameMap* map);
	void cast_light_xy(uint x, uint y, uint radius, uint row, float start_slope, float end_slope, uint xx, uint xy, uint yx, uint yy);
	void cast_light_yz(uint x, uint y, uint radius, uint row, float start_slope, float end_slope, uint xx, uint xy, uint yx, uint yy);
	void cast_light_xz(uint x, uint y, uint radius, uint row, float start_slope, float end_slope, uint xx, uint xy, uint yx, uint yy);
	void do_fov(uint x, uint y, uint radius);
};

#endif //FOV_LIGHT_H
