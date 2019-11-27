#ifndef I_JSON_SERIALIZABLE_H
#define I_JSON_SERIALIZABLE_H

#include <string>

class JsonWriter;

class iJsonSerializable {
public:
    virtual ~iJsonSerializable() = default;
    virtual std::u16string serialize_to_json_reference(JsonWriter& value) = 0;
    virtual std::u16string serialize_to_json_pointer(JsonWriter& value) = 0;
};

#endif
