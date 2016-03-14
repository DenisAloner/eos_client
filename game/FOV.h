#pragma once
#include <cmath>
#include "GameObject.h"
#include "GameMap.h"

typedef unsigned int uint;

class GameObject;
class GameMap;

//struct value{
//	float angle;
//	float shift_x;
//	float shift_y;
//	int distance;
//};

//const int multipliers[4][2] = { { 1, 1 }, { 1, -1 }, { -1, -1 }, { -1, 1 } };

//float calculate_angle(float x, float y);

//struct values{
//	value item[middle + 1][middle + 1];
//	values();
//};

class FOV
{
public:

	struct cell
	{
		bool opaque;
		bool visible;
	};

	static const int m_max_size = 129;
	static const int m_middle = (m_max_size - 1) >> 1;

	cell m_map[m_max_size][m_max_size];

	FOV();

	
	//static const values m_values;

	//int m_radius;
	//int m_sqr_radius;
	//int m_quarter;

	/*void quarter(int start_x, int start_y, float start_angle, float end_angle);
	void cast_shadow(int start_x, int start_y, float start_angle, float end_angle);
	void light_quarter(int start_x, int start_y, float start_angle, float end_angle);*/
	void calculate(GameObject* unit, GameMap* map, AI_FOV& fov);
	void cast_light(uint x, uint y, uint radius, uint row, float start_slope, float end_slope, uint xx, uint xy, uint yx, uint yy);
	/*void do_fov(uint x, uint y, uint radius);*/
	void do_fov(uint x, uint y, uint radius, int start_angle, int end_angle,bool fold);

};

