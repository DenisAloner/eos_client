#ifndef GUI_WINDOW_H
#define	GUI_WINDOW_H

#include "GUI_Container.h"
#include "GUI_Item.h"
#include <list>
#include "GUI_TextBox.h"
#include "Visitors.h"

class GUI_Header :
	public GUI_Container
{
public:

	Event<GUI_Object*> close;
	GUI_Header(int x, int y, int width, int height, std::u16string text);
	~GUI_Header();

	void on_close_button_click();
	virtual void resize(int width, int height);

};

class GUI_Window :
	public GUI_Container
{
public:

	Event<GUI_Object*> close;

	GUI_Window(int _x, int _y, int _width, int _height, std::u16string _Name);
	~GUI_Window();

	position_t m_initial_position;

	//virtual void on_mouse_down(MouseEventArgs const& e);
	/*virtual void on_mouse_move(MouseEventArgs const& e);*/
	void on_mouse_start_drag(MouseEventArgs const& e) override;
	void on_mouse_drag(MouseEventArgs const& e) override;
	void on_mouse_end_drag(MouseEventArgs const& e) override;
	virtual void on_close(GUI_Object* e);
	virtual void on_header_close();
	virtual void resize(int _width, int _height);
	void on_mouse_wheel(MouseEventArgs const& e) override;
	void add(GUI_Object* object) override;
	virtual rectangle_t client_rect();

private:
	GUI_Header* m_header;

};

class Visitor_object_description_getter :public Visitor_doublehandle
{
public:

	Tree<std::u16string>* m_value;

	std::list<Tree<std::u16string>*> m_active;

	Visitor_object_description_getter();

	void handle_simple(std::u16string& value);
	void handle_simple(std::u16string const& value);
	void handle_complex(std::u16string& value);
	void handle_complex(std::u16string const& value);

	void visit(Object_state& value) override;
	void visit(ObjectTag::Label& value) override;

	void begin(GameObject& value) override;
	void end(GameObject& value) override;
	void begin(Attribute_map& value) override;
	void end(Attribute_map& value) override;
	void begin(Object_state& value) override;
	void end(Object_state& value) override;
	void begin(Interaction_list& value) override;
	void end(Interaction_list&value) override;
	void begin(Parameter_list& value) override;
	void end(Parameter_list& value) override;
	void begin(Vision_list& value) override;
	void end(Vision_list& value) override;
	void begin(Vision_component& value) override;
	void end(Vision_component& value) override;
	void begin(Parts_list& value) override;
	void end(Parts_list& value) override;
	void begin(Object_part& value) override;
	void end(Object_part& value) override;
	void begin(Tag_list& value) override;
	void end(Tag_list& value) override;
	
};

class GUI_description_window :
	public GUI_Window
{
public:

	GameObject* m_object;
	GUI_TextBox* m_textbox;
	GUI_description_window(int x, int y, int width, int height, std::u16string Name, GameObject*& object);
	std::list<std::u16string> m_text;
	void update_info();
private:
	void add_tree(Tree<std::u16string>& value,int level);

};

class GUI_Body;

class GUI_body_window :
	public GUI_Window
{
public:

	GameObject* m_object;
	GUI_Body* m_item;
	GUI_body_window(int x, int y, int width, int height, std::u16string Name, GameObject*& object);
	std::list<std::u16string> m_text;
	void update_info();

};

#endif GUI_WINDOW_H
