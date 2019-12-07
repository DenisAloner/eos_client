#ifndef I_JSON_SERIALIZABLE_H
#define I_JSON_SERIALIZABLE_H

#include <string>

class JsonWriter;

#define IJSONSERIALIZABLE()                                                  \
    std::u16string serialize_to_json_reference(JsonWriter& value) override; \
    std::u16string serialize_to_json_pointer(JsonWriter& value) override;

#define IJSONSERIALIZABLE_IMPL(Class)                                                                         \
    std::u16string Class::serialize_to_json_reference(JsonWriter& value) { return value.write(*this); } \
    std::u16string Class::serialize_to_json_pointer(JsonWriter& value) { return value.write(this); }

class iJsonSerializable {
public:
    virtual ~iJsonSerializable() = default;
    virtual std::u16string serialize_to_json_reference(JsonWriter& value) = 0;
    virtual std::u16string serialize_to_json_pointer(JsonWriter& value) = 0;
};

#endif
