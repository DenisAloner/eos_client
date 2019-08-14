#include "GUI_button.h"


GUI_button::GUI_button(int x, int y, int width, int height, std::u16string text)
{
	m_position = position_t(x, y);
	m_size = dimension_t(width, height);
	m_text = new GUI_Text(text);
	m_text->m_position = Application::instance().m_graph->center_aling_to_point(m_size.w*0.5, (m_size.h - m_text->m_size.h)*0.5, m_text->m_text);
}


GUI_button::~GUI_button()
{
}

void GUI_button::render(GraphicalController* Graph, int px, int py)
{
	glEnable(GL_SCISSOR_TEST);
	if (Graph->add_scissor(rectangle<float>(float(px), float(py), float(m_size.w), float(m_size.h))))
	{
		Graph->blur_rect(px, py, m_size.w, m_size.h);
		glEnable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glColor4d(0.0, 0.0, 0.0, 0.5);
		const GraphicalController::rectangle_t rect(px, py, m_size.w, m_size.h);
		Graph->draw_sprite(rect);
		glEnable(GL_TEXTURE_2D);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		m_text->render(Graph, px + m_text->m_position.x, py + m_text->m_position.y);
		Graph->remove_scissor();
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		Graph->draw_rectangle(GraphicalController::rectangle_t(px, py, m_size.w, m_size.h));
	}
}

GUI_Mainmenu_button::GUI_Mainmenu_button(int x, int y, int width, int height, std::u16string text, parameter_type_e kind) :GUI_button(x, y, width, height, text), m_parameter(new Parameter(kind))
{
}

void GUI_Mainmenu_button::on_mouse_click(MouseEventArgs const& e)
{
	GUI_Object::on_mouse_click(e);
	if (Application::instance().m_message_queue.m_reader)
	{
		Application::instance().m_message_queue.push(m_parameter);
	}
}
