#ifndef I_JSON_DESERIALIZABLE_H
#define I_JSON_DESERIALIZABLE_H

#include <string>

class JsonReader;

#define IJSONDESERIALIZABLE()                                                                         \
    void deserialize_to_json_reference(JsonReader& value, const std::u16string_view& json) override; \
    void deserialize_to_json_pointer(JsonReader& value, const std::u16string_view& json) override;

#define IJSONDESERIALIZABLE_IMPL(Class)                                                                                               \
    void Class::deserialize_to_json_reference(JsonReader& value, const std::u16string_view& json) { return value.read(*this, json); } \
    void Class::deserialize_to_json_pointer(JsonReader& value, const std::u16string_view& json) { return value.read(this, json); }

class iJsonDeserializable {
public:
    virtual ~iJsonDeserializable() = default;
    virtual void deserialize_to_json_reference(JsonReader& value, const std::u16string_view& json) = 0;
    virtual void deserialize_to_json_pointer(JsonReader& value, const std::u16string_view& json) = 0;
};

#endif
