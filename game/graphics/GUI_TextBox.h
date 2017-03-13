#ifndef GUI_TEXTBOX_H
#define	GUI_TEXTBOX_H

#include "GUI_Container.h"
#include <list>
#include "GUI_Text.h"
#include "Definiton.h"
#include "game/Game_log.h"

class GUI_TextBox :
	public GUI_Container
{
public:

	std::list<GUI_Object*>::iterator start_render;
	std::list<GUI_Object*>::iterator end_render;

	GUI_TextBox();
	~GUI_TextBox();

	//virtual void Render(GraphicalController* Graph);
	virtual void add_item_control(GUI_Object* object);
	void on_mouse_wheel(MouseEventArgs const& e) override;
	void set_scroll(int dy) override;
	void update();
	void render(GraphicalController* Graph, int px, int py) override;
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
