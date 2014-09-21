#pragma once
#include <cmath>
#include "GameObject.h"
#include "GameMap.h"

class GameObject;
class GameMap;

struct fov_cell{
	bool visible;
	bool opaque;
};

struct value{
	float angle;
	float shift_x;
	float shift_y;
	int distance;
};

const int max_size = 71;
const int middle = (max_size - 1) >> 1;
const int multipliers[4][2] = { { 1, 1 }, { 1, -1 }, { -1, -1 }, { -1, 1 } };

float calculate_angle(float x, float y);

struct values{
	value item[middle + 1][middle + 1];
	values();
};

class FOV
{
public:

	static const values m_values;

	fov_cell m_map[max_size][max_size];

	int m_radius;
	int m_sqr_radius;
	int m_quarter;

	FOV();

	void quarter(int start_x, int start_y, float start_angle, float end_angle);
	void calculate(int radius, GameObject* unit, GameMap* map);
};

