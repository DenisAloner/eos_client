#pragma once
#include <Windows.h>
#include <functional>

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
	rectangle_t(int x, int y, const dimension_t& dimension) : position_t(x, y), dimension_t(dimension) {}
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
	frectangle_t(float x, float y, const fdimension_t& dimension) : fposition_t(x, y), fdimension_t(dimension) {}
	frectangle_t() : fposition_t(), fdimension_t() {}
};

struct MouseEventArgs
{
	position_t position;
	WPARAM flags;

	MouseEventArgs(position_t position, WPARAM flags) : position(position), flags(flags) {}
};

enum ActionKind
{
	ActionKind_Move,
	ActionKind_Push,
	ActionKind_Turn,
	ActionKind_OpenInventory
};

enum PropertyKind
{
	PropertyKind_None,
	PropertyKind_Impassable,
	PropertyKind_Container
};

enum ParameterKind
{
	ParameterKind_Simple,
	ParameterKind_Position,
	ParameterKind_MapCell,
	ParameterKind_MoveObjectByUnit,
	ParameterKind_GameObject,
	ParameterKind_Cancel
};

enum ObjectDirection
{
	ObjectDirection_Up,
	ObjectDirection_Left,
	ObjectDirection_Down,
	ObjectDirection_Right
};