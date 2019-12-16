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
                if constexpr (std::is_base_of_v<IVisitable, std::remove_pointer_t<typename property_type::type>>) {
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
                if constexpr (std::is_base_of_v<IVisitable, std::remove_reference_t<typename property_type::type>>) {
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

	VISIT_OVERLOAD(Action);
    VISIT_OVERLOAD(GameWorld);
    VISIT_OVERLOAD(GameMap);
    VISIT_OVERLOAD(GameObject);
    VISIT_OVERLOAD(AttributeMap);
    VISIT_OVERLOAD(Object_state);
    VISIT_OVERLOAD(Interaction_list);
    VISIT_OVERLOAD(MapCell);
    VISIT_OVERLOAD(InventoryCell);
    VISIT_OVERLOAD(ObjectPart);
    VISIT_OVERLOAD(AI_enemy);
    VISIT_OVERLOAD(AI_trap);
    VISIT_OVERLOAD(predicate_t);
    VISIT_OVERLOAD(TileManager);
    VISIT_OVERLOAD(Icon);
    VISIT_OVERLOAD(Effect);
    VISIT_OVERLOAD(Instruction_arg_extract);
    VISIT_OVERLOAD(Instruction_check_owner_type);
    VISIT_OVERLOAD(Instruction_check_part_type);
    VISIT_OVERLOAD(Instruction_result);
    VISIT_OVERLOAD(Parameter_list);
    VISIT_OVERLOAD(Action_list);
    VISIT_OVERLOAD(Instruction_check_tag);
    VISIT_OVERLOAD(Instruction_game_owner);
    VISIT_OVERLOAD(Instruction_get_owner);
    VISIT_OVERLOAD(Instruction_slot_link);
    VISIT_OVERLOAD(Interaction_addon);
    VISIT_OVERLOAD(Interaction_copyist);
    VISIT_OVERLOAD(Interaction_prefix);
    VISIT_OVERLOAD(ObjectTag::Activator);
    VISIT_OVERLOAD(ObjectTag::Label);
    VISIT_OVERLOAD(ObjectTag::Equippable);
    VISIT_OVERLOAD(ObjectTag::Fast_move);
    VISIT_OVERLOAD(ObjectTag::Mortal);
    VISIT_OVERLOAD(ObjectTag::Purification_from_poison);
    VISIT_OVERLOAD(ObjectTag::Requirements_to_object);
    VISIT_OVERLOAD(ObjectTag::Can_transfer_object);
    VISIT_OVERLOAD(ObjectTag::Poison_resist);
    VISIT_OVERLOAD(Interaction_time);
    VISIT_OVERLOAD(Interaction_timer);
    VISIT_OVERLOAD(Slot_set_state);
    VISIT_OVERLOAD(Parameter);
    VISIT_OVERLOAD(Config);
    VISIT_OVERLOAD(Instruction_slot_parameter);
    VISIT_OVERLOAD(GraphicalController);
    VISIT_OVERLOAD(Action_wrapper);

    void map_cell_owner_save(GameMap* value);
    void active_object_state_save(Object_state* value);

    template <typename T>
    constexpr static auto object_properties();
};

#endif