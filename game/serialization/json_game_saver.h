#ifndef JSON_GAME_SAVER_H
#define JSON_GAME_SAVER_H

#include "writer.h"
#include <string>
#include "member_map.h"

class Game_object_owner;

class JsonGameSaver : public JsonWriter {
public:
	
    using JsonWriter::write;

    template <typename T>
    constexpr std::u16string
    write_object(T& object)
    {
        std::u16string result;
        auto ref = objects.find(&object);
        if (ref == objects.end()) {
            result = u"{";
        } else
            result = u"{\"$link\":" + cp1251_to_utf16(std::to_string(ref->second)) + u",";
        static_for<object_properties<T>>([&](auto i) {
            constexpr auto item = std::get<i>(object_properties<T>());
            if (i != 0) {
                result += u",";
            }
            if constexpr (std::is_base_of_v<iCustomHandler, decltype(item)>) {
                result += u"\"" + std::u16string { item.property.name } + u"\":" + (*this.*(item.custom_function))(object.*(item.property.member));
            } else
                result += u"\"" + std::u16string { item.name } + u"\":" + write(object.*(item.member));
        });
        return result + u'}';
    }

    std::u16string write(iSerializable& value) override;
    std::u16string write(iSerializable* value) override;
    //std::u16string write(const iSerializable& value) override;
    //std::u16string write(const iSerializable* value) override;
	std::u16string write(Object_interaction& value) override;
    std::u16string write(Object_interaction* value) override;
    std::u16string write(Action& value) override;
    std::u16string write(Action* value) override;
	std::u16string write(Game_object_owner& value) override;
	std::u16string write(Game_object_owner* value) override;
    std::u16string write(GameWorld& value) override;
    std::u16string write(GameWorld* value) override;
    std::u16string write(GameMap& value) override;
    std::u16string write(GameMap* value) override;
    std::u16string write(GameObject& value) override;
    std::u16string write(GameObject* value) override;
    std::u16string write(Attribute_map& value) override;
    std::u16string write(Attribute_map* value) override;
    std::u16string write(Object_state& value) override;
    std::u16string write(Object_state* value) override;
    std::u16string write(Interaction_list& value) override;
    std::u16string write(Interaction_list* value) override;
    std::u16string write(MapCell& value) override;
    std::u16string write(MapCell* value) override;
    std::u16string write(InventoryCell& value) override;
    std::u16string write(InventoryCell* value) override;
    std::u16string write(ObjectPart& value) override;
    std::u16string write(ObjectPart* value) override;
    std::u16string write(AI& value) override;
    std::u16string write(AI* value) override;
    std::u16string write(AI_enemy& value) override;
    std::u16string write(AI_enemy* value) override;
    std::u16string write(predicate_t& value) override;
    std::u16string write(predicate_t* value) override;
    std::u16string write(TileManager& value) override;
    std::u16string write(TileManager* value) override;
    std::u16string write(Icon& value) override;
    std::u16string write(Icon* value) override;
    std::u16string write(Effect& value) override;
    std::u16string write(Effect* value) override;
    std::u16string write(Instruction_arg_extract& value) override;
    std::u16string write(Instruction_arg_extract* value) override;
    std::u16string write(Instruction_check_owner_type& value) override;
    std::u16string write(Instruction_check_owner_type* value) override;
    std::u16string write(Instruction_check_part_type& value) override;
    std::u16string write(Instruction_check_part_type* value) override;
    std::u16string write(Instruction_result& value) override;
    std::u16string write(Instruction_result* value) override;
    std::u16string write(Parameter_list& value) override;
    std::u16string write(Parameter_list* value) override;
	
    std::u16string write(Instruction_check_tag& value) override;
    std::u16string write(Instruction_check_tag* value) override;
    std::u16string write(Instruction_game_owner& value) override;
    std::u16string write(Instruction_game_owner* value) override;
    std::u16string write(Instruction_get_owner& value) override;
    std::u16string write(Instruction_get_owner* value) override;
    std::u16string write(Instruction_slot_link& value) override;
    std::u16string write(Instruction_slot_link* value) override;
    std::u16string write(Interaction_addon& value) override;
    std::u16string write(Interaction_addon* value) override;
    std::u16string write(Interaction_copyist& value) override;
    std::u16string write(Interaction_copyist* value) override;
    std::u16string write(Interaction_prefix& value) override;
    std::u16string write(Interaction_prefix* value) override;
    std::u16string write(Interaction_slot& value) override;
    std::u16string write(Interaction_slot* value) override;
    std::u16string write(ObjectTag::Activator& value) override;
    std::u16string write(ObjectTag::Activator* value) override;
    std::u16string write(ObjectTag::Label& value) override;
    std::u16string write(ObjectTag::Label* value) override;
    std::u16string write(ObjectTag::Equippable& value) override;
    std::u16string write(ObjectTag::Equippable* value) override;
    std::u16string write(ObjectTag::Fast_move& value) override;
    std::u16string write(ObjectTag::Fast_move* value) override;
    std::u16string write(ObjectTag::Mortal& value) override;
    std::u16string write(ObjectTag::Mortal* value) override;
    std::u16string write(ObjectTag::Purification_from_poison& value) override;
    std::u16string write(ObjectTag::Purification_from_poison* value) override;
    std::u16string write(ObjectTag::Requirements_to_object& value) override;
    std::u16string write(ObjectTag::Requirements_to_object* value) override;
    std::u16string write(Object_tag& value) override;
    std::u16string write(Object_tag* value) override;
    std::u16string write(Interaction_time& value) override;
    std::u16string write(Interaction_time* value) override;
    std::u16string write(Slot_set_state& value) override;
    std::u16string write(Slot_set_state* value) override;
    std::u16string write(Parameter& value) override;
    std::u16string write(Parameter* value) override;
    std::u16string write(Config& value) override;
    std::u16string write(Config* value) override;
    std::u16string write(ObjectTag::Can_transfer_object& value) override;
    std::u16string write(ObjectTag::Can_transfer_object* value) override;
    std::u16string write(ObjectTag::Poison_resist& value) override;
    std::u16string write(ObjectTag::Poison_resist* value) override;
	
    std::u16string map_cell_owner_save(GameMap* value);

	template <typename T>
    constexpr static auto object_properties();


};

#endif
