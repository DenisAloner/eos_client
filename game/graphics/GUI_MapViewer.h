#ifndef GUI_MAPVIEWER_H
#define	GUI_MAPVIEWER_H

#include "GUI_Layer.h"
#include "GUI_Object.h"
#include "GameMap.h"
#include "Application.h"
#include "Action.h"
#include "Parameter.h"


class Application;
class Action;
class GUI_Object;
class GUI_Layer;
class Parameter;
class MapCell;
class GameMap;
class GameObject;

class gui_mapviewer_hint;

class GUI_MapViewer : public GUI_Object
{ 
public:

	Event<MouseEventArgs> start_moving;
	Event<MouseEventArgs> move;
	Event<MouseEventArgs> end_moving;

	GUI_Object* m_focus;
	MapCell* m_cursored;

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

	GameObject* m_player;
	GameMap* m_map;
	GUI_Layer* m_GUI;

	position_t m_center;
	position_t m_initial_position;

	std::list<gui_mapviewer_hint*> m_hints;

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

class gui_mapviewer_hint
{
public:

	GUI_MapViewer* m_owner;

	gui_mapviewer_hint(GUI_MapViewer* owner);
	virtual void render() = 0;
};

class mapviewer_hint_preselect_area : public gui_mapviewer_hint
{
public:
	MapCell* m_cell;
	mapviewer_hint_preselect_area(GUI_MapViewer* owner, MapCell* cell);
	virtual void render();
};

#endif //GUI_MAPVIEWER_H

