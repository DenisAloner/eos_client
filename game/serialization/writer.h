#ifndef WRITER_H
#define WRITER_H

//#include "all_effects.h"
#include "game_world.h"
#include "i_json_serializable.h"
#include "json_helpers.h"
#include "object_part.h"
#include "object_state.h"

#define WRITE(Type)                                  \
    virtual std::u16string write(Type& value) = 0; \
    virtual std::u16string write(Type* value) = 0

class iJsonSerializable;
class Object_state;
class AI;
class AI_enemy;
class AI_trap;
class Config;
class Instruction_arg_extract;
class Instruction_check_owner_type;
class Instruction_check_part_type;
class Instruction_game_owner;
class Instruction_game_owner;

class JsonWriter {
public:
    unsigned long counter = 0;
    std::unordered_map<iSerializable*, long> objects;

    virtual std::u16string write(int& value);
    virtual std::u16string write(int* value);
    virtual std::u16string write(const int& value);
    virtual std::u16string write(const int* value);
    virtual std::u16string write(std::u16string& value);
    virtual std::u16string write(std::u16string* value);
    virtual std::u16string write(const std::u16string& value);
    virtual std::u16string write(const std::u16string* value);
    virtual std::u16string write(dimension3_t& value);
    virtual std::u16string write(dimension3_t* value);
    virtual std::u16string write(optical_properties_t& value);
    virtual std::u16string write(optical_properties_t* value);

    WRITE(Action);
    WRITE(GameWorld);
    WRITE(GameMap);
    WRITE(GameObject);
    WRITE(Attribute_map);
    WRITE(Object_state);
    WRITE(Interaction_list);
    WRITE(Parameter_list);
    WRITE(Action_list);
    WRITE(MapCell);
    WRITE(InventoryCell);
    WRITE(ObjectPart);
    WRITE(AI_enemy);
    WRITE(AI_trap);
    WRITE(predicate_t);
    WRITE(TileManager);
    WRITE(Icon);
    WRITE(Effect);
    WRITE(Instruction_arg_extract);
    WRITE(Instruction_check_owner_type);
    WRITE(Instruction_check_part_type);
    WRITE(Instruction_result);
    WRITE(Instruction_check_tag);
    WRITE(Instruction_game_owner);
    WRITE(Instruction_get_owner);
    WRITE(Instruction_slot_link);
    WRITE(Instruction_slot_parameter);
    WRITE(Interaction_addon);
    WRITE(Interaction_copyist);
    WRITE(Interaction_prefix);
    WRITE(ObjectTag::Activator);
    WRITE(ObjectTag::Label);
    WRITE(ObjectTag::Equippable);
    WRITE(ObjectTag::Fast_move);
    WRITE(ObjectTag::Mortal);
    WRITE(ObjectTag::Purification_from_poison);
    WRITE(ObjectTag::Requirements_to_object);
    WRITE(ObjectTag::Can_transfer_object);
    WRITE(ObjectTag::Poison_resist);
    WRITE(Interaction_time);
    WRITE(Interaction_timer);
    WRITE(Slot_set_state);
    WRITE(Parameter);
    WRITE(Config);

    template <typename T, class = typename std::enable_if<std::is_enum<T>::value, T>::type>
    std::u16string write(const T& value)
    {
        return u"\"" + cp1251_to_utf16(Dictionaries::get_dictionary<T>().get_string(value)) + u"\"";
    }

    template <typename T>
    std::u16string write(std::vector<T>& ref)
    {
        if (ref.empty())
            return u"[]";
        std::u16string result = u"[";
        if constexpr (std::is_pointer_v<T> && std::is_base_of_v<iJsonSerializable, std::remove_pointer_t<T>>) {
            for (auto& current : ref) {
                result += (current ? current->serialize_to_json_pointer(*this) : u"null") + u",";
            }
        } else
            for (auto& current : ref) {
                result += write(current) + u",";
            }
        result[result.length() - 1] = u']';
        return result;
    }

    template <typename T>
    std::u16string write(std::list<T>& ref)
    {
        if (ref.empty())
            return u"[]";
        std::u16string result = u"[";
        if constexpr (std::is_pointer_v<T> && std::is_base_of_v<iJsonSerializable, std::remove_pointer_t<T>>) {
            for (auto& current : ref) {
                result += (current ? current->serialize_to_json_pointer(*this) : u"null") + u",";
            }
        } else
            for (auto& current : ref) {
                result += write(current) + u",";
            }
        result[result.length() - 1] = u']';
        return result;
    }

    template <typename Key, typename Value>
    std::u16string write(std::unordered_map<Key, Value>& ref)
    {
        if (ref.empty())
            return u"[]";
        std::u16string result = u"[";
        if constexpr (std::is_pointer_v<Value> && std::is_base_of_v<iJsonSerializable, std::remove_pointer_t<Value>>) {
            for (auto& current : ref) {
                result += write(current.first) + u"," + (current.second ? current.second->serialize_to_json_pointer(*this) : u"null") + u",";
            }
        } else
            for (auto& current : ref) {
                result += write(current.first) + u"," + write(current.second) + u",";
            }
        result[result.length() - 1] = u']';
        return result;
    }

    template <typename Key, typename Value>
    std::u16string write(std::map<Key, Value>& ref)
    {
        if (ref.empty())
            return u"[]";
        std::u16string result = u"[";
        if constexpr (std::is_pointer_v<Value> && std::is_base_of_v<iJsonSerializable, std::remove_pointer_t<Value>>) {
            for (auto& current : ref) {
                result += write(current.first) + u"," + (current.second ? current.second->serialize_to_json_pointer(*this) : u"null") + u",";
            }
        } else
            for (auto& current : ref) {
                result += write(current.first) + u"," + write(current.second) + u",";
            }
        result[result.length() - 1] = u']';
        return result;
    }

    template <typename T>
    std::u16string write(rgb_t<T>& ref)
    {
        return u"[" + write(ref.r) + u"," + write(ref.g) + u"," + write(ref.b) + u"]";
    }

    template <typename T>
    std::u16string write(rgb_t<T>* ref)
    {
        return ref ? write(*ref) : u"null";
    }

    template <typename T>
    std::u16string write_pointer(T* value)
    {
        if (!value)
            return u"null";
        auto ref = objects.find(value);
        if (ref == objects.end()) {
            counter += 1;
            objects[value] = counter;
            return write(*value);
        }
        return u"{\"$type\":\"link\",\"$link\":" + cp1251_to_utf16(std::to_string(ref->second)) + u"}";
    }
};

#endif //WRITER_H