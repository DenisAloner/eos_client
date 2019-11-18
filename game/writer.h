#ifndef WRITER_H
#define WRITER_H

#include "game_world.h"
#include "i_json_serializable.h"
#include "object_part.h"
#include "object_state.h"

class iJsonSerializable;
class Object_state;

class JsonWriter {
public:
    unsigned long counter = 0;
    std::unordered_map<iSerializable*, long> objects;

    virtual std::u16string write(int& value);
    virtual std::u16string write(const int& value);
    virtual std::u16string write(std::u16string& value);
    virtual std::u16string write(const std::u16string& value);
    virtual std::u16string write(dimension3_t& value);
    virtual std::u16string write(const dimension3_t& value);

    virtual std::u16string write(GameWorld& value) = 0;
    virtual std::u16string write(GameMap& value) = 0;
    virtual std::u16string write(GameObject& value) = 0;
    virtual std::u16string write(Attribute_map& value) = 0;
    virtual std::u16string write(Object_state& value) = 0;
    virtual std::u16string write(Interaction_list& value) = 0;
    virtual std::u16string write(MapCell& value) = 0;
    virtual std::u16string write(InventoryCell& value) = 0;
    virtual std::u16string write(ObjectPart& value) = 0;

    virtual std::u16string write(Game_object_owner* value) = 0;
    virtual std::u16string write(GameWorld* value) = 0;
    virtual std::u16string write(GameMap* value) = 0;
    virtual std::u16string write(GameObject* value) = 0;
    virtual std::u16string write(Attribute_map* value) = 0;
    virtual std::u16string write(Object_state* value) = 0;
    virtual std::u16string write(Interaction_list* value) = 0;
    virtual std::u16string write(MapCell* value) = 0;
    virtual std::u16string write(InventoryCell* value) = 0;
    virtual std::u16string write(ObjectPart* value) = 0;

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