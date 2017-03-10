#include "GUI_Text.h"


GUI_Text::GUI_Text(std::u16string text)
{
	m_format = new GUI_TextFormat(8, 17, RGBA_t(1.0, 1.0, 1.0, 1.0));
	set(text);
}

GUI_Text::GUI_Text(std::u16string text, GUI_TextFormat* format) : m_format(format)
{
	set(text);
}

GUI_Text::~GUI_Text()
{
}

void GUI_Text::render(GraphicalController* Graph, int px, int py)
{
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glColor4d(m_format->m_color.R, m_format->m_color.G, m_format->m_color.B, m_format->m_color.A);
	Graph->output_text(px, py, m_text, m_format->m_symbol_size.w, m_format->m_symbol_size.h);
}

void GUI_Text::set(std::u16string t)
{
	m_text = t;
	m_size.w = m_text.length() * m_format->m_symbol_size.w;
	m_size.h = (Application::instance().m_graph->m_face->size->metrics.ascender - Application::instance().m_graph->m_face->size->metrics.descender) >> 6;
}
