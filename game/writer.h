#ifndef WRITER_H
#define WRITER_H

#include "GameObject.h"
#include "i_json_serializable.h"

class iJsonSerializable;

template <typename Class, typename T, typename W>
struct JsonProperty {

    typedef std::u16string (W::*custom_function_t)(T&);

    constexpr JsonProperty(T Class::*member,
        const char16_t* name,
        custom_function_t custom_function)
        : member { member }
        , name { name }
        , custom_function { custom_function } {};

	 constexpr JsonProperty(T Class::*member,
        const char16_t* name)
        : member { member }
        , name { name } {};

    T Class::*member;
    const char16_t* name;
    std::optional<custom_function_t> custom_function;
};

template <typename Class, typename T, typename W>
constexpr auto
json_property(T Class::*member, const char16_t* name, std::u16string (W::*fn)(T&))
{
    return JsonProperty<Class, T, W> { member, name, fn };
}

template <typename Class, typename T>
constexpr auto
json_property(T Class::*member, const char16_t* name)
{
    return JsonProperty<Class, T, Class> { member, name };
}

template <typename T, T... S, typename F>
constexpr void
for_sequence_json(std::integer_sequence<T, S...>, F&& f)
{
    using unpack_t = int[];
    (void)unpack_t { (static_cast<void>(f(std::integral_constant<T, S> {})), 0)...,
        0 };
}

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

    virtual std::u16string write(Game_world& value) = 0;
    virtual std::u16string write(GameMap& value) = 0;
    virtual std::u16string write(GameObject& value) = 0;
    virtual std::u16string write(Attribute_map& value) = 0;
    virtual std::u16string write(Object_state& value) = 0;
    virtual std::u16string write(Interaction_list& value) = 0;
    virtual std::u16string write(MapCell& value) = 0;
    virtual std::u16string write(Inventory_cell& value) = 0;
    virtual std::u16string write(Object_part& value) = 0;

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
    std::u16string write(T* value)
    {
        if (!value)
            return u"null";
        auto ref = objects.find(value);
        if (ref == objects.end()) {
            counter += 1;
            objects[value] = counter;
            return value->serialize_to_json(*this);
        }
        return u"{\"$type\":link,\"$link\":" + cp1251_to_utf16(std::to_string(ref->second)) + u"}";
    }
};

#endif //WRITER_H