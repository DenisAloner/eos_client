#ifndef WRITER_H
#define WRITER_H

//#include "all_effects.h"
#include "game_world.h"
#include "i_json_serializable.h"
#include "json_helpers.h"
#include "object_part.h"
#include "object_state.h"

#define WRITE(Type)                             \
    virtual std::u16string write(##Type& value) = 0; \
    virtual std::u16string write(##Type* value) = 0

class iJsonSerializable;
class Object_state;
class AI;
class AI_enemy;
class Config;
class Instruction_arg_extract;
class Instruction_check_owner_type;
class Instruction_check_part_type;
class Instruction_game_owner;
class Instruction_game_owner;

//template <typename Type, typename... Rest>
//class IVisitor : public IVisitor<Type>, public IVisitor<Rest...> {
//public:
//    //using IVisitor<Type>::write;
//    //using Visitor<Rest...>::write;
//};
//
//template <typename Type>
//class IVisitor<Type> {
//public:
//    virtual std::u16string write(Type& value) = 0;
//    virtual std::u16string write(Type* value) = 0;
//};
//
//class IJsonWriter : IVisitor<int,
//                        const int,
//                        std::u16string,
//                        const std::u16string,
//                        dimension3_t,
//                        optical_properties_t,
//                        Object_interaction,
//                        Action,
//                        Game_object_owner,
//                        GameWorld,
//                        GameMap,
//                        GameObject,
//                        Attribute_map,
//                        Object_state,
//                        Interaction_list,
//                        MapCell,
//                        InventoryCell,
//                        ObjectPart,
//                        AI,
//                        AI_enemy,
//                        predicate_t,
//                        TileManager,
//                        Icon,
//                        Effect,
//                        Instruction_arg_extract,
//                        Instruction_check_owner_type,
//                        Instruction_check_part_type,
//                        Instruction_result,
//                        Instruction_check_tag,
//                        Instruction_game_owner,
//                        Instruction_get_owner,
//                        Instruction_slot_link,
//                        Interaction_addon,
//                        Interaction_copyist,
//                        Interaction_prefix,
//                        Interaction_slot,
//                        ObjectTag::Activator,
//                        ObjectTag::Equippable,
//                        ObjectTag::Fast_move,
//                        ObjectTag::Mortal,
//                        ObjectTag::Purification_from_poison,
//                        ObjectTag::Requirements_to_object> {
//};

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

	WRITE(iSerializable);
    WRITE(Object_interaction);
    WRITE(Action);
    WRITE(Game_object_owner);
    WRITE(GameWorld);
    WRITE(GameMap);
    WRITE(GameObject);
    WRITE(Attribute_map);
    WRITE(Object_state);
    WRITE(Interaction_list);
    WRITE(Parameter_list);
    WRITE(MapCell);
    WRITE(InventoryCell);
    WRITE(ObjectPart);
    WRITE(AI);
    WRITE(AI_enemy);
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
    WRITE(Interaction_addon);
    WRITE(Interaction_copyist);
    WRITE(Interaction_prefix);
    WRITE(Interaction_slot);
    WRITE(ObjectTag::Activator);
    WRITE(ObjectTag::Label);
    WRITE(ObjectTag::Equippable);
    WRITE(ObjectTag::Fast_move);
    WRITE(ObjectTag::Mortal);
    WRITE(ObjectTag::Purification_from_poison);
    WRITE(ObjectTag::Requirements_to_object);
    WRITE(ObjectTag::Can_transfer_object);
    WRITE(ObjectTag::Poison_resist);
    WRITE(Object_tag);
    WRITE(Interaction_time);
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
            return value->serialize_to_json_reference(*this);
        }
        return u"{\"$type\":link,\"$link\":" + cp1251_to_utf16(std::to_string(ref->second)) + u"}";
    }
};

#endif //WRITER_H