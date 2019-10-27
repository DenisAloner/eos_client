#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H


#include "Definiton.h"
#include "Visitors.h"

class Game_object_owner;
class Config;
class Object_part;

class GameObjectManager {
public:
    static Config* m_config;

    std::map<std::string, effect_prefix_e> m_to_effect_prefix_e;
    std::map<std::string, feature_list_type_e> m_to_feature_list_type_e;

    std::map<effect_prefix_e, std::string> m_effect_prefix_string;

    effect_prefix_e get_effect_prefix_e(const std::string& key);

    void init(SerializationContext& context);
    GameObject* new_object(std::string unit_name);
    //void register_object(GameObject*& object);

    std::string get_effect_prefix_string(effect_prefix_e key);

    void bind_body(GameObject* object);
};

class Visitor_part_hierarchy_setter : public Visitor_simple {
public:
    std::list<Game_object_owner*> m_game_object_owner_stack;

    void visit(Object_part& value) override;
};

#endif //GAME_OBJECT_MANAGER_H
