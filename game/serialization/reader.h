#ifndef READER_H
#define READER_H

#include "Definiton.h"
#include "gui_style.h"
#include "i_json_deserializable.h"
#include <string>

class GraphicalController;
typedef std::vector<std::u16string_view> json_vector_t;
typedef std::unordered_map<std::u16string_view, std::u16string_view> json_map_t;

int json_to_int(const std::u16string_view& value);
float json_to_float(const std::u16string_view& value);
std::u16string_view json_token(const std::u16string_view& value, std::size_t& i);
json_vector_t* parse_json_array(const std::u16string_view& value);
json_vector_t* read_json_array(const std::u16string_view& value);
json_map_t* parse_json_object(const std::u16string_view& value);
json_map_t* read_json_object(const std::u16string_view& value);

class JsonReader {
public:
	
    template <typename Key, typename Value>
    void read(std::unordered_map<Key, Value>& ref, const std::u16string_view& json)
    {
        const auto array = read_json_array(json);
        if (array) {
            if constexpr (std::is_pointer_v<Value> && std::is_base_of_v<iJsonSerializable, std::remove_pointer_t<Value>>) {
                for (auto& current : ref) {
                    for (std::size_t index = 0; index < array->size(); index += 2) {
                        Key key;
                        read(key, (*array)[index]);
                        Value& value = ref[key];
                        value->serialize_to_json_pointer(*this, (*array)[index + 1]);
                    }
                }
            } else
                for (std::size_t index = 0; index < array->size(); index += 2) {
                    Key key;
                    read(key, (*array)[index]);
                    Value& value = ref[key];
                    read(value, (*array)[index + 1]);
                }
            delete array;
        }
    }

    virtual void read(int& ref, const std::u16string_view& json);
    virtual void read(int*& ref, const std::u16string_view& json);
    virtual void read(float& ref, const std::u16string_view& json);
    virtual void read(std::u16string& ref, const std::u16string_view& json);
    virtual void read(atlas_tile_t& ref, const std::u16string_view& json);

	
    virtual void read(gui_style_t& ref, const std::u16string_view& json) = 0;
    virtual void read(GraphicalController& ref, const std::u16string_view& json) = 0;
    virtual void read(GraphicalController* ref, const std::u16string_view& json) = 0;
};

#endif //READER_H