#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "GameMap.h"

class Player;
class GameMap;

class GameWorld : public iSerializable, public iJsonSerializable {
public:
    float m_coefficient[21][21][21];

    std::list<GameMap*> m_maps;
    std::list<GameObject> m_objects;

    Player* m_player;

    GameWorld();
    void calculate_lighting();
    void update_buff();
    void calculate_ai();
    void reset_serialization_index() override;

    std::u16string serialize(SerializationContext& context);
    void deserialize(std::u16string& value, SerializationContext& context);

    std::string bin_serialize(SerializationContext& context);
    void bin_deserialize(std::string& value, SerializationContext& context);

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&GameWorld::m_objects, u"objects"),
            make_property(&GameWorld::m_maps, u"maps")

        );
    }

    std::u16string serialize_to_json_reference(JsonWriter& value) override;
    std::u16string serialize_to_json_pointer(JsonWriter& value) override;
};

#endif
