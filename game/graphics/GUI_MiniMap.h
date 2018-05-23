#ifndef GUI_MINIMAP_H
#define	GUI_MINIMAP_H

#include "game\graphics\GUI_Object.h"
#include "game\graphics\GUI_MapViewer.h"
#include "GameObject.h"
#include "Application.h"
#include "FOV.h"

class GUI_MapViewer;
class AI_enemy;
class Vision_list;

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
	void on_update();

private:
	bool m_canvas_create;
};

class GUI_FOV :
	public GUI_Object
{
public:

	GUI_FOV(position_t position, dimension_t size, GameObject* object);
	
	GameObject* m_object;
	FOV* m_fov;
	int m_radius;
	fdimension_t m_cell_size;

	GLint m_canvas;
	AI_enemy* m_AI;
	Vision_list* m_vision_list;

	virtual void render(GraphicalController* Graph, int px, int py);
	void render_on_canvas();
	void on_update();

private:
	bool m_canvas_create;
};

#endif //GUI_MINIMAP_H
