#include "GUI_button.h"
#include "Application.h"

GUI_button::GUI_button(int x, int y, int width, int height, std::u16string text)
{
    m_position = position_t<int>(x, y);
    m_size = dimension_t<int>(width, height);
    m_text = new GUI_Text(text);
    m_text->m_position = Application::instance().m_graph->center_align_to_point(m_size.w * 0.5, (m_size.h - m_text->m_size.h) * 0.5, m_text->m_text);
}

GUI_button::~GUI_button()
{
}

void GUI_button::render(GraphicalController* graph, int px, int py)
{
    if (graph->add_scissor(px, py, m_size.w, m_size.h)) {

        auto& background = graph->atlas_tiles[u"background"];
        for (int y = 0; y < m_size.h; y += 280) {
            for (int x = 0; x < m_size.w; x += 280) {
                auto& tile = graph->get_gui_quad(px + x, py + y, 280, 280, background);
                tile.vertex[0].color[3] = 0.5f;
                tile.vertex[1].color[3] = 0.5f;
                tile.vertex[2].color[3] = 0.5f;
                tile.vertex[3].color[3] = 0.5f;
            }
        }

        m_text->render(graph, px + m_text->m_position.x, py + m_text->m_position.y);

        auto& border_tile = graph->atlas_tiles[u"border"];
        if (graph->add_scissor(px + 32, py, m_size.w - 64, m_size.h)) {
            for (int i = px + 32; i < px + m_size.w - 32; i += 64) {
                graph->get_gui_quad(i, py, 64, 12, border_tile);
                graph->get_gui_quad(i, py + m_size.h - 12, 64, 12, border_tile, tile_options_e::FLIP_Y);
            }
            graph->remove_scissor();
        }
        auto& border_vertical_tile = graph->atlas_tiles[u"border_vertical"];
        if (graph->add_scissor(px, py + 32, m_size.w, m_size.h - 64)) {
            for (int i = py + 32; i < py + m_size.h - 32; i += 64) {
                graph->get_gui_quad(px, i, 12, 64, border_vertical_tile);
                graph->get_gui_quad(px + m_size.w - 12, i, 12, 64, border_vertical_tile, tile_options_e::FLIP_X);
            }
            graph->remove_scissor();
        }
        auto& corner_tile = graph->atlas_tiles[u"corner"];
        graph->get_gui_quad(px, py + m_size.h - 32, 32, 32, corner_tile);
        graph->get_gui_quad(px + m_size.w - 32, py + m_size.h - 32, 32, 32, corner_tile, tile_options_e::FLIP_X);
        graph->get_gui_quad(px, py, 32, 32, corner_tile, tile_options_e::FLIP_Y);
        graph->get_gui_quad(px + m_size.w - 32, py, 32, 32, corner_tile, tile_options_e::FLIP_X | tile_options_e::FLIP_Y);

        graph->remove_scissor();
    }
}

GUI_Mainmenu_button::GUI_Mainmenu_button(int x, int y, int width, int height, std::u16string text, parameter_type_e kind)
    : GUI_button(x, y, width, height, text)
    , m_parameter(new Parameter(kind))
{
}

void GUI_Mainmenu_button::on_mouse_click(MouseEventArgs const& e)
{
    GUI_Object::on_mouse_click(e);
    if (Application::instance().m_message_queue.m_reader) {
        Application::instance().m_message_queue.push(m_parameter);
    }
}
