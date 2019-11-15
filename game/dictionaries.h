#ifndef DICTIONARIES_H
#define DICTIONARIES_H

#include <unordered_map>

enum class interaction_e {
    total_damage,
    damage,
    buff,
    use,
    health,
    strength,
    intelligence,
    dexterity,
    hunger,
    thirst,
    demand_weapon_skill,
    evasion_skill,
    tag,
    poison,
    action,
    body,
    skill_sword,
    skill_bow,
    weapon_damage,
    strength_bonus,
    weapon_range,
    vision,
    vision_component,
    skill_unarmed_combat,
    equip
};

enum class object_tag_e {
    none,
    poison_resist,
    purification_from_poison,
    mortal,
    pass_able,
    pick_able,
    seethrough_able,
    activator,
    fast_move,
    equippable,
    ring,
    requirements_to_object,
    cursed,
    can_transfer_object,
    footwear
};

enum class body_part_e {
    // eye,
    mouth,
    // ear,
    // nose,
    // teeth,
    // tongue,
    wrist,
    finger,
    head,
    hand,
    foot,
    waist,
    container
};

enum class ai_type_e {
    trap,
    non_humanoid
};

enum class feature_list_type_e {
    generic,
    parameter,
    tag,
    action,
    parts,
    vision,
    vision_component
};

enum class entity_e {
    cell,
    inventory_cell,
    body_part,
    game_object
};

class action_e {
public:
    enum type {
        none = -1,
        move,
        move_step,
        push,
        turn,
        open_inventory,
        cell_info,
        set_motion_path,
        pick,
        open,
        hit,
        hit_melee,
        equip,
        show_parameters,
        use,
        save,
        autoexplore,
        shoot,
        load,
        move_out,
        rotate_view,
        change_z_level,
        max
    };
};

enum class object_state_e {
    alive,
    dead,
    on,
    off,
    equip
};

enum class effect_e {
    value,
    limit,
    start_angle,
    end_angle
};

enum class object_direction_e : int {
    down,
    downleft,
    left,
    topleft,
    top,
    topright,
    right,
    downright,
    none
};

enum class rotate_direction_e : int {
    counterclockwise = -1,
    clockwise = 1

};

object_direction_e
operator+(object_direction_e lhs, const rotate_direction_e& rhs);

template <typename T>
class Dictionary {
public:
    std::unordered_map<std::string, T> m_by_string;
    std::unordered_map<T, std::string> m_by_enum;

    Dictionary(std::initializer_list<std::tuple<T, std::string>> l)
    {
        for (auto element : l) {
            add(std::get<0>(element), std::get<1>(element));
        }
    }

    void add(T enm, std::string json_string)
    {
        m_by_string[json_string] = enm;
        m_by_enum[enm] = json_string;
    }

    void add(T enm, std::string json_string, std::string description_string)
    {
        m_by_string[json_string] = enm;
        m_by_enum[enm] = json_string;
    }

    std::string get_string(T key)
    {
        auto value = m_by_enum.find(key);
        if (value == m_by_enum.end()) {
            Logger::instance().critical("Элемент отсутствует");
        }
        return value->second;
    }

    T get_enum(const std::string& key)
    {
        auto value = m_by_string.find(key);
        if (value == m_by_string.end()) {
            Logger::instance().critical("Элемент отсутствует");
        }
        return value->second;
    }
};

struct Dictionaries {
    static Dictionary<interaction_e> m_json_interaction_e;
    static Dictionary<object_tag_e> m_json_object_tag;
    static Dictionary<body_part_e> m_json_body_part_e;
    static Dictionary<ai_type_e> m_json_ai_type_e;
    static Dictionary<feature_list_type_e> m_json_feature_list_type_e;
    static Dictionary<entity_e> m_json_entity_e;
    static Dictionary<action_e::type> m_json_action_e;
    static Dictionary<object_state_e> m_json_object_state_e;
    static Dictionary<effect_e> m_json_effect_e;
    static Dictionary<object_direction_e> m_json_object_direction_e;

    static std::unordered_map<effect_e, std::u16string> m_string_effect_e;
    static std::unordered_map<interaction_e, std::u16string>
        m_string_interaction_e;
    static std::unordered_map<object_tag_e, std::u16string> m_string_object_tag_e;

    template <typename T>
    constexpr static Dictionary<T>& get_dictionary()
    {
        if constexpr (std::is_same<T, object_tag_e>::value)
            return m_json_object_tag;
        else if constexpr (std::is_same<T, interaction_e>::value)
            return m_json_interaction_e;
        else if constexpr (std::is_same<T, body_part_e>::value)
            return m_json_body_part_e;
        else if constexpr (std::is_same<T, feature_list_type_e>::value)
            return m_json_feature_list_type_e;
        else if constexpr (std::is_same<T, entity_e>::value)
            return m_json_entity_e;
        else if constexpr (std::is_same<T, ai_type_e>::value)
            return m_json_ai_type_e;
        else if constexpr (std::is_same<T, action_e::type>::value)
            return m_json_action_e;
        else if constexpr (std::is_same<T, object_state_e>::value)
            return m_json_object_state_e;
        else if constexpr (std::is_same<T, effect_e>::value)
            return m_json_effect_e;
        else if constexpr (std::is_same<T, object_direction_e>::value)
            return m_json_object_direction_e;
        else
            static_assert(always_false<T>::value, "<get_dictionary> type error");
    }
};

#endif