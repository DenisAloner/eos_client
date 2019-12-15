#ifndef OBJECT_STATE_H
#define OBJECT_STATE_H

#include "attribute_map.h"
#include "TileManager.h"
#include "AI.h"

class AI;

class Object_state : public Attribute_map {
public:
    object_state_e m_state;
    int m_layer;
    dimension3_t m_size;
    Icon* m_icon;
    float m_weight;
    rgb_t<int>* m_light;
    optical_properties_t* m_optical;

    float* m_visibility;

    TileManager* m_tile_manager;
    dimension_t<int> m_tile_size;

    AI* m_ai;

    //virtual Action* find_action(action_e kind);
    virtual void set_tile_size();
    Object_state();
    Object_state* clone() override;
    void apply_visitor(Visitor_generic& visitor) override;

    void reset_serialization_index() override;

    iPacker& get_packer() override;

    std::u16string icon_to_json(Icon*& value, SerializationContext& context);
    std::string icon_to_binary(Icon*& value, SerializationContext& context);
    void icon_from_json(const std::u16string& value, Icon*& prop, SerializationContext& context);
    void icon_from_binary(const std::string& value, Icon*& prop, std::size_t& pos, SerializationContext& context);

    std::u16string tilemanager_to_json(TileManager*& value, SerializationContext& context);
    std::string tilemanager_to_binary(TileManager*& value, SerializationContext& context);
    void tilemanager_from_json(const std::u16string& value, TileManager*& prop, SerializationContext& context);
    void tilemanager_from_binary(const std::string& value, TileManager*& prop, std::size_t& pos, SerializationContext& context);

    constexpr static auto properties()
    {
        return make_union(
            Attribute_map::properties(),
            std::make_tuple(
                make_property(&Object_state::m_state, u"state"),
                make_property(&Object_state::m_size, u"size"),
                make_property(&Object_state::m_layer, u"layer"),
                make_property(&Object_state::m_ai, u"AI"),
                make_property(&Object_state::m_tile_manager, u"tile_manager", &Object_state::tilemanager_to_json,
                    &Object_state::tilemanager_from_json, &Object_state::tilemanager_to_binary,
                    &Object_state::tilemanager_from_binary),
                make_property(&Object_state::m_icon, u"icon", &Object_state::icon_to_json, &Object_state::icon_from_json,
                    &Object_state::icon_to_binary, &Object_state::icon_from_binary),
                make_property(&Object_state::m_light, u"light"),
                make_property(&Object_state::m_optical, u"optical")));
    }

    IJSONSERIALIZABLE();
};


#endif