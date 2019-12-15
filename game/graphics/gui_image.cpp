#include "gui_image.h"
#include "GraphicalController.h"

GuiImage::GuiImage(const int x, const int y, const int width, const int height, const GLuint texture)
{
    m_position = position_t<int>(x, y);
    m_size = dimension_t<int>(width, height);
    m_texture = texture;
}

void GuiImage::render(GraphicalController* graph, const int px, const int py)
{
	auto& quad = graph->get_gui_quad(px, py, m_size.w, m_size.h);
}
