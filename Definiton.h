#pragma once
#include <Windows.h>
#include <functional>

class GUI_Object;

struct Point
{
	int x;
	int y;

	Point(int _x, int _y) : x(_x), y(_y){}
};

struct MouseEventArgs
{
	Point Position;
	WPARAM Flags;

	MouseEventArgs(Point _Position, WPARAM _Flags) : Position(_Position), Flags(_Flags) {}
};

typedef std::function<void(WPARAM)> KeyboardEventCallback;
typedef std::function<void(MouseEventArgs)> MouseEventCallback;

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