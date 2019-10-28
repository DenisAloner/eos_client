#ifndef PARAMETER_H
#define PARAMETER_H

#include "Definiton.h"

class Game_object_owner;
class GameObject;
class MapCell;
class Object_part;

struct Parameter_argument_t {
    type_e kind;

    union {
        GameObject* m_object;
        MapCell* m_cell;
        Game_object_owner* m_owner;
        Object_part* m_part;
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
    };
};

//class Parameter_MoveObjectByUnit : public Parameter
//{
//public:
//	GameObject* m_unit;
//	GameObject* m_object;
//	MapCell* m_place;
//	GameMap* m_map;
//Packer_generic& get_packer()
//	Parameter_MoveObjectByUnit(void);
//	~Parameter_MoveObjectByUnit(void);
//
//	virtual void reset_serialization_index();
//	virtual void save();
//	virtual void load();
//};

class Instruction_result : public Object_interaction {
public:
    Parameter_argument_t m_value;
    bool m_result;

    Instruction_result();
    Instruction_result* clone() override { return nullptr; };
    void apply_effect(GameObject* unit, Object_interaction* object) override {};

    iPacker& get_packer() override;
    interaction_message_type_e get_interaction_message_type() override;
};

#endif //PARAMETER_H
