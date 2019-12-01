#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Action.h"
#include "all_effects.h"
#include "game_object_owner.h"
#include <Application.h>
#include <i_json_serializable.h>
#include <list>
#include <map>
#include "object_state.h"

class Object_feature;
class Application;
class TileManager;
class Effect;
class Object_interaction;
class Interaction_list;
class Parameter_list;
class Vision_list;
class Vision_component;
class Tag_list;
struct object_parameter_t;
class Action;
class AI;
class Object_tag;
class Icon;
class GameMap;
class ObjectPart;
class MapCell;
class InventoryCell;
class Object_state;

class Tag_getter : public Visitor {
public:
    object_tag_e m_key;
    Object_tag* m_result;

    Tag_getter(object_tag_e key);
    void visit(Object_interaction& value) override;
};


class GameObject : public Object_interaction, public GUI_connectable_i, public Game_object_owner {
public:
    std::u16string m_name;
    object_direction_e m_direction;
    bool m_selected;

    Object_state* m_active_state;
    std::list<Object_state*> m_state;

    GameObject();

    void turn();
    void set_direction(object_direction_e dir);
    void set_state(object_state_e state);
    Object_state* get_state(object_state_e state);
    void add_effect(interaction_e key, Object_interaction* item);
    void add_from(interaction_e key, Object_interaction* item);
    Interaction_list* get_effect(interaction_e key);
    void remove_effect(interaction_e key, Object_interaction* item);
    bool get_stat(object_tag_e key);
    Object_tag* get_tag(object_tag_e key);
    Parameter_list* get_parameter(interaction_e key);
    Parts_list* get_parts_list(interaction_e key);
    MapCell* cell();
    void update_interaction();
    void add_from(interaction_e key, Interaction_list* feature);
    void remove_from(interaction_e key, Interaction_list* feature);
    bool is_own(MapCell* cell);
    bool is_own(int x, int y, int z);
    MapCell* get_center(MapCell* c);

    Object_interaction* clone() override;
    virtual void do_predicate(predicate func);
    void apply_visitor(Visitor_generic& visitor) override;

    void reset_serialization_index() override;

    void get_actions_list(std::list<Action_helper_t>& value) override;

    // Для поддержки iJSONSerializable
    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&GameObject::m_name, u"name"),
            make_property(&GameObject::m_direction, u"direction"),
            make_property(&GameObject::m_owner, u"owner"),
            make_property(&GameObject::m_state, u"state"),
            make_property(&GameObject::m_active_state, u"active_state"));
    }

    interaction_message_type_e get_interaction_message_type() override;
    std::u16string serialize_to_json_reference(JsonWriter& value) override;
    std::u16string serialize_to_json_pointer(JsonWriter& value) override;

private:
    class Action_getter : public Visitor {
    public:
        std::list<Action_helper_t>& m_list;
        GameObject* m_object;

        Action_getter(GameObject* object, std::list<Action_helper_t>& list);
        void visit(Object_interaction& value) override;
    };
};

class Config : public iSerializable {
public:
    std::map<std::string, Object_interaction*> m_templates;
    std::map<std::string, GameObject> m_items;
    InstanceDictionary<TileManager*> m_tile_managers;
    InstanceDictionary<TileManager*> m_tile_managers_atlas;
    InstanceDictionary<Icon*> m_icons;

    iPacker& get_packer() override;

    void instancedictonary_icon_from_json(const std::u16string& value, InstanceDictionary<Icon*>& prop, SerializationContext& context);
    void instancedictonary_icon_from_binary(const std::string& value, InstanceDictionary<Icon*>& prop, std::size_t& pos, SerializationContext& context);

    void instancedictonary_tilemanager_from_json_atlas(const std::u16string& value, InstanceDictionary<TileManager*>& prop, SerializationContext& context);

    void instancedictonary_tilemanager_from_json(const std::u16string& value, InstanceDictionary<TileManager*>& prop, SerializationContext& context);
    void instancedictonary_tilemanager_from_binary(const std::string& value, InstanceDictionary<TileManager*>& prop, std::size_t& pos, SerializationContext& context);

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Config::m_icons, u"icons").from_json(&Config::instancedictonary_icon_from_json).from_binary(&Config::instancedictonary_icon_from_binary),
            make_property(&Config::m_tile_managers_atlas, u"atlases").from_json(&Config::instancedictonary_tilemanager_from_json_atlas),
            make_property(&Config::m_tile_managers, u"tile_managers")
                .from_json(&Config::instancedictonary_tilemanager_from_json)
                .from_binary(
                    &Config::instancedictonary_tilemanager_from_binary),
            make_property(&Config::m_templates, u"templates"),
            make_property(&Config::m_items, u"items"));
    }

    std::u16string serialize_to_json_reference(JsonWriter& value) override;
    std::u16string serialize_to_json_pointer(JsonWriter& value) override;
};

class Player : public GUI_connectable_i {
public:
    std::list<Action*> m_actions;
    GameObject* m_object;
    GameMap* m_map;
    Player(GameObject* object, GameMap* map);

    void get_actions_list(std::list<Action_helper_t>& value) override;
};

#endif
