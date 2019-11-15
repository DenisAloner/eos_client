#ifndef I_JSON_SERIALIZABLE_H
#define I_JSON_SERIALIZABLE_H

#include <string>

class JsonWriter;

class iJsonSerializable {
public:
    virtual ~iJsonSerializable() = default;
    virtual std::u16string serialize_to_json(JsonWriter& value) = 0;
};

#endif
