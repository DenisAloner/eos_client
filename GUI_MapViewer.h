#pragma once
#include "GUI_Layer.h"
#include "GUI_Object.h"
#include "GameMap.h"
#include "Application.h"
#include "ActionsDescription.h"
#include <thread>
#include "TParameter.h"

class Application;
class TAction;
class TPlayer;
class GUI_Object;
class GUI_Layer;
class TParameter;
class MapCell;
class GameMap;

class GCMapElement:
	public GUI_Object
{
public:

	MapCell* MapElement;
	int x;
	int y;

	GCMapElement();
	~GCMapElement();

};

class GUI_MapViewer :
	public GUI_Object
{ 
public:

	Event<MouseEventArgs> StartMoving;
	Event<MouseEventArgs> Move;
	Event<MouseEventArgs> EndMoving;

	GUI_Object* focus;
	GCMapElement* cursored;
	Application* app;

	bool JustFocused;
	bool IsMoving;

	GUI_MapViewer(Application* app);
	~GUI_MapViewer(void);

	int TileCountX =32;
	int TileCountY =64;

	double TileSizeX;
	double TileSizeY;

	int CursorX;
	int CursorY;

	TPlayer* Player;
	GameMap* Map;
	GCMapElement* Items[512][512];
	GUI_Layer* GUI;
	TAction* AppliedAction;

	Point center=Point(0,0);
	Point InitialPosition = Point(0, 0);

	void Update();
	virtual bool Select(int x,int y);
	virtual void RenderAt(GraphicalController* Graph, int px, int py);
	
	void Ñalculate();
	virtual KeyboardEventCallback GetKeyboardEventCallback();
	virtual void OnKeyPress(WPARAM w);
	virtual MouseEventCallback GetMouseEventCallback();
	virtual void OnMouseClick(MouseEventArgs const& e);
	virtual void OnMouseDown(MouseEventArgs const& e);
	virtual void OnMouseWheel(MouseEventArgs const& e);
	virtual void OnMouseScroll(MouseEventArgs const& e);
	virtual void SetFocus(bool state);
	virtual void OnItemGetFocus(GUI_Object* sender);
	virtual void OnUnderCursor(MouseEventArgs const& e);
	virtual void OnLoseFocus(GUI_Object* sender);
	Point LocalXY(Point p);

	virtual void OnMouseMove(MouseEventArgs const& e);
	virtual void OnStartMoving(MouseEventArgs const& e);
	virtual void OnMove(MouseEventArgs const& e);
	virtual void OnEndMoving(MouseEventArgs const& e);
	virtual MouseEventArgs SetLocalMousePosition(MouseEventArgs const& source);
};