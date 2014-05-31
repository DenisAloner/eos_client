#pragma once
#include <Windows.h>
#include <functional>

class GUI_Object;

struct GPosition
{
	int x;
	int y;

	GPosition(int _x, int _y) : x(_x), y(_y){}
	GPosition() : x(0), y(0){}
};

struct GSize
{
	int x;
	int y;

	GSize(int _x, int _y) : x(_x), y(_y){}
	GSize() : x(0), y(0){}
};

struct MouseEventArgs
{
	GPosition position;
	WPARAM flags;

	MouseEventArgs(GPosition position, WPARAM flags) : position(position), flags(flags) {}
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