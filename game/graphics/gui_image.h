#ifndef GUI_IMAGE_H
#define GUI_IMAGE_H

#include "GUI_Object.h"

class GuiImage : public GUI_Object {

public:
    GLuint m_texture;
    GuiImage(int x, int y, int width, int height, GLuint texture);
    void render(GraphicalController* graph, int px, int py) override;
};


#endif //GUI_IMAGE_H
