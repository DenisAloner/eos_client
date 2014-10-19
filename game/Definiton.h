#ifndef DEFINITION_H
#define	DEFINITION_H

#include <functional>
#include "graphics/gl.hpp"


class GUI_Object;

struct position_t
{
	int x;
	int y;

	position_t(int x, int y) : x(x), y(y) {}
	position_t() : x(0), y(0) {}
};

struct dimension_t
{
	int w;
	int h;

	dimension_t(int w, int h) : w(w), h(h) {}
	dimension_t() : w(0), h(0) {}
};

struct rectangle_t : public position_t, public dimension_t
{
	rectangle_t(int x, int y, int w, int h) : position_t(x, y), dimension_t(w, h) {}
	rectangle_t(int x, int y, const dimension_t &dimension) : position_t(x, y), dimension_t(dimension) {}
	rectangle_t() : position_t(), dimension_t() {}
};

struct fposition_t
{
	float x;
	float y;

	fposition_t(float x, float y) : x(x), y(y) {}
	fposition_t() : x(0.0f), y(0.0f) {}
};

struct fdimension_t
{
	float w;
	float h;

	fdimension_t(float w, float h) : w(w), h(h) {}
	fdimension_t() : w(0.0f), h(0.0f) {}
};

struct frectangle_t : public fposition_t, public fdimension_t
{
	frectangle_t(float x, float y, float w, float h) : fposition_t(x, y), fdimension_t(w, h) {}
	frectangle_t(float x, float y, const fdimension_t &dimension) : fposition_t(x, y), fdimension_t(dimension) {}
	frectangle_t() : fposition_t(), fdimension_t() {}
};

struct MouseEventArgs
{
	position_t position;
	int key;
	int value;

	MouseEventArgs(const position_t &position, int key) : position(position), key(key), value(0) {}
	MouseEventArgs(const position_t &position, int key, int value) : position(position), key(key), value(value) {}
};

enum mouse_key_e
{
	mk_none = 0,
	mk_left = 1,
	mk_right = 2,
	mk_middle = 3
};

struct VoidEventArgs
{
};

enum entity_e
{
	cell,
	inventory_cell,
	body_part
};

enum action_e
{
	move,
	push,
	turn,
	open_inventory,
	cell_info,
	set_motion_path,
	pick,
	open,
	hit,
	equip
};

enum property_e
{
	none,
	permit_move,
	permit_pick,
	permit_equip_hand,
	container,
	strength,
	health,
	body,
	wield
};

enum state_e
{ 
	alive,
	dead,
	on,
	off
};

enum ParameterKind
{
	ParameterKind_Simple,
	ParameterKind_Position,
	ParameterKind_MapCell,
	ParameterKind_MoveObjectByUnit,
	ParameterKind_GameObject,
	ParameterKind_Cancel,
	ParameterKind_Destination,
	parameter_kind_cell,
	parameter_kind_object,
	parameter_kind_inventory_cell,
	parameter_kind_body_part,
	parameter_kind_unit_interaction
};

enum ObjectDirection
{
	ObjectDirection_Down,
	ObjectDirection_Left,
	ObjectDirection_Up,
	ObjectDirection_Right
};

struct tile_t
{
	GLuint unit;
	GLdouble coordinat[4];
};

struct game_object_size_t
{
	int x;
	int y;
	int z;

	game_object_size_t(int x, int y,int z) : x(x), y(y),z(z) {}
	game_object_size_t() : x(0), y(0),z(0) {}
};

struct RGBA_t
{
	float R, G, B, A;

	RGBA_t(float R, float G, float B, float A) : R(R), G(G), B(B), A(A){};

	float RGBA_t::operator[](const std::size_t& index)
	{
		switch (index)
		{
		case 0: return R;
		case 1: return G;
		case 2: return B;
		case 3: return A;
		}
	}
};

struct RGB_t
{
	float R, G, B;
	RGB_t(float R, float G, float B) : R(R), G(G), B(B){};
};

struct light_t
{
	int R, G, B;

	light_t(int R, int G, int B) : R(R), G(G), B(B){};
	light_t() : R(0), G(0), B(0){};
};

enum class reaction_e
{
	get_damage,
	get_buff,
	change_health
};

enum class reaction_applicator_e
{
	get_damage_basic,
	change_health_basic,
	get_buff_basic
};

enum class effect_e
{
	damage,
	buff
};

enum class effect_subtype_e
{
	physical_damage,
	poison_damage
};

enum class body_part_e
{
	head,
	hand,
	foot
};

#endif //DEFINITION_H
