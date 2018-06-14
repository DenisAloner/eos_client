#ifndef GUI_MAPVIEWER_H
#define	GUI_MAPVIEWER_H

#include "GUI_Layer.h"
#include "GUI_Object.h"
#include "GameMap.h"
#include "Application.h"
#include "Action.h"
#include "Parameter.h"
#include "FOV.h"
#include "graphics\GUI_PopMenu.h"

class Application;
class Action;
class GUI_Object;
class GUI_Layer;
class Parameter;
class P_cell;
class GameMap;
class GameObject;
class gui_mapviewer_hint;
class FOV;
class FOV_help;
class Player;

class GUI_MapViewer : public GUI_Object
{ 
public:

	GLuint m_vertex_buffer;
	GLuint m_textcoor_buffer;
	GLuint m_light_buffer;
	GLuint m_vao;

	GUI_Object* m_focus;
	MapCell* m_cursored;

	bool m_just_focused;

	GUI_MapViewer(Application* app);
	~GUI_MapViewer(void);

	int m_tile_count_x;
	int m_tile_count_y;

	double m_tile_size_x;
	double m_tile_size_y;

	int m_cursor_x;
	int m_cursor_y;

	fposition_t m_shift;

	Player* m_player;
	GameMap* m_map;
	GUI_Layer* m_GUI;

	position_t m_center;
	position_t m_initial_position;
	int m_rotate;

	struct quads_t
	{
		int value[8];
	};

	struct textcoor_t
	{
		GLdouble value[12];
	};

	struct light_val_t
	{
		GLfloat value[12];
	};

	std::vector<quads_t> m_triangles;
	std::vector<textcoor_t> m_textcoords;
	std::vector<light_val_t> m_light;

	int m_max_count;

	void on_key_press(WPARAM w) override;
	void on_mouse_click(MouseEventArgs const& e) override;
	void on_mouse_down(MouseEventArgs const& e) override;
	void on_mouse_wheel(MouseEventArgs const& e) override;
	void on_mouse_move(MouseEventArgs const& e) override;
	void on_mouse_start_drag(MouseEventArgs const& e) override;
	void on_mouse_drag(MouseEventArgs const& e)override;
	void on_mouse_end_drag(MouseEventArgs const& e)override;
	virtual void on_item_get_focus(GUI_Object* sender);
	virtual void on_lose_focus(GUI_Object* sender);
	position_t local_xy(position_t p);
	virtual void set_focus(bool state);
	void update();
	virtual bool select(int x, int y);
	void render(GraphicalController* Graph, int px, int py) override;
	void calculate();
	virtual MouseEventArgs set_local_mouse_control(MouseEventArgs const& source);
};

class gui_mapviewer_hint
{
public:

	bool m_top;

	gui_mapviewer_hint();
	virtual void render(GUI_MapViewer* owner) = 0;
	virtual void init(GUI_MapViewer* owner) {};
	virtual void render_on_cell(GUI_MapViewer* owner,MapCell* c) {};
	void draw_cell(GUI_MapViewer* owner,MapCell* cell, int index);
};

class mapviewer_hint_path : public gui_mapviewer_hint
{
public:

	GameObject* m_object;
	std::vector<MapCell*>* m_path;
	mapviewer_hint_path(std::vector<MapCell*>* path, GameObject* object);
	void render(GUI_MapViewer* owner) override;
};

class mapviewer_hint_area : public gui_mapviewer_hint
{
public:

	bool m_consider_object_size;

	GameObject* m_object;
	mapviewer_hint_area(GameObject* object, bool consider_object_size);
	void render(GUI_MapViewer* owner) override;
};

class mapviewer_hint_object_area : public gui_mapviewer_hint
{
public:

	GameObject* m_object;
	mapviewer_hint_object_area(GameObject* object);
	void render(GUI_MapViewer* owner) override;
};

class mapviewer_hint_line : public gui_mapviewer_hint
{
public:
	 
	int m_step_count;

	MapCell* m_cell;
	GameObject* m_object;
	mapviewer_hint_line( MapCell* cell,GameObject* object);
	void render(GUI_MapViewer* owner) override;
	void draw_cell(GUI_MapViewer* owner,MapCell* a);
};

class mapviewer_hint_shoot : public gui_mapviewer_hint
{
public:

	int m_step_count;
	int m_range;

	GameObject* m_object;
	mapviewer_hint_shoot(GameObject* object, int range);
	void render(GUI_MapViewer* owner) override;
	void draw_cell(GUI_MapViewer* owner,MapCell* a);
};

class mapviewer_hint_weapon_range : public gui_mapviewer_hint
{
public:

	int m_range;

	GameObject* m_object;
	mapviewer_hint_weapon_range( GameObject* object, int range);
	void render(GUI_MapViewer* owner) override;
};

class mapviewer_object_move : public gui_mapviewer_hint
{
public:

	MapCell* m_cell;
	GameObject* m_object;
	mapviewer_object_move(GameObject* object);
	void init(GUI_MapViewer* owner) override;
	void render(GUI_MapViewer* owner) override;
	void render_on_cell(GUI_MapViewer* owner,MapCell* c) override;
};

class mapviewer_object_rotate : public gui_mapviewer_hint
{
public:

	FOV_help* m_fov;
	object_direction_e m_direction;

	GameObject* m_object;
	mapviewer_object_rotate( GameObject* object);
	void init(GUI_MapViewer* owner) override;
	void render(GUI_MapViewer* owner) override;
	void render_on_cell(GUI_MapViewer* owner,MapCell* c) override;
};

#endif //GUI_MAPVIEWER_H

