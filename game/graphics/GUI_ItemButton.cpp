#include "GUI_ItemButton.h"
#include "GraphicalController.h"

GUI_ItemButton::GUI_ItemButton()
{
}

GUI_ItemButton::~GUI_ItemButton()
{
}

//bool GUI_ItemButton::check_region(MouseEventArgs const& e)
//{
//    const auto radius = m_size.w * 0.5F;
//    const auto local_x = (e.position.x - (m_position.x + radius));
//    const auto local_y = (e.position.y - (m_position.y + radius));
//    if (local_x * local_x + local_y * local_y - radius * radius <= 0) {
//        return true;
//    }
//    return false;
//}

void GUI_ItemButton::render(GraphicalController* graph, int px, int py)
{
    auto& quad = graph->get_gui_quad(px, py, m_size.w, m_size.h);
    auto& tile = graph->atlas_tiles[u"box_button"];
    auto& t = tile.texture;
    quad.vertex[0].texture[0] = t.x;
    quad.vertex[0].texture[1] = t.y;
    quad.vertex[0].texture[2] = tile.layer;

    quad.vertex[1].texture[0] = t.x;
    quad.vertex[1].texture[1] = t.bottom();
    quad.vertex[1].texture[2] = tile.layer;

    quad.vertex[2].texture[0] = t.right();
    quad.vertex[2].texture[1] = t.bottom();
    quad.vertex[2].texture[2] = tile.layer;

    quad.vertex[3].texture[0] = t.right();
    quad.vertex[3].texture[1] = t.y;
    quad.vertex[3].texture[2] = tile.layer;
}

void GUI_ItemButton::on_mouse_move(MouseEventArgs const& e)
{
    //SetCursor(LoadCursor(NULL, IDC_HAND));
}