#ifndef READER_H
#define READER_H

#include "Definiton.h"
#include "gui_style.h"
#include <pattern_visitor.h>
#include <string>

typedef std::vector<std::u16string_view> json_vector_t;
typedef std::unordered_map<std::u16string_view, std::u16string_view> json_map_t;

int json_to_int(const std::u16string_view& value);
float json_to_float(const std::u16string_view& value);
std::u16string_view json_token(const std::u16string_view& value, std::size_t& i);
json_vector_t* parse_json_array(const std::u16string_view& value);
json_vector_t* read_json_array(const std::u16string_view& value);
json_map_t* parse_json_object(const std::u16string_view& value);
json_map_t* read_json_object(const std::u16string_view& value);

class JsonReader : public iVisitor {
public:
    template <typename Key, typename Value>
    void read(const std::u16string_view& json, std::unordered_map<Key, Value>& ref)
    {
        const auto array = read_json_array(json);
        if (array) {
            for (std::size_t index = 0; index < array->size(); index += 2) {
                Key key;
                read((*array)[index], key);
                Value& value = ref[key];
                read((*array)[index + 1], value);
            }
            delete array;
        }
    }

	virtual void read(const std::u16string_view& json, int& ref);
    virtual void read(const std::u16string_view& json, float& ref);
    virtual void read(const std::u16string_view& json, atlas_tile_t& ref);
    virtual void read(const std::u16string_view& json, std::u16string& ref);
    virtual void read(const std::u16string_view& json, gui_style_t& ref) = 0;
};

#endif //READER_H