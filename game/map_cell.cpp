#include "map_cell.h"

MapCell::MapCell()
{
    m_kind = entity_e::cell;
    m_notable = false;
    m_owner = nullptr;
    m_map = nullptr;
}

MapCell::MapCell(int x, int y, int z, GameMap* map)
    : m_map(map)
    , x(x)
    , y(y)
    , z(z)
{
    m_kind = entity_e::cell;
    m_notable = false;
    m_owner = nullptr;
}

void MapCell::add_object(GameObject* object)
{
    m_items.push_back(object);
}

void MapCell::reset_serialization_index()
{
    m_serialization_index = 0;
    for (auto obj : m_items) {
        if (obj->m_serialization_index != 0) {
            obj->reset_serialization_index();
        }
    }
}

iPacker& MapCell::get_packer()
{
    return Packer<MapCell>::instance();
}

IJSONSERIALIZABLE_IMPL(MapCell);