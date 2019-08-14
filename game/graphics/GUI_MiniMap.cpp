#include "game\graphics\GUI_MiniMap.h"


GUI_MiniMap::GUI_MiniMap(position_t position, dimension_t size, GUI_MapViewer* map_viewer) :m_map_viewer(map_viewer)
{
	m_position = position;
	m_size = size;
	dimension_t cr = Application::instance().m_size;
	m_cell_size = dimension<float>(static_cast<float>(cr.w) / static_cast<float>(m_map_viewer->m_map->m_size.dx), static_cast<float>(cr.h) / static_cast<float>(m_map_viewer->m_map->m_size.dy));
	m_canvas_create = false;
	m_map_viewer->m_map->update += std::bind(&GUI_MiniMap::on_update, this);
}


GUI_MiniMap::~GUI_MiniMap()
{
}

void GUI_MiniMap::on_update()
{
	execute_in_render_thread(std::bind(&GUI_MiniMap::render_on_canvas, this));
}

void GUI_MiniMap::render_on_canvas()
{
	if (!m_canvas_create)
	{
		dimension_t cr = Application::instance().m_size;
		m_canvas = Application::instance().m_graph->create_empty_texture(dimension_t(cr.w, cr.h));
		m_canvas_create = true;
	};
	double x0, y0, x1, y1, x2, y2, x3, y3;
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_SCISSOR_TEST);
	GLint default_fb = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &default_fb);
	const GLuint fbo = Application::instance().m_graph->m_FBO;
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_canvas, 0);
	glUseProgramObjectARB(0);
	glClearColor(0.0, 0.0, 0.0, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT); 
	for (int y = 0; y < m_map_viewer->m_map->m_size.dy; y++)
	{
		for (int x = 0; x < m_map_viewer->m_map->m_size.dx; x++)
		{
			x0 = x*m_cell_size.w;
			y0 = (m_map_viewer->m_map->m_size.dy - y)*m_cell_size.h;
			x1 = x0;
			y1 = (m_map_viewer->m_map->m_size.dy - (y + 1))*m_cell_size.h;
			x2 = (x + 1)*m_cell_size.w;
			y2 = y1;
			x3 = x2;
			y3 = y0;
			if (m_map_viewer->m_map->get(0,y, x).m_mark==true)
			{
				glColor4d(1.0, 0.5, 0.0, 1.0);
				glBegin(GL_QUADS);
				glVertex2d(x0, y0);
				glVertex2d(x1, y1);
				glVertex2d(x2, y2);
				glVertex2d(x3, y3);
				glEnd();
				continue;
			}
			for (std::list<GameObject*>::iterator obj = m_map_viewer->m_map->get(0,y, x).m_items.begin(); obj != m_map_viewer->m_map->get(0,y, x).m_items.end(); ++obj)
			{
				glColor4d(0.0, 0.0, 0.0, 0.0);
				if ((*obj)->m_name == u"floor")
				{
					glColor4d(0.3, 0.3, 0.3, 1.0);
				}
				if ((*obj)->m_name == u"wall")
				{
					glColor4d(0.0, 0.3, 0.5, 1.0);
				}
				if ((*obj)->m_name == u"elf")
				{
					glColor4d(0.0, 0.7, 0.0, 1.0);
				}
				if ((*obj)->m_name == u"door")
				{
					glColor4d(0.3, 0.1, 0.0, 1.0);
				}
				if ((*obj)->m_name == u"torch")
				{
					glColor4d(0.9, 0.8, 0.0, 1.0);
				}
				if ((*obj)->m_name == u"snake")
				{
					glColor4d(0.7, 0.0, 0.0, 1.0);
				}
				glBegin(GL_QUADS);
				glVertex2d(x0, y0);
				glVertex2d(x1, y1);
				glVertex2d(x2, y2);
				glVertex2d(x3, y3);
				glEnd();
			}
		}
	}
	glBindTexture(GL_TEXTURE_2D, m_canvas);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindFramebuffer(GL_FRAMEBUFFER, default_fb);
}


void GUI_MiniMap::render(GraphicalController* graph, int px, int py)
{ 
	if (m_canvas_create)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
		//glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01);
		//glGenerateMipmap(GL_TEXTURE_2D);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, m_canvas);
		GraphicalController::rectangle_t rect(px, py, m_size.w, m_size.h);
		graph->draw_sprite(rect);
		glDisable(GL_TEXTURE_2D);
		double x0, y0, x1, y1;
		x0 = px + (m_map_viewer->m_center.x - m_map_viewer->m_tile_count_x / 2)*m_size.w / static_cast<float>(m_map_viewer->m_map->m_size.dx);
		if (x0 < px) { x0 = px; }
		if (x0 > px + m_size.w) { x0 = px + m_size.w; }
		y0 = py + (m_map_viewer->m_map->m_size.dy - (m_map_viewer->m_center.y - m_map_viewer->m_tile_count_y / 2))*m_size.h / static_cast<float>(m_map_viewer->m_map->m_size.dy);
		if (y0 < py) { y0 = py; }
		if (y0 > py + m_size.h) { y0 = py + m_size.h; }
		x1 = px + (m_map_viewer->m_center.x + m_map_viewer->m_tile_count_x / 2)*m_size.w / static_cast<float>(m_map_viewer->m_map->m_size.dx);
		if (x1 < px) { x1 = px; }
		if (x1 > px + m_size.w) { x1 = px + m_size.w; }
		y1 = py + (m_map_viewer->m_map->m_size.dy - (m_map_viewer->m_center.y + m_map_viewer->m_tile_count_y / 2))*m_size.h / static_cast<float>(m_map_viewer->m_map->m_size.dy);
		if (y1 < py) { y1 = py; }
		if (y1 > py + m_size.h) { y1 = py + m_size.h; }
		glColor4d(0.0, 1.0, 0.5, 0.3);
		graph->draw_sprite(rect);
	}
}

GUI_FOV::GUI_FOV(position_t position, dimension_t size, GameObject* object)
{
	m_canvas_create = false;
	m_object = object;
	m_AI = static_cast<AI_enemy*>(m_object->m_active_state->m_ai);
	m_fov = m_AI->m_fov;
	
	m_vision_list = static_cast<Vision_list*>(m_object->m_active_state->get_list(interaction_e::vision));
	m_radius = m_vision_list->m_max_radius;
	m_position = position;
	m_size = size;
	m_AI->update += std::bind(&GUI_FOV::on_update, this);
}

void GUI_FOV::on_update()
{
	execute_in_render_thread(std::bind(&GUI_FOV::render_on_canvas, this));
}


void GUI_FOV::render(GraphicalController* graph, int px, int py)
{
	if (m_canvas_create)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
		//glBindTexture(GL_TEXTURE_2D, Application::instance().m_graph->m_empty_01);
		//glGenerateMipmap(GL_TEXTURE_2D);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, m_canvas);
		GraphicalController::rectangle_t rect(px, py, m_size.w, m_size.h);
		graph->draw_sprite(rect);
	}
}

void GUI_FOV::render_on_canvas()
{
	dimension_t cr = Application::instance().m_size;
	if (!m_canvas_create)
	{
		m_canvas = Application::instance().m_graph->create_empty_texture(dimension_t(cr.w, cr.h));
		m_canvas_create = true;
	};
	m_radius = m_vision_list->m_max_radius;
	m_cell_size = dimension<float>(static_cast<float>(cr.w) / static_cast<float>(m_fov->m_view.l + m_fov->m_view.r + 1), static_cast<float>(cr.h) / static_cast<float>(m_fov->m_view.d + m_fov->m_view.u + 1));
	double x0, y0, x1, y1, x2, y2, x3, y3;
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_SCISSOR_TEST);
	GLint default_fb = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &default_fb);
	const GLuint fbo = Application::instance().m_graph->m_FBO;
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_canvas, 0);
	glUseProgramObjectARB(0);
	glClearColor(0.0, 0.0, 0.0, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	int width = m_fov->m_view.l + m_fov->m_view.r;
	int height = m_fov->m_view.d + m_fov->m_view.u;
	for (int y = 0; y < height + 1; y++)
	{
		for (int x = 0; x < width+1; x++)
		{
			x0 = x*m_cell_size.w;
			y0 = (height + 1 - y)*m_cell_size.h;
			x1 = x0;
			y1 = (height +1 - (y + 1))*m_cell_size.h;
			x2 = (x + 1)*m_cell_size.w;
			y2 = y1;
			x3 = x2;
			y3 = y0;
			glColor4d(0.0, 0.0, 0.0, 1.0);
			if (m_fov->m_map[y][x].opaque){
				if (m_fov->m_map[y][x].visible){
					glColor4d(1.0, 0.0, 0.0, 1.0);
				}
				else glColor4d(0.5, 0.0, 0.0, 1.0);
			}
			else {
				if (m_fov->m_map[y][x].visible){
					glColor4d(0.9, 0.9, 0.9, 1.0);
				}
				else glColor4d(0.5, 0.5, 0.5, 1.0);
			}
			glBegin(GL_QUADS);
			glVertex2d(x0, y0);
			glVertex2d(x1, y1);
			glVertex2d(x2, y2);
			glVertex2d(x3, y3);
			glEnd();
		}
	}
	glBindTexture(GL_TEXTURE_2D, m_canvas);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindFramebuffer(GL_FRAMEBUFFER, default_fb);
}
