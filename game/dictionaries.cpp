#include "dictionaries.h"

object_direction_e operator+(object_direction_e lhs,
    const rotate_direction_e& rhs)
{
    auto result = static_cast<int>(lhs) + static_cast<int>(rhs);
    switch (result) {
    case -1:
        return object_direction_e::downright;
    case 8:
        return object_direction_e::down;
    default:
        return static_cast<object_direction_e>(result);
    }
}

Dictionary<object_tag_e> Dictionaries::m_json_object_tag = {
    { object_tag_e::poison_resist, "poison_resist" },
    { object_tag_e::purification_from_poison, "purification_from_poison" },
    { object_tag_e::mortal, "mortal" },
    { object_tag_e::pick_able, "pick_able" },
    { object_tag_e::pass_able, "pass_able" },
    { object_tag_e::seethrough_able, "seethrough_able" },
    { object_tag_e::activator, "activator" },
    { object_tag_e::fast_move, "fast_move" },
    { object_tag_e::equippable, "equippable" },
    { object_tag_e::ring, "ring" },
    { object_tag_e::requirements_to_object, "requirements_to_object" },
    { object_tag_e::cursed, "cursed" },
    { object_tag_e::can_transfer_object, "can_transfer_object" },
    { object_tag_e::footwear, "footwear" }
};

Dictionary<body_part_e> Dictionaries::m_json_body_part_e = {
    { body_part_e::mouth, "mouth" }, { body_part_e::wrist, "wrist" },
    { body_part_e::finger, "finger" }, { body_part_e::head, "head" },
    { body_part_e::hand, "hand" }, { body_part_e::foot, "foot" },
    { body_part_e::waist, "waist" }, { body_part_e::container, "container" }
};

Dictionary<ai_type_e> Dictionaries::m_json_ai_type_e = {
    { ai_type_e::non_humanoid, "non_humanoid" }, { ai_type_e::trap, "trap" }
};

Dictionary<interaction_e> Dictionaries::m_json_interaction_e = {
    { interaction_e::total_damage, "total_damage" },
    { interaction_e::damage, "damage" },
    { interaction_e::buff, "buff" },
    { interaction_e::use, "use" },
    { interaction_e::health, "health" },
    { interaction_e::strength, "strength" },
    { interaction_e::intelligence, "intelligence" },
    { interaction_e::dexterity, "dexterity" },
    { interaction_e::hunger, "hunger" },
    { interaction_e::thirst, "thirst" },
    { interaction_e::poison, "poison" },
    { interaction_e::action, "action" },
    { interaction_e::tag, "tag" },
    { interaction_e::body, "body" },
    { interaction_e::weapon_damage, "weapon_damage" },
    { interaction_e::skill_sword, "skill_sword" },
    { interaction_e::skill_bow, "skill_bow" },
    { interaction_e::strength_bonus, "strength_bonus" },
    { interaction_e::demand_weapon_skill, "demand_weapon_skill" },
    { interaction_e::evasion_skill, "evasion_skill" },
    { interaction_e::weapon_range, "weapon_range" },
    { interaction_e::vision, "vision" },
    { interaction_e::vision_component, "vision_component" },
    { interaction_e::skill_unarmed_combat, "skill_unarmed_combat" },
    { interaction_e::equip, "equip" }
};

Dictionary<feature_list_type_e> Dictionaries::m_json_feature_list_type_e = {
    { feature_list_type_e::generic, "generic" },
    { feature_list_type_e::parameter, "parameter" },
    { feature_list_type_e::tag, "tag" },
    {
        feature_list_type_e::action,
        "action",
    },
    { feature_list_type_e::parts, "parts" },
    { feature_list_type_e::vision, "vision" },
    { feature_list_type_e::vision_component, "vision_component" }
};

Dictionary<entity_e> Dictionaries::m_json_entity_e = {
    { entity_e::game_object, "gameobject" },
    { entity_e::body_part, "object_part" },
    { entity_e::cell, "cell" }
};

Dictionary<action_e::type> Dictionaries::m_json_action_e = {
    { action_e::move, "move" },
    { action_e::move_step, "move_step" },
    { action_e::push, "push" },
    { action_e::turn, "turn" },
    { action_e::open_inventory, "open_inventory" },
    { action_e::cell_info, "cell_info" },
    { action_e::set_motion_path, "set_motion_path" },
    { action_e::pick, "pick" },
    { action_e::open, "open" },
    { action_e::hit, "hit" },
    { action_e::hit_melee, "hit_melee" },
    { action_e::equip, "equip" },
    { action_e::show_parameters, "how_parameters" },
    { action_e::use, "use" },
    { action_e::save, "save" },
    { action_e::autoexplore, "autoexplore" },
    { action_e::shoot, "shoot" },
    { action_e::load, "load" },
    { action_e::move_out, "move_out" }
};

Dictionary<object_state_e> Dictionaries::m_json_object_state_e = {
    { object_state_e::alive, "alive" },
    { object_state_e::dead, "dead" },
    { object_state_e::on, "on" },
    { object_state_e::off, "off" },
    { object_state_e::equip, "equip" }
};

Dictionary<effect_e> Dictionaries::m_json_effect_e = {
    { effect_e::value, "value" },
    { effect_e::limit, "limit" },
    { effect_e::start_angle, "start_angle" },
    { effect_e::end_angle, "end_angle" }
};

Dictionary<object_direction_e> Dictionaries::m_json_object_direction_e = {
    { object_direction_e::down, "down" },
    { object_direction_e::downleft, "downleft" },
    { object_direction_e::left, "left" },
    { object_direction_e::topleft, "topleft" },
    { object_direction_e::top, "top" },
    { object_direction_e::topright, "topright" },
    { object_direction_e::right, "right" },
    { object_direction_e::downright, "downright" },
};

std::unordered_map<effect_e, std::u16string> Dictionaries::m_string_effect_e = {
    { effect_e::value, u"модификатор значения" },
    { effect_e::limit, u"модификатор лимита" },
    { effect_e::start_angle, u"модификатор начального угла обзора" },
    { effect_e::end_angle, u"модификатор конечного угла обзора" }
};

std::unordered_map<interaction_e, std::u16string>
    Dictionaries::m_string_interaction_e = {
        { interaction_e::total_damage, u"общий дополнительный урон" },
        { interaction_e::damage, u"урон" },
        { interaction_e::buff, u"баффы" },
        { interaction_e::use, u"применение" },
        { interaction_e::health, u"здоровье" },
        { interaction_e::strength, u"сила" },
        { interaction_e::intelligence, u"интеллект" },
        { interaction_e::dexterity, u"ловкость" },
        { interaction_e::hunger, u"голод" },
        { interaction_e::thirst, u"жажда" },
        { interaction_e::poison, u"яд" },
        { interaction_e::action, u"действия" },
        { interaction_e::tag, u"метки" },
        { interaction_e::body, u"тело" },
        { interaction_e::weapon_damage, u"урон оружия" },
        { interaction_e::skill_sword, u"владение мечом" },
        { interaction_e::skill_bow, u"владение луком" },
        { interaction_e::strength_bonus, u"бонус силы" },
        { interaction_e::demand_weapon_skill, u"требование к владению оружием" },
        { interaction_e::evasion_skill, u"навык уклонения" },
        { interaction_e::weapon_range, u"дальность" },
        { interaction_e::vision, u"зрение" },
        { interaction_e::vision_component, u"поле зрения" },
        { interaction_e::skill_unarmed_combat, u"владение безоружным боем" },
        { interaction_e::equip, u"экипировка" }
    };

std::unordered_map<object_tag_e, std::u16string>
    Dictionaries::m_string_object_tag_e = {
        { object_tag_e::poison_resist, u"сопротивление к яду" },
        { object_tag_e::purification_from_poison, u"очищение от яда" },
        { object_tag_e::mortal, u"смертное существо" },
        { object_tag_e::pass_able, u"не является преградой" },
        { object_tag_e::pick_able, u"можно взять" },
        { object_tag_e::seethrough_able, u"не загораживает обзор" },
        { object_tag_e::activator, u"активирует/деактивирует механизмы" },
        { object_tag_e::fast_move, u"быстрое передвижение" },
        { object_tag_e::equippable, u"можно одеть" },
        { object_tag_e::ring, u"кольцо" },
        { object_tag_e::requirements_to_object, u"требования к предмету" },
        { object_tag_e::cursed, u"наложено проклятье" },
        { object_tag_e::can_transfer_object, u"может перекладывать предметы" },
        { object_tag_e::footwear, u"обувь" }
    };