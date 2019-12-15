#ifndef TYPE_INFO_H
#define TYPE_INFO_H

#include <cstddef>

enum class type_id_e : std::size_t {
    GameWorld = 1,
    GameMap = 2,
    GameObject = 3,
    size = 3
};

#endif