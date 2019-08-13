#ifndef GUI_POPMENU_H
#define	GUI_POPMENU_H

#include <list>
#include "GUI_Object.h"
#include "GUI_Container.h"
#include "GameObject.h"
#include "Application.h"
#include <algorithm>

class GameObject;
class Application;

template<typename _val>
class GUI_PopMenu :public GUI_Container
{
public:

	class Item : public GUI_Object
	{
	public:

		std::u16string m_text;
		_val m_object;

		GUI_PopMenu* m_owner;

		Item(){ m_size.h = (Application::instance().m_graph->m_face->size->metrics.ascender - Application::instance().m_graph->m_face->size->metrics.descender) >> 6; }
		~Item();

		void on_mouse_click(MouseEventArgs const& e) override
		{
			set_focus(true);
			m_owner->item_click(this);
		}

		void on_mouse_move(MouseEventArgs const& e) override { set_focus(true); }
		/*virtual void on_get_focus(GUI_Object* sender) { m_object->m_selected = true; }
		virtual void on_lose_focus(GUI_Object* sender) { m_object->m_selected = false; }*/


		void render(GraphicalController* graph, int px, int py) override
		{
			glEnable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			if (focused)
			{
				glColor4d(1.0, 1.0, 1.0, 0.75);
				const GraphicalController::rectangle_t rect(px,py, m_size.w,m_size.h);
				graph->draw_sprite(rect);
				glColor4d(0, 0, 0, 1);
				glEnable(GL_TEXTURE_2D);
				graph->output_text(px + 4, py, m_text, 8, 17);
			}
			else {
				glColor4d(1, 1, 1, 1);
				glEnable(GL_TEXTURE_2D);
				graph->output_text(px, py, m_text, 8, 17);
			}
		}
	};

	Event<Item*> item_click;

	GUI_PopMenu() :GUI_Container(0, 0, 0, 0){ item_click += std::bind(&GUI_PopMenu::on_item_click, this, std::placeholders::_1); }
	~GUI_PopMenu();

	virtual void add_item(std::u16string text, _val tag)
	{
		Item* object = new Item();
		object->m_text = text;
		object->m_owner = this;
		object->m_object = tag;
		if (!m_items.empty())
		{
			const auto back = m_items.back();
			object->m_position.x = 0;
			object->m_position.y = back->m_position.y + back->m_size.h;
		}
		else
		{
			object->m_position.x = 0;
			object->m_position.y = 0;
		}
		GUI_Layer::add(object);
		std::size_t max_length = 0;
		std::size_t height = 0;
		for (auto& current: m_items)
		{
			object = static_cast<Item*>(current);
			max_length = std::max<std::size_t>(Application::instance().m_graph->measure_text_width(object->m_text), max_length);
		}
		resize(max_length + 8, m_items.size() * ((Application::instance().m_graph->m_face->size->metrics.ascender - Application::instance().m_graph->m_face->size->metrics.descender) >> 6));
		for (auto& current : m_items)
		{
			current->m_size.w = m_size.w - 1;
		}
	};

	void on_lose_focus(GUI_Object* sender) override { destroy(this); }

	void on_mouse_move(MouseEventArgs const& e) override
	{
		for (auto& current : m_items)
		{
			if (current->check_region(MouseEventArgs(position_t(e.position.x - m_position.x, e.position.y - m_position.y), e.key, e.value)))
			{
				current->mouse_move(MouseEventArgs(position_t(e.position.x - m_position.x, e.position.y - m_position.y), e.key, e.value));
				return;
			}
		}
	};

	virtual void on_item_click(Item* sender){};
};

class Select_object_popmenu :public GUI_PopMenu < GameObject* >
{
public:

	void on_item_click(Item* sender) override;
};

#endif //GUI_POPMENU_H
