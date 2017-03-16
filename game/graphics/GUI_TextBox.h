#ifndef GUI_TEXTBOX_H
#define	GUI_TEXTBOX_H

#include "GUI_Container.h"
#include <list>
#include "GUI_Text.h"
#include "Definiton.h"
#include "game/Game_log.h"

class GUI_TextBox;

class GUI_Scrollbar_vertical :
	public GUI_Object
{
public:

	GUI_TextBox* m_owner;

	int m_bar_top;
	int m_bar_height;
	int m_pos;

	bool m_is_moving;

	Event<MouseEventArgs> start_moving;
	Event<MouseEventArgs> move;
	Event<MouseEventArgs> end_moving;

	GUI_Scrollbar_vertical(GUI_TextBox* owner);
	void render(GraphicalController* Graph, int px, int py) override;
	void content_update();
	MouseEventArgs set_local_mouse_control(MouseEventArgs const& source);
	void on_mouse_move(MouseEventArgs const& e) override;
	void on_start_moving(MouseEventArgs const& e);
	void on_move(MouseEventArgs const& e);
	void on_end_moving(MouseEventArgs const& e);

};

class GUI_TextBox :
	public GUI_Container
{
public:

	GUI_Scrollbar_vertical m_scrollbar;

	std::list<GUI_Object*>::iterator start_render;
	std::list<GUI_Object*>::iterator end_render;

	GUI_TextBox();
	~GUI_TextBox();

	//virtual void Render(GraphicalController* Graph);
	virtual void add_item_control(GUI_Object* object);
	void on_mouse_wheel(MouseEventArgs const& e) override;
	void on_mouse_click(MouseEventArgs const& e) override;
	void on_mouse_down(MouseEventArgs const& e) override;
	void on_mouse_move(MouseEventArgs const& e) override;
	void set_scroll(int dy) override;
	void set_scroll2(int dy);
	void update();
	void render(GraphicalController* Graph, int px, int py) override;
	void resize(int width, int height) override;
};

class GUI_game_console :
	public GUI_TextBox
{
public:

	std::vector<GUI_TextFormat> m_styles;

	Game_log& m_log;
	GUI_game_console(Game_log& log);

	virtual void on_log_add_item(game_log_message_t& e);
};

#endif //GUI_TEXTBOX_H
