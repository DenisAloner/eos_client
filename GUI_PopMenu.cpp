#include "GUI_PopMenu.h"


GUI_PopMenu::GUI_PopMenu(void) :GUI_Container(0, 0, 0, 0)
{
}

GUI_PopMenu::~GUI_PopMenu(void)
{
}

void GUI_PopMenu::add(std::string Text, GameObject* Object)
{
	GUI_PopMenuItem* object;
	object = new GUI_PopMenuItem();
	object->m_text = Text;
	object->m_owner = this;
	object->m_object = Object;
	if (!m_item_controls->m_items.empty())
	{
		GUI_Object* Back = m_item_controls->m_items.back();
		object->m_position.x = 0;
		object->m_position.y = Back->m_position.y + Back->m_size.y;
	/*	if (object->y + object->height>height)
		{
			Scroll.y -= object->height;
		}*/
	}
	else
	{
		object->m_position.x = 0;
		object->m_position.y = 0;
	}
	m_item_controls->add(object);
	std::size_t maxlen = 0;
	for (std::list<GUI_Object*>::iterator Current = m_item_controls->m_items.begin(); Current != m_item_controls->m_items.end(); Current++)
	{
		object = (GUI_PopMenuItem*)(*Current);
		if (object->m_text.length()>maxlen)
		{
			maxlen = object->m_text.length();
		}
	}
	resize(maxlen * 9+2, m_item_controls->m_items.size() * 18);
	for (std::list<GUI_Object*>::iterator Current = m_item_controls->m_items.begin(); Current != m_item_controls->m_items.end(); Current++)
	{
		(*Current)->m_size.x = m_size.x - 1;
	}
}

void GUI_PopMenu::on_lose_focus(GUI_Object* sender)
{
	destroy(this);
}

void GUI_PopMenu::on_under_cursor(MouseEventArgs const& e)
{
	for (std::list<GUI_Object*>::iterator Current = m_item_controls->m_items.begin(); Current != m_item_controls->m_items.end(); Current++)
	{
		if ((*Current)->check_region(MouseEventArgs(GPosition(e.position.x - m_position.x, e.position.y - m_position.y), e.flags)))
		{
			(*Current)->under_cursor(MouseEventArgs(GPosition(e.position.x - m_position.x, e.position.y - m_position.y), e.flags));
			return;
		}
	}
}


GUI_PopMenuItem::GUI_PopMenuItem(void)
{
	m_size.y = 18;
}

GUI_PopMenuItem::~GUI_PopMenuItem(void)
{
}


void GUI_PopMenuItem::on_mouse_click(MouseEventArgs const& e)
{
	set_focus(true);
	if (Application::instance().m_message_queue.m_reader)
	{
		Parameter_GameObject* p = new Parameter_GameObject();
		p->m_object = m_object;
		Application::instance().m_message_queue.push(p);
	}
	//Owner->Destroy(Owner);
	//Owner->Game->GUI->MapViewer->SetFocus(true);
	//Owner->Game->GUI->MapViewer->UnderCursor(MouseEventArgs(Point(e.Position.x + Owner->x, e.Position.y + Owner->y), e.Flags));
	//Owner->Game->GUI->MapViewer->MouseClick(MouseEventArgs(Point(e.Position.x + Owner->x, e.Position.y + Owner->y), e.Flags));
	set_focus(false);
	m_owner->destroy(m_owner);
	/*char buf2[32];
	itoa(e.Position.y + Owner->y, buf2, 10);
	MessageBox(NULL, buf2, "Down", MB_OK);*/
}


void GUI_PopMenuItem::render(GraphicalController* Graph, int px, int py)
{
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	if (focused)
	{
		glColor4d(1.0, 1.0,1.0, 0.75);
		Graph->draw_sprite(px, py, px, py + m_size.y, px + m_size.x, py + m_size.y, px + m_size.x, py);
		glColor4d(0, 0, 0, 1);
	}
	else {
		glColor4d(1, 1, 1, 1);
		//glColor4d(1, 1, 1, 0.25);
		//Graph->draw_sprite(px, py, px, py + height, px + width, py + height, px + width, py);
	}
	glEnable(GL_TEXTURE_2D);
	Graph->output_text(px, py, m_text, 8, 17);
}

void GUI_PopMenuItem::on_under_cursor(MouseEventArgs const& e)
{
	set_focus(true);
}

void GUI_PopMenuItem::on_get_focus(GUI_Object* sender)
{
	m_object->m_selected = true;
}

void GUI_PopMenuItem::on_lose_focus(GUI_Object* sender)
{
	m_object->m_selected = false;
}
