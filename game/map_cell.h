#ifndef MAP_CELL_H
#define MAP_CELL_H

#include "GameMap.h"
#include "game_object_owner.h"
#include "writer.h"

class MapCell : public Game_object_owner {
public:
    GameMap* m_map;

    bool m_closed;
    int m_state;

    bool m_notable;
    bool m_mark;

    int x;
    int y;
    int z;

    cell_t m_pathfind_info;

    std::list<GameObject*> m_items;

    unsigned int m_path_info;

    rgb_t<int> m_light;
    rgb_t<int> m_light_blur;

    MapCell();
    MapCell(int x, int y, int z, GameMap* map);

    void add_object(GameObject* object);
    //virtual Object_feature* find_property(property_e kind, GameObject* excluded);

    void reset_serialization_index() override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&MapCell::x, u"x"),
            make_property(&MapCell::y, u"y"),
            make_property(&MapCell::m_items, u"items"));
    }

    IVISITABLE();
};

#endif
