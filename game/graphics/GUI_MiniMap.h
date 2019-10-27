#ifndef GUI_MINIMAP_H
#define GUI_MINIMAP_H

#include "FOV.h"
#include "game/graphics/GUI_MapViewer.h"
#include "game/graphics/GUI_Object.h"

class GUI_MapViewer;
class AI_enemy;
class Vision_list;
class FOV;
class GameObject;

class GUI_MiniMap : public GUI_Object {
public:
    GUI_MiniMap(position_t<int> position, dimension_t<int> size, GUI_MapViewer* map_viewer);
    ~GUI_MiniMap();

    GUI_MapViewer* m_map_viewer;

    GLint m_canvas;

    dimension_t<float> m_cell_size;

    void render(GraphicalController* graph, int px, int py) override;
    void render_on_canvas() override;
    void on_update();

private:
    bool m_canvas_create;
};

class GUI_FOV : public GUI_Object {
public:
    GUI_FOV(position_t<int> position, dimension_t<int> size, GameObject* object);

    GameObject* m_object;
    FOV* m_fov;
    int m_radius;
    dimension_t<float> m_cell_size;

    GLint m_canvas;
    AI_enemy* m_AI;
    Vision_list* m_vision_list;

    void render(GraphicalController* graph, int px, int py) override;
    void render_on_canvas() override;
    void on_update();

private:
    bool m_canvas_create;
};

#endif //GUI_MINIMAP_H
