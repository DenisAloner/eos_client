#ifndef JSON_GAME_SAVER_H
#define JSON_GAME_SAVER_H

#include "member_map.h"
#include "writer.h"
#include <string>

class JsonGameSaver : public JsonWriter {
public:
	
    using JsonWriter::visit;

    template <typename T>
    constexpr void
    write_object(T& object)
    {
        std::u16string result;
        auto ref = objects.find(&object);
        visit(MemberMap::type<T>);
        if (ref == objects.end()) {
            result = u"{\"$type\":" + m_result + u",";
        } else
            result = u"{\"$type\":" + m_result + u",\"$link\":" + cp1251_to_utf16(std::to_string(ref->second)) + u",";
        static_for<object_properties<T>>([&](auto i) {
            constexpr auto item = std::get<i>(object_properties<T>());
            if (i != 0) {
                result += u",";
            }
            using property_type = decltype(item);
            if constexpr (std::is_base_of_v<iCustomHandler, property_type>) {
                (*this.*(item.custom_function))(object.*(item.property.member));
                result += u"\"" + std::u16string { item.property.name } + u"\":" + m_result;
            } else if constexpr (std::is_pointer_v<typename property_type::type>) {
                result += u"\"" + std::u16string { item.name } + u"\":";
                if constexpr (std::is_base_of_v<iJsonSerializable, std::remove_pointer_t<typename property_type::type>>) {
                    auto value = object.*(item.member);
                    if (value) {
                        value->accept_pointer(*this);
                        result += m_result;
                    } else
                        result += u"null";
                } else {
                    visit(object.*(item.member));
                    result += m_result;
                }
            } else if constexpr (std::is_reference_v<typename property_type::type>) {
                result += u"\"" + std::u16string { item.name } + u"\":";
                if constexpr (std::is_base_of_v<iJsonSerializable, std::remove_reference_t<typename property_type::type>>) {
                    auto value = object.*(item.member);
                    value.accept_reference(*this);
                    result += m_result;
                } else {
                    visit(object.*(item.member));
                    result += m_result;
                }
            } else {
                visit(object.*(item.member));
                result += u"\"" + std::u16string { item.name } + u"\":" + m_result;
            }
        });
        m_result = result + u'}';
    }

    void visit(Action& value) override;
    void visit(Action* value) override;
    void visit(GameWorld& value) override;
    void visit(GameWorld* value) override;
    void visit(GameMap& value) override;
    void visit(GameMap* value) override;
    void visit(GameObject& value) override;
    void visit(GameObject* value) override;
    void visit(Attribute_map& value) override;
    void visit(Attribute_map* value) override;
    void visit(Object_state& value) override;
    void visit(Object_state* value) override;
    void visit(Interaction_list& value) override;
    void visit(Interaction_list* value) override;
    void visit(MapCell& value) override;
    void visit(MapCell* value) override;
    void visit(InventoryCell& value) override;
    void visit(InventoryCell* value) override;
    void visit(ObjectPart& value) override;
    void visit(ObjectPart* value) override;
    void visit(AI_enemy& value) override;
    void visit(AI_enemy* value) override;
    void visit(AI_trap& value) override;
    void visit(AI_trap* value) override;
    void visit(predicate_t& value) override;
    void visit(predicate_t* value) override;
    void visit(TileManager& value) override;
    void visit(TileManager* value) override;
    void visit(Icon& value) override;
    void visit(Icon* value) override;
    void visit(Effect& value) override;
    void visit(Effect* value) override;
    void visit(Instruction_arg_extract& value) override;
    void visit(Instruction_arg_extract* value) override;
    void visit(Instruction_check_owner_type& value) override;
    void visit(Instruction_check_owner_type* value) override;
    void visit(Instruction_check_part_type& value) override;
    void visit(Instruction_check_part_type* value) override;
    void visit(Instruction_result& value) override;
    void visit(Instruction_result* value) override;
    void visit(Parameter_list& value) override;
    void visit(Parameter_list* value) override;
    void visit(Action_list& value) override;
    void visit(Action_list* value) override;
    void visit(Instruction_check_tag& value) override;
    void visit(Instruction_check_tag* value) override;
    void visit(Instruction_game_owner& value) override;
    void visit(Instruction_game_owner* value) override;
    void visit(Instruction_get_owner& value) override;
    void visit(Instruction_get_owner* value) override;
    void visit(Instruction_slot_link& value) override;
    void visit(Instruction_slot_link* value) override;
    void visit(Interaction_addon& value) override;
    void visit(Interaction_addon* value) override;
    void visit(Interaction_copyist& value) override;
    void visit(Interaction_copyist* value) override;
    void visit(Interaction_prefix& value) override;
    void visit(Interaction_prefix* value) override;
    void visit(ObjectTag::Activator& value) override;
    void visit(ObjectTag::Activator* value) override;
    void visit(ObjectTag::Label& value) override;
    void visit(ObjectTag::Label* value) override;
    void visit(ObjectTag::Equippable& value) override;
    void visit(ObjectTag::Equippable* value) override;
    void visit(ObjectTag::Fast_move& value) override;
    void visit(ObjectTag::Fast_move* value) override;
    void visit(ObjectTag::Mortal& value) override;
    void visit(ObjectTag::Mortal* value) override;
    void visit(ObjectTag::Purification_from_poison& value) override;
    void visit(ObjectTag::Purification_from_poison* value) override;
    void visit(ObjectTag::Requirements_to_object& value) override;
    void visit(ObjectTag::Requirements_to_object* value) override;
    void visit(Interaction_time& value) override;
    void visit(Interaction_time* value) override;
    void visit(Slot_set_state& value) override;
    void visit(Slot_set_state* value) override;
    void visit(Parameter& value) override;
    void visit(Parameter* value) override;
    void visit(Config& value) override;
    void visit(Config* value) override;
    void visit(ObjectTag::Can_transfer_object& value) override;
    void visit(ObjectTag::Can_transfer_object* value) override;
    void visit(ObjectTag::Poison_resist& value) override;
    void visit(ObjectTag::Poison_resist* value) override;
    void visit(Interaction_timer& value) override;
    void visit(Interaction_timer* value) override;
    void visit(Instruction_slot_parameter& value) override;
    void visit(Instruction_slot_parameter* value) override;
    void visit(GraphicalController& value) override;
    void visit(GraphicalController* value) override;
    void visit(Action_wrapper& value) override;
    void visit(Action_wrapper* value) override;

    void map_cell_owner_save(GameMap* value);
    void active_object_state_save(Object_state* value);

    template <typename T>
    constexpr static auto object_properties();
};

#endif
