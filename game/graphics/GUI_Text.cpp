#include "GUI_Text.h"
#include "Application.h"

GUI_Text::GUI_Text(const std::u16string& text)
{
    m_format = new GUI_TextFormat(8, 17, RGBA_t(1.0, 1.0, 1.0, 1.0));
    set(text);
}

GUI_Text::GUI_Text(const std::u16string& text, GUI_TextFormat* format)
    : m_format(format)
{
    set(text);
}

GUI_Text::~GUI_Text()
{
}

void GUI_Text::render(GraphicalController* graph, int px, int py)
{
    auto x = px;
	auto y = py;
    y = y + (graph->m_face->size->metrics.ascender >> 6);
    for (auto k : m_text) {
        auto& fs = graph->get_symbol(k);
        if (k != 32) {

            auto& t = fs.texture;
            auto& quad = graph->get_gui_quad(x, y + fs.bearing.h, fs.size.w - 1, -fs.size.h + 1);

            quad.vertex[0].texture[0] = t.x;
            quad.vertex[0].texture[1] = t.y;
            quad.vertex[0].texture[2] = float(1);

            quad.vertex[1].texture[0] = t.x;
            quad.vertex[1].texture[1] = t.bottom();
            quad.vertex[1].texture[2] = float(1);

            quad.vertex[2].texture[0] = t.right();
            quad.vertex[2].texture[1] = t.bottom();
            quad.vertex[2].texture[2] = float(1);

            quad.vertex[3].texture[0] = t.right();
            quad.vertex[3].texture[1] = t.y;
            quad.vertex[3].texture[2] = float(1);
        	
        	quad.vertex[0].color[0] = m_format->m_color.r;
            quad.vertex[0].color[1] = m_format->m_color.g;
            quad.vertex[0].color[2] = m_format->m_color.b;
            quad.vertex[1].color[0] = m_format->m_color.r;
            quad.vertex[1].color[1] = m_format->m_color.g;
            quad.vertex[1].color[2] = m_format->m_color.b;
            quad.vertex[2].color[0] = m_format->m_color.r;
            quad.vertex[2].color[1] = m_format->m_color.g;
            quad.vertex[2].color[2] = m_format->m_color.b;
            quad.vertex[3].color[0] = m_format->m_color.r;
            quad.vertex[3].color[1] = m_format->m_color.g;
            quad.vertex[3].color[2] = m_format->m_color.b;
        }
        x += fs.size.w;
    }
}

void GUI_Text::set(const std::u16string& t)
{
    m_text = t;
    m_size.w = Application::instance().m_graph->measure_text_width(m_text);
    m_size.h = (Application::instance().m_graph->m_face->size->metrics.ascender - Application::instance().m_graph->m_face->size->metrics.descender) >> 6;
}
