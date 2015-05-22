#ifndef GUI_POPMENU_H
#define	GUI_POPMENU_H

#include <list>
#include "GUI_Object.h"
#include "GUI_Container.h"
#include "GameObject.h"
#include "Application.h"


class GameObject;
class Application;

template<typename _val>
class GUI_PopMenu :
	public GUI_Container
{
public:

	class Item :
		public GUI_Object
	{
	public:

		std::string m_text;
		_val m_object;

		GUI_PopMenu* m_owner;

		Item(void){ m_size.h = 18; }
		~Item(void);

		virtual void on_mouse_click(MouseEventArgs const& e)
		{
			set_focus(true);
			m_owner->item_click(this);
		}

		virtual void on_mouse_move(MouseEventArgs const& e) { set_focus(true); }
		/*virtual void on_get_focus(GUI_Object* sender) { m_object->m_selected = true; }
		virtual void on_lose_focus(GUI_Object* sender) { m_object->m_selected = false; }*/


		virtual void render(GraphicalController* Graph, int px, int py)
		{
			glEnable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			if (focused)
			{
				glColor4d(1.0, 1.0, 1.0, 0.75);
				Graph->draw_sprite(px, py, px, py + m_size.h, px + m_size.w, py + m_size.h, px + m_size.w, py);
				glColor4d(0, 0, 0, 1);
				glEnable(GL_TEXTURE_2D);
				Graph->output_text(px + 4, py, m_text, 8, 17);
			}
			else {
				glColor4d(1, 1, 1, 1);
				glEnable(GL_TEXTURE_2D);
				Graph->output_text(px, py, m_text, 8, 17);
			}
		}
	};

	Event<Item*> item_click;

	GUI_PopMenu(void) :GUI_Container(0, 0, 0, 0){ item_click += std::bind(&GUI_PopMenu::on_item_click, this, std::placeholders::_1); }
	~GUI_PopMenu(void);

	virtual void add(std::string Text, _val Object)
	{
		GUI_PopMenu::Item* object;
		object = new GUI_PopMenu::Item();
		object->m_text = Text;
		object->m_owner = this;
		object->m_object = Object;
		if (!m_items.empty())
		{
			GUI_Object* Back = m_items.back();
			object->m_position.x = 0;
			object->m_position.y = Back->m_position.y + Back->m_size.h;
		}
		else
		{
			object->m_position.x = 0;
			object->m_position.y = 0;
		}
		GUI_Layer::add(object);
		std::size_t maxlen = 0;
		for (std::list<GUI_Object*>::iterator Current = m_items.begin(); Current != m_items.end(); Current++)
		{
			object = (GUI_PopMenu::Item*)(*Current);
			if (object->m_text.length() > maxlen)
			{
				maxlen = object->m_text.length();
			}
		}
		resize(maxlen * 9 + 8, m_items.size() * 18);
		for (std::list<GUI_Object*>::iterator Current = m_items.begin(); Current != m_items.end(); Current++)
		{
			(*Current)->m_size.w = m_size.w - 1;
		}
	}
	virtual void on_lose_focus(GUI_Object* sender){ destroy(this); }
	virtual void on_mouse_move(MouseEventArgs const& e)
	{
		for (std::list<GUI_Object*>::iterator Current = m_items.begin(); Current != m_items.end(); Current++)
		{
			if ((*Current)->check_region(MouseEventArgs(position_t(e.position.x - m_position.x, e.position.y - m_position.y), e.key, e.value)))
			{
				(*Current)->mouse_move(MouseEventArgs(position_t(e.position.x - m_position.x, e.position.y - m_position.y), e.key, e.value));
				return;
			}
		}
	}

	virtual void on_item_click(Item* sender){};
};

class Select_object_popmenu :public GUI_PopMenu < GameObject* >
{
public:

	virtual void on_item_click(Item* sender);
};

#endif //GUI_POPMENU_H
