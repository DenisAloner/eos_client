#ifndef READER_H
#define READER_H

#include <pattern_visitor.h>
#include <string>
#include "Definiton.h"
typedef std::vector<std::u16string_view> json_vector_t;

float json_to_float(const std::u16string& value);
std::u16string_view json_token(const std::u16string_view& value, std::size_t& i);
json_vector_t* json_pair(const std::u16string_view& value);
json_vector_t* read_json_pair(const std::u16string_view& value);

class JsonReader : public iVisitor {
public:
    virtual void read(const std::u16string_view& json, float& ref);
    virtual void read(const std::u16string_view& json, atlas_tile_t& ref);
    virtual void read(const std::u16string_view& json, std::u16string& ref);
};

#endif //READER_H
