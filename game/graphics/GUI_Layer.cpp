#include "GUI_Layer.h"
#include <Windows.h>

GUI_Layer::GUI_Layer()
{
	m_focus=nullptr;
}


GUI_Layer::~GUI_Layer(void)
{
}

MouseEventArgs GUI_Layer::set_local_mouse_position(MouseEventArgs const& source)
{
	return MouseEventArgs(position_t(source.position.x - m_position.x, source.position.y - m_position.y), source.key, source.value);
}

void GUI_Layer::on_mouse_click(MouseEventArgs const& e)
{ 
	MouseEventArgs LocalMouseEventArgs = set_local_mouse_position(e);
	for(std::list<GUI_Object*>::iterator Current=m_items.begin();Current!=m_items.end();Current++)
	{
		
		if ((*Current)->check_region(LocalMouseEventArgs))
		{
			(*Current)->mouse_click(LocalMouseEventArgs);
			return;
		}
	}
}

void GUI_Layer::on_mouse_down(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = set_local_mouse_position(e);
	for (std::list<GUI_Object*>::iterator Current = m_items.begin(); Current != m_items.end(); Current++)
	{
		if ((*Current)->check_region(LocalMouseEventArgs))
		{
			(*Current)->mouse_down(LocalMouseEventArgs);
			return;
		}
	}
}

void GUI_Layer::on_mouse_move(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = set_local_mouse_position(e);
	if (m_focus)
	{
		m_focus->mouse_move(LocalMouseEventArgs);
	}
}

void GUI_Layer::on_mouse_wheel(MouseEventArgs const& e)
{
	if (m_focus)
	{
		m_focus->mouse_wheel(e);
	}
}

void GUI_Layer::render(GraphicalController* Graph, int px, int py)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgramObjectARB(0);
	for (std::list<GUI_Object*>::reverse_iterator Current =m_items.rbegin(); Current != m_items.rend(); Current++)
	{
		(*Current)->render(Graph, px + (*Current)->m_position.x, py + (*Current)->m_position.y);
	}
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void GUI_Layer::on_key_press(WPARAM w)
{
	if(m_focus)
	{
		m_focus->key_press(w);
	}
}

void GUI_Layer::set_focus(bool state)
{
	focused=state;
	if(state)
	{
		if(m_focus!=nullptr)
		{
			m_focus->set_focus(false);
		}
		get_focus(this);
	} else {
		if(m_focus!=nullptr)
		{
			m_focus->set_focus(false);
			m_focus=nullptr;
		}
		lose_focus(this);
	}
}

void GUI_Layer::add(GUI_Object* object)
{
	m_items.push_back(object);
	object->get_focus+=std::bind(&GUI_Layer::on_item_get_focus,this,std::placeholders::_1);
	object->destroy += std::bind(&GUI_Layer::remove, this, std::placeholders::_1);
}

void GUI_Layer::add_front(GUI_Object* object)
{
	m_items.push_front(object);
	object->get_focus+=std::bind(&GUI_Layer::on_item_get_focus,this,std::placeholders::_1);
	object->destroy += std::bind(&GUI_Layer::remove, this, std::placeholders::_1);
}

void GUI_Layer::remove(GUI_Object* object)
{
	if(m_focus==object)
	{
		m_focus=nullptr;
	}
	m_items.remove(object);
}

bool GUI_Layer::check_region(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = set_local_mouse_position(e);
	if (this->m_position.x <= e.position.x&&this->m_position.x + m_size.w >= e.position.x&&this->m_position.y <= e.position.y&&this->m_position.y + m_size.h >= e.position.y)
	{
		for(std::list<GUI_Object*>::iterator Current=m_items.begin();Current!=m_items.end();Current++)
		{
			if ((*Current)->check_region(LocalMouseEventArgs))
			{
				return true;
			}
		}
	}
	return false;
}

void GUI_Layer::on_get_focus(GUI_Object* sender)
{
}

void GUI_Layer::on_item_get_focus(GUI_Object* sender)
{
	if(m_focus!=sender)
	{
	set_focus(true);
	m_focus=sender;
	}
}