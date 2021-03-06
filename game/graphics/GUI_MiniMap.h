#pragma once
#include "game\graphics\GUI_Object.h"
#include "game\graphics\GUI_MapViewer.h"
#include "GameObject.h"
#include "Application.h"

class GUI_MapViewer;

class GUI_MiniMap :
	public GUI_Object
{
public:

	GUI_MiniMap(position_t position, dimension_t size, GUI_MapViewer* map_viewer);
	~GUI_MiniMap();

	GUI_MapViewer* m_map_viewer;

	GLint m_canvas;

	fdimension_t m_cell_size;

	virtual void render(GraphicalController* Graph, int px, int py);
	void render_on_canvas();
};

