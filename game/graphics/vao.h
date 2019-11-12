#ifndef VAO_H
#define VAO_H

#include <glbinding/gl/types.h>

using namespace gl;

struct vertex_t {
    GLint position[2] {};
    GLfloat texture_coordinates[3] {};
    GLfloat light[3] {};
};

const auto position_offset = reinterpret_cast<const void*>(offsetof(vertex_t, position));
const auto texture_coordinates_offset = reinterpret_cast<const void*>(offsetof(vertex_t, texture_coordinates));
const auto light_offset = reinterpret_cast<const void*>(offsetof(vertex_t, light));

struct gui_vertex_t {
    GLfloat position[2] {};
    GLfloat texture[3] {};
    GLfloat color[4] {};
    GLfloat clip[4] {};
};

const auto gui_position_offset = reinterpret_cast<void*>(offsetof(gui_vertex_t, position));
const auto gui_texture_offset = reinterpret_cast<void*>(offsetof(gui_vertex_t, texture));
const auto gui_color_offset = reinterpret_cast<void*>(offsetof(gui_vertex_t, color));
const auto gui_clip_offset = reinterpret_cast<void*>(offsetof(gui_vertex_t, clip));

template <typename T>
struct vao_quad_t {
    T vertex[4];
};

#endif //VAO_H
