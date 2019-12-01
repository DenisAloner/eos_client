#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "AI.h"
#include "Event.h"
#include "FOV_light.h"
#include <functional>
#include <list>
#include "map_cell.h"

//class GameObject;
//class Object_feature;
//class MapCell;
//class AI;
class Dijkstra_map;
//class Player;

const int max_light_radius = 20;

class GameMap : public iSerializable {
public:
    Event<VoidEventArgs> update;

    std::size_t m_index{};

    Dijkstra_map* m_dijkstra_map;

    struct block_t {
        rectangle_t<int> rect;
        block_t(int x, int y, int w, int h)
            : rect(rectangle_t<int>(x, y, w, h))
        {
        }
    };

    dimension3_t m_size;

    std::vector<MapCell> m_items;

    std::list<block_t*> m_rooms;
    std::list<block_t*> m_link_rooms;

    explicit GameMap(dimension3_t size);
    GameMap();

    MapCell& get(int z, int y, int x);

    void init(dimension3_t size);
    void add_object(GameObject* object, MapCell& cell);
    void remove_object(GameObject* object);
    void move_object(GameObject* object, MapCell& cell);
    void turn_object(GameObject* object);

    void generate_room();
    bool divide_block(block_t* Block, int depth, int current);
    void generate_level(void);

    void add_to_map(GameObject* object, MapCell& cell);

    void fill();
    void random_block(block_t* block);
    void connect_room();
    void link_room(block_t* a, block_t* b);
    void add_wall();
    void add_doors();
    void generate_traps();
    //bool line2(int x1, int y1, int x2, int y2);
    void add_lighting();
    void bresenham_line(MapCell* a, MapCell* b, const std::function<void(MapCell*)>& fn);
    MapCell* bresenham_line2(MapCell* a, MapCell* b, std::function<bool(MapCell*)> f);
    MapCell* bresenham_line2(MapCell* a, MapCell* b, Parameter* p, std::function<bool(Parameter*, MapCell*)> f);
    //bool check(int x,int y);
    //MapCell* get_cell(int x, int y);

    void blur_lighting();

    void reset_serialization_index() override;

    iPacker& get_packer() override;

    std::u16string vector_mapcell_to_json(std::vector<MapCell>& value, SerializationContext& context);
    void vector_mapcell_from_json(const std::u16string& value, std::vector<MapCell>& prop, SerializationContext& context);
    std::string vector_mapcell_to_binary(std::vector<MapCell>& value, SerializationContext& context);
    void vector_mapcell_from_binary(const std::string& value, std::vector<MapCell>& prop, std::size_t& pos, SerializationContext& context);

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&GameMap::m_size, u"size"),
            make_property(&GameMap::m_items, u"item", &GameMap::vector_mapcell_to_json, &GameMap::vector_mapcell_from_json, &GameMap::vector_mapcell_to_binary, &GameMap::vector_mapcell_from_binary));
    }

    std::u16string serialize_to_json_reference(JsonWriter& value) override;
    std::u16string serialize_to_json_pointer(JsonWriter& value) override;
};

#endif