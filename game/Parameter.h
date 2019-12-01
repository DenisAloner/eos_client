#ifndef PARAMETER_H
#define PARAMETER_H

#include "Definiton.h"

class Game_object_owner;
class GameObject;
class MapCell;
class ObjectPart;

struct Parameter_argument_t {
    type_e kind;

    union {
        GameObject* m_object;
        MapCell* m_cell;
        Game_object_owner* m_owner;
        ObjectPart* m_part;
        object_direction_e m_direction;
    };

    Parameter_argument_t() {};

    Parameter_argument_t(type_e kind)
        : kind(kind) {};

    void set(GameObject* value);
    void set(Game_object_owner* value);
    void set(object_direction_e value);

    void init();
    void reset_serialization_index();

    operator bool();
};

class Parameter : public iSerializable {
public:
    parameter_type_e m_kind;
    Parameter_argument_t* m_args;
    size_t m_size;

    Parameter() {};

    Parameter(parameter_type_e kind);

    template <typename... T>
    Parameter(const parameter_type_e kind, T... args)
        : Parameter(kind)
    {
        set_helper(0, args...);
    }

    Parameter_argument_t& operator[](const std::size_t i)
    {
        return m_args[i];
    }

    const Parameter_argument_t& operator[](const std::size_t i) const
    {
        return m_args[i];
    }

    Parameter* clone();

    void reset_serialization_index() override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Parameter::m_size, u"size"));
    }

	std::u16string serialize_to_json_reference(JsonWriter& value) override;
    std::u16string serialize_to_json_pointer(JsonWriter& value) override;

private:
    template <typename T, typename... Args>
    void set_helper(const int i, T value)
    {
        m_args[i].set(value);
    };

    template <typename T, typename... Args>
    void set_helper(const int i, T value, Args... a)
    {
        m_args[i].set(value);
        if (i < m_size) {
            set_helper(i + 1, a...);
        }
    }
};

#endif //PARAMETER_H
