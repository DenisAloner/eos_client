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

	MapCell* m_map_element;
	int m_x;
	int m_y;

	GCMapElement();
	~GCMapElement();

};

class GUI_MapViewer :
	public GUI_Object
{ 
public:

	Event<MouseEventArgs> start_moving;
	Event<MouseEventArgs> move;
	Event<MouseEventArgs> end_moving;

	GUI_Object* m_focus;
	GCMapElement* m_cursored;

	bool m_just_focused;
	bool m_is_moving;

	GUI_MapViewer(Application* app);
	~GUI_MapViewer(void);

	int m_tile_count_x;
	int m_tile_count_y;

	double m_tile_size_x;
	double m_tile_size_y;

	int m_cursor_x;
	int m_cursor_y;

	TPlayer* m_player;
	GameMap* m_map;
	GCMapElement* m_items[512][512];
	GUI_Layer* m_GUI;

	position_t m_center;
	position_t m_initial_position;

	
	virtual void on_key_press(WPARAM w);
	virtual void on_mouse_click(MouseEventArgs const& e);
	virtual void on_mouse_down(MouseEventArgs const& e);
	virtual void on_mouse_wheel(MouseEventArgs const& e);
	virtual void on_mouse_move(MouseEventArgs const& e);
	virtual void on_start_moving(MouseEventArgs const& e);
	virtual void on_move(MouseEventArgs const& e);
	virtual void on_end_moving(MouseEventArgs const& e);
	virtual void on_item_get_focus(GUI_Object* sender);
	virtual void on_lose_focus(GUI_Object* sender);
	position_t local_xy(position_t p);
	virtual void set_focus(bool state);
	void update();
	virtual bool select(int x, int y);
	virtual void render(GraphicalController* Graph, int px, int py);
	void calculate();
	virtual MouseEventArgs set_local_mouse_control(MouseEventArgs const& source);
};