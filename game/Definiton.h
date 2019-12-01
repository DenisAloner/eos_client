#ifndef DEFINITION_H
#define DEFINITION_H

#include "game/utils/Logger.h"
#include "graphics/gl.hpp"
#include <experimental/string>
#include <functional>
#include <list>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include "dictionaries.h"
#include "i_json_serializable.h"

using namespace std::string_literals;
using namespace gl;

const float Pi = 3.14159265F;
const float cos22_5 = cos(22.5F * Pi / 180.0F);
const float cos67_5 = cos(67.5F * Pi / 180.0F);

class GameObject;
class GUI_Object;
class Action;
class Parameter;

const int tile_size_x = 38;
const int tile_size_y = 22;
const int tile_size_x_half = 19;
const int tile_size_y_half = 11;

template <typename T>
struct position_t {
    T x;
    T y;

    position_t(const T x, const T y)
        : x(x)
        , y(y)
    {
    }
    position_t()
        : x(0)
        , y(0)
    {
    }
};

template <typename T>
struct dimension_t {
    T w;
    T h;

    dimension_t(const T w, const T h)
        : w(w)
        , h(h)
    {
    }
    dimension_t()
        : w(0)
        , h(0)
    {
    }
};

struct position3_t {
    int x;
    int y;
    int z;

    position3_t(int x, int y, int z)
        : x(x)
        , y(y)
        , z(z)
    {
    }
    position3_t()
        : x(0)
        , y(0)
        , z(0)
    {
    }
};

struct dimension3_t {
    int dx;
    int dy;
    int dz;

    dimension3_t(int dx, int dy, int dz)
        : dx(dx)
        , dy(dy)
        , dz(dz)
    {
    }
    dimension3_t()
        : dx(0)
        , dy(0)
        , dz(0)
    {
    }
};

struct view_t {
    int l;
    int r;
    int u;
    int d;

    view_t(int l, int r, int u, int d)
        : l(l)
        , r(r)
        , u(u)
        , d(d)
    {
    }
    view_t()
        : l(0)
        , r(0)
        , u(0)
        , d(0)
    {
    }
};

template <typename T>
struct rectangle_t
    : position_t<T>,
      dimension_t<T> {
    rectangle_t(const T x, const T y, const T w, const T h)
        : position_t<T>(x, y)
        , dimension_t<T>(w, h)
    {
    }
    rectangle_t(const T x, const T y, const dimension_t<T>& dimension)
        : position_t<T>(x, y)
        , dimension_t<T>(dimension)
    {
    }
    rectangle_t() = default;

    [[nodiscard]] T right() const { return x + w; }

    [[nodiscard]] T bottom() const { return y + h; }
};

struct area_t {
    position_t<int> p1;
    position_t<int> p2;

    area_t(position_t<int> p1, position_t<int> p2)
        : p1(p1)
        , p2(p2)
    {
    }
};

struct MouseEventArgs {
    position_t<int> position;
    int key;
    int value;

    MouseEventArgs(const position_t<int>& position, int key)
        : position(position)
        , key(key)
        , value(0)
    {
    }
    MouseEventArgs(const position_t<int>& position, int key, int value)
        : position(position)
        , key(key)
        , value(value)
    {
    }
};

enum mouse_key_e {
    mk_none = 0,
    mk_left = 1,
    mk_right = 2,
    mk_middle = 3
};

struct VoidEventArgs {
};

enum class parameter_type_e {
    position,
    mapcell,
    // ParameterKind_MoveObjectByUnit,
    cancel,
    destination,
    object,
    owner,
    unit_interaction,
    interaction_cell,
    accept,
    bow_shoot,
    direction,
    game_quit,
    new_game,
    load_game
};


struct tile_t {
    GLuint unit;
    GLdouble coordinates[4];
};

struct atlas_tile_t {
    unsigned char layer;
    rectangle_t<GLint> texture;
};

struct RGBA_t {
    float r, g, b, a;

    RGBA_t(const float r, const float g, const float b, const float a)
        : r(r)
        , g(g)
        , b(b)
        , a(a) {};

    float& operator[](const std::size_t& index)
    {
        switch (index) {
        case 0:
            return r;
        case 1:
            return g;
        case 2:
            return b;
        case 3:
            return a;
        default:
            throw;
        }
    }
};

template <typename T>
struct rgb_t {
    using type = T;

    T r, g, b;
    rgb_t(const T r, const T g, const T b)
        : r(r)
        , g(g)
        , b(b) {};
    rgb_t()
        : r(0)
        , g(0)
        , b(0) {};
};

typedef std::map<std::u16string, std::u16string> scheme_map_t;
typedef std::list<std::u16string> scheme_list_t;
typedef std::vector<std::u16string> scheme_vector_t;

struct optical_properties_t {
    rgb_t<float> attenuation;
    optical_properties_t(rgb_t<float> attenuation)
        : attenuation(attenuation) {};
    optical_properties_t()
        : attenuation(rgb_t<float>(1.0, 1.0, 1.0)) {};
    // optical_properties_t(const optical_properties_t& value)
    // :seethrough(value.seethrough),attenuation(value.attenuation){};
};

enum class interaction_message_type_e {
    list,
    action,
    part,
    slot_set_state,
    select_location,
    mover,
    allocator,
    interaction_timer,
    tag,
    instruction_slot_parameter,
    instruction_game_owner,
    game_object,
    instruction_result,
    effect,
	action_wrapper,
    instruction_check_tag,
    instruction_check_part_type,
    instruction_arg_extract,
    instruction_get_owner,
    instruction_check_owner_type,
    interaction_copyist,
    interaction_prefix,
    interaction_addon,
    interaction_time,
    instruction_slot_link,
    impact_copy_chance
};





enum class effect_prefix_e {
    physical_damage,
    poison_damage,
    parameter_change,
    state_change,
    action
};


class Object_interaction;

class Visitor {
public:
    virtual void visit(Object_interaction& value) = 0;
};

class GameTask;
class GameObject;

struct tag_t {

    GameTask* task;

    tag_t(GameTask* task)
        : task(task) {};
    tag_t()
        : task(nullptr) {};
};

struct f2dvector_t {
    float x;
    float y;

    f2dvector_t(float x, float y)
        : x(x)
        , y(y)
    {
    }
    f2dvector_t()
        : x(0.0f)
        , y(0.0f)
    {
    }

    void normalize()
    {
        const auto length = sqrtf(x * x + y * y);
        x = x / length;
        y = y / length;
    }
};

enum class type_e {
    null,
    object_part,
    gameobject,
    mapcell,
    inventory_cell,
    object_owner,
    direction
};

struct predicate_t {
    std::function<bool(GameObject*)> predicate;
    size_t index;

    predicate_t()
        : index(0) {};
    predicate_t(std::function<bool(GameObject*)> predicate, const size_t index)
        : predicate(std::move(predicate))
        , index(index) {};
};

struct cell_t {
    int value;
    bool opaque;
};

struct common_action_t {
    bool pick;

    common_action_t() { pick = false; }
};

struct AI_FOV {
    int radius;
    predicate_t* qualifier;
    int start_angle;
    int end_angle;

    AI_FOV(int radius, predicate_t* qualifier, int start_angle, int end_angle)
        : radius(radius)
        , qualifier(qualifier)
        , start_angle(start_angle)
        , end_angle(end_angle) {};
    AI_FOV()
        : radius(0)
        , qualifier(nullptr)
        , start_angle(0)
        , end_angle(0) {};
};

struct Action_helper_t {
    Action* action;
    Parameter* parameter;

    Action_helper_t(Action* action = nullptr, Parameter* parameter = nullptr)
        : action(action)
        , parameter(parameter) {
            /*if ((!action) || (!parameter)) { LOG(FATAL) << "Action_helper_t
         инициализирован нулевым указателем"; }*/
        };
};

struct atlas_symbol_t: atlas_tile_t {
    char16_t value;
    dimension_t<int> size;
    dimension_t<int> bearing;
};

enum game_log_message_type_e {
    message_time,
    message_action_interaction,
    message_battle
};

struct game_log_message_t {
    game_log_message_type_e type;
    std::u16string value;
    game_log_message_t(const game_log_message_type_e type, std::u16string& value)
        : type(type)
        , value(value) {};
};

class GUI_TextFormat {
public:
    GUI_TextFormat(const int width, const int height, const RGBA_t color)
        : m_color(color)
        , m_symbol_size(dimension_t<int>(width, height)) {};
    RGBA_t m_color;
    dimension_t<int> m_symbol_size;
};

enum class mode_t {
    equip,
    unequip,
    check
};

enum class animation_e {
    idle,
    move
};



class InstanceDictionaryElement {
public:
    std::size_t m_index;
    std::string m_json_string;
};

class Icon : public InstanceDictionaryElement {
public:
    GLuint m_value;
};

template <typename T>
class InstanceDictionary {
public:
    std::vector<T> m_by_index;
    std::unordered_map<std::string, T> m_by_string;

    void add(T object, std::string json_string)
    {
        m_by_index.push_back(object);
        object->m_index = m_by_index.size() - 1;
        object->m_json_string = json_string;
        m_by_string[json_string] = object;
    }

    T get_by_string(std::string key)
    {
        auto value = m_by_string.find(key);
        if (value == m_by_string.end()) {
            Logger::instance().critical("Элемент отсутствует");
        }
        return value->second;
    }
};

class iSerializable;
class GameWorld;

class SerializationContext {
public:
    GameWorld& m_game_world;
    std::size_t m_object_index;
    std::vector<iSerializable*> m_items;

    explicit SerializationContext(GameWorld& game_world)
        : m_game_world(game_world)
        , m_object_index(0)
    {
    }

    void reset();
};

template <typename Class, typename T>
struct Property {

    typedef std::u16string (Class::*to_json_function_t)(T&,
        SerializationContext&);
    typedef void (Class::*from_json_function_t)(const std::u16string&,
        T&,
        SerializationContext&);
    typedef std::string (Class::*to_binary_function_t)(T&, SerializationContext&);
    typedef void (Class::*from_binary_function_t)(const std::string&,
        T&,
        std::size_t&,
        SerializationContext&);

    constexpr Property(T Class::*member,
        const char16_t* name,
        to_json_function_t to_json_function,
        from_json_function_t from_json_function,
        to_binary_function_t to_binary_function,
        from_binary_function_t from_binary_function)
        : member { member }
        , name { name }
        , to_json_function { to_json_function }
        , from_json_function { from_json_function }
        , to_binary_function { to_binary_function }
        , from_binary_function { from_binary_function } {};
    constexpr Property(T Class::*member, const char16_t* name)
        : member { member }
        , name { name } {};

    using Type = T;

    T Class::*member;
    const char16_t* name;

    std::optional<to_json_function_t> to_json_function;
    std::optional<from_json_function_t> from_json_function;
    std::optional<to_binary_function_t> to_binary_function;
    std::optional<from_binary_function_t> from_binary_function;

    constexpr Property& to_json(to_json_function_t value)
    {
        to_json_function = value;
        return *this;
    }

    constexpr Property& from_json(from_json_function_t value)
    {
        from_json_function = value;
        return *this;
    }

    constexpr Property& to_binary(to_binary_function_t value)
    {
        to_binary_function = value;
        return *this;
    }

    constexpr Property& from_binary(from_binary_function_t value)
    {
        from_binary_function = value;
        return *this;
    }
};

template <typename Class, typename T>
constexpr auto
make_property(
    T Class::*member,
    const char16_t* name,
    std::u16string (Class::*func1)(T&, SerializationContext&),
    void (Class::*func2)(const std::u16string&, T&, SerializationContext&),
    std::string (Class::*func3)(T&, SerializationContext&),
    void (
        Class::*func4)(const std::string&, T&, std::size_t&, SerializationContext&))
{
    return Property<Class, T> { member, name, func1, func2, func3, func4 };
}

template <typename Class, typename T>
constexpr auto
make_property(T Class::*member, const char16_t* name)
{
    return Property<Class, T> { member, name };
}

template <typename A, typename B>
constexpr auto
make_union(A a, B b)
{
    return std::tuple_cat(a, b);
};

template <typename T, T... S, typename F>
constexpr void
for_sequence(std::integer_sequence<T, S...>, F&& f)
{
    using unpack_t = int[];
    (void)unpack_t { (static_cast<void>(f(std::integral_constant<T, S> {})), 0)...,
        0 };
}

class iPacker {
public:
    virtual void from_json(iSerializable& object,
        scheme_map_t* value,
        SerializationContext& context)
        = 0;
    virtual iSerializable* from_json(scheme_map_t* value,
        SerializationContext& context)
        = 0;
    virtual std::u16string to_json(iSerializable& object,
        SerializationContext& context)
        = 0;
    virtual std::u16string to_json(iSerializable* object,
        SerializationContext& context)
        = 0;
    virtual void from_binary(iSerializable& object,
        const std::string& value,
        std::size_t& pos,
        SerializationContext& context)
        = 0;
    virtual iSerializable* from_binary(const std::string& value,
        std::size_t& pos,
        SerializationContext& context)
        = 0;
    virtual std::string to_binary(iSerializable& object,
        SerializationContext& context)
        = 0;
    virtual std::string to_binary(iSerializable* object,
        SerializationContext& context)
        = 0;
    virtual std::u16string get_type_name() = 0;
    virtual std::size_t get_type_id() = 0;
};

template <typename T>
class Packer;

class Serialization {
public:
    std::map<std::u16string, iPacker*> t_constructors_from_json = {};
    std::vector<iPacker*> t_constructors_from_binary = {};

    static Serialization& instance()
    {
        static Serialization singleton;
        return singleton;
    }

    template <typename T>
    void register_packer(std::u16string class_name)
    {
        // Packer_generic* packer = &Packer<T>::Instance();
        t_constructors_from_json[class_name] = &Packer<T>::instance();
        t_constructors_from_binary.push_back(&Packer<T>::instance());
        Packer<T>::instance().m_type_name = class_name;
        Packer<T>::instance().m_type_id = t_constructors_from_binary.size() - 1;
        // m_type_id_counter += 1;
    }

    ~Serialization();

private:
    Serialization();
    // static std::u16string serialize_object(const iSerializable* value);
};

scheme_map_t*
read_object(std::u16string& value);
scheme_list_t*
read_array(std::u16string& value);
scheme_vector_t*
read_pair(std::u16string& value);
scheme_map_t*
parse_object(std::u16string& value);
scheme_list_t*
parse_array(std::u16string& value);
scheme_vector_t*
parse_pair(std::u16string& value);
std::u16string
get_value(const std::u16string& value);
std::u16string
float_to_u16string(const float& i);
int to_int32(const std::u16string& value);
float to_float(const std::u16string& value);
std::u16string
utf8_to_utf16(const std::u8string& value);
std::string
utf16_to_cp1251(std::u16string const& value);
std::u16string
cp1251_to_utf16(std::string const& value);
std::u16string
int_to_u16string(int const& value);

template <class T>
struct always_false : std::false_type {
};

template <class T>
struct is_map : std::false_type {
};

template <class Key, class Value>
struct is_map<std::map<Key, Value>> : std::true_type {
};

template <class T>
struct is_list : std::false_type {
};

template <class T>
struct is_list<std::list<T>> : std::true_type {
};

template <class T>
struct is_rgb : std::false_type {
};

template <class T>
struct is_rgb<rgb_t<T>> : std::true_type {
};

template <class T>
struct is_instance_dictionary : std::false_type {
};

template <class T>
struct is_instance_dictionary<InstanceDictionary<T>> : std::true_type {
};

template <class T>
struct is_vector : std::false_type {
};

template <class T>
struct is_vector<std::vector<T>> : std::true_type {
};

template <class T>
struct is_pair : std::false_type {
};

template <class Key, class Value>
struct is_pair<std::pair<Key, Value>> : std::true_type {
};

template <typename T>
void parser_from_json(const std::u16string& value,
    T& prop,
    SerializationContext& context)
{
    if constexpr (std::is_pointer<T>::value) {
        if constexpr (std::is_base_of<iSerializable,
                          std::remove_pointer_t<T>>::value) {
            // LOG(INFO) << "from_json<iSerializable*> :: " << typeid(T).name();
            auto temp(value);
            auto s = read_object(temp);
            if (s) {
                prop = dynamic_cast<T>(Serialization::instance()
                                           .t_constructors_from_json[get_value((*s)[u"$type"])]
                                           ->from_json(s, context));
                // LOG(INFO) << UTF16_to_CP1251(prop->get_packer().get_type_name());
                delete s;
                return;
            }
            prop = nullptr;
        } else {
            // LOG(INFO) << "from_json<"<<typeid(T).name()<<">";
            if (get_value(value) == u"null") {
                prop = nullptr;
            } else {
                using Type = std::remove_pointer_t<T>;
                T v = new Type();
                parser_from_json<Type>(value, *v, context);
                prop = v;

                /*using Type = std::remove_pointer_t<T>;
                        T v = new Type();
                        from_json<Type&>(value, *v);
                        prop = v;*/
            }
        }
    } else if constexpr (std::is_enum<T>::value) {
        auto start_pos = value.find(u'"');
        if (start_pos != std::string::npos) {
            auto end_pos = value.find(u'"', start_pos + 1);
            if (end_pos != std::string::npos) {
                prop = Dictionaries::get_dictionary<T>().get_enum(utf16_to_cp1251(
                    value.substr(start_pos + 1, end_pos - start_pos - 1)));
                return;
            }
        }
        prop = static_cast<T>(0);
    } else if constexpr (std::is_base_of<iSerializable, T>::value) {
        // LOG(INFO) << "from_json<iSerializable> :: " << typeid(T).name();
        auto temp(value);
        // LOG(INFO) << UTF16_to_CP1251(temp);
        auto s = read_object(temp);
        prop.from_json(s, context);
        delete s;
    } else if constexpr (is_vector<T>::value) {
        using Value = typename T::value_type;
        auto temp = value;
        auto s = read_array(temp);
        if (s) {
            std::size_t i = 0;
            prop.resize(s->size());
            for (auto element : (*s)) {
                Value v;
                parser_from_json<Value>(element, prop[i], context);
                i += 1;
            }
        }
        delete s;
    } else if constexpr (is_list<T>::value) {
        // LOG(INFO) << "from_json<List>";
        using Value = typename T::value_type;
        auto temp = value;
        auto s = read_array(temp);
        // LOG(INFO) << "LIST : " << UTF16_to_CP1251(temp);
        if (s) {
            for (const auto& element : (*s)) {
                Value& v = prop.emplace_back();
                parser_from_json<Value>(element, v, context);
            }
        }
        delete s;
    } else if constexpr (is_map<T>::value) {
        // LOG(INFO) << "from_json<Map>";
        using Key = typename T::key_type;
        using Value = typename T::mapped_type;
        auto temp = value;
        // LOG(INFO) << UTF16_to_CP1251(temp);
        auto s = read_array(temp);
        if (s) {
            for (auto& element : (*s)) {
                auto p = read_pair(element);
                Key k;
                parser_from_json<Key>((*p)[0], k, context);
                Value& v = prop[k];
                parser_from_json<Value>((*p)[1], v, context);
                delete p;
            }
            delete s;
        }
    } else if constexpr (std::is_same<T, int>::value) {
        prop = to_int32(value);
    } else if constexpr (std::is_same<T, std::u16string>::value) {
        auto start_pos = value.find(u'"');
        if (start_pos != std::string::npos) {
            auto end_pos = value.find(u'"', start_pos + 1);
            if (end_pos != std::string::npos) {
                prop = value.substr(start_pos + 1, end_pos - start_pos - 1);
                return;
            }
        }
        prop = value;
    } else if constexpr (std::is_same<T, std::string>::value) {
        auto start_pos = value.find(u'"');
        if (start_pos != std::string::npos) {
            auto end_pos = value.find(u'"', start_pos + 1);
            if (end_pos != std::string::npos) {
                prop = utf16_to_cp1251(value.substr(start_pos + 1, end_pos - start_pos - 1));
                return;
            }
        }
        prop = utf16_to_cp1251(value);
    } else if constexpr (std::is_same<T, bool>::value) {
        if (get_value(value) == u"true") {
            prop = nullptr;
        } else {
            prop = false;
        }
    } else if constexpr (std::is_same<T, std::size_t>::value) {
        std::size_t result = to_int32(value);
        prop = result;
    } else if constexpr (std::is_same<T, float>::value) {
        auto result = to_float(value);
        prop = result;
    } else if constexpr (std::is_same<T, AI_FOV>::value) {
        auto temp = value;
        auto s = read_pair(temp);
        parser_from_json<int>((*s)[0], prop.radius, context);
        int x;
        parser_from_json<int>((*s)[1], x, context);
        prop.qualifier = Application::instance().m_ai_manager->m_fov_qualifiers[x];
        parser_from_json<int>((*s)[2], prop.start_angle, context);
        parser_from_json<int>((*s)[3], prop.end_angle, context);
    } else if constexpr (std::is_same<T, predicate_t>::value) {
        // LOG(INFO) << Parser::UTF16_to_CP1251(get_value(value)) <<
        // Parser::UTF16_to_CP1251(value);
        auto result = to_int32(value);
        prop = *Application::instance().m_ai_manager->m_path_qualifiers[result];
    } else if constexpr (is_rgb<T>::value) {
        auto temp = value;
        auto s = read_pair(temp);
        using type = typename T::type;
        parser_from_json<type>((*s)[0], prop.r, context);
        parser_from_json<type>((*s)[1], prop.g, context);
        parser_from_json<type>((*s)[2], prop.b, context);
    } else if constexpr (std::is_same<T, optical_properties_t>::value) {
        // LOG(INFO) << Parser::UTF16_to_CP1251(get_value(value)) <<
        // Parser::UTF16_to_CP1251(value);
        auto temp = value;
        auto s = read_pair(temp);
        parser_from_json<float>((*s)[0], prop.attenuation.r, context);
        parser_from_json<float>((*s)[1], prop.attenuation.g, context);
        parser_from_json<float>((*s)[2], prop.attenuation.b, context);
    } else if constexpr (std::is_same<T, dimension_t<int>>::value) {
        auto temp = value;
        auto s = read_pair(temp);
        parser_from_json<int>((*s)[0], prop.w, context);
        parser_from_json<int>((*s)[1], prop.h, context);
    } else if constexpr (std::is_same<T, dimension3_t>::value) {
        auto temp = value;
        auto s = read_pair(temp);
        parser_from_json<int>((*s)[0], prop.dx, context);
        parser_from_json<int>((*s)[1], prop.dy, context);
        parser_from_json<int>((*s)[2], prop.dz, context);
    } else if constexpr (std::is_same<T, rectangle_t<int>>::value) {
        auto temp = value;
        auto s = read_pair(temp);
        parser_from_json<int>((*s)[0], prop.x, context);
        parser_from_json<int>((*s)[1], prop.y, context);
        parser_from_json<int>((*s)[2], prop.w, context);
        parser_from_json<int>((*s)[3], prop.h, context);
    } else if constexpr (std::is_same<T, atlas_tile_t>::value) {
        auto temp = value;
        auto s = read_pair(temp);
        auto& texture = prop.texture;
        parser_from_json<GLfloat>((*s)[0], texture.x, context);
        parser_from_json<GLfloat>((*s)[1], texture.y, context);
        parser_from_json<GLfloat>((*s)[2], texture.w, context);
        parser_from_json<GLfloat>((*s)[3], texture.h, context);
    } else if constexpr (std::is_same<T, Icon>::value || std::is_same<T, TileManager>::value || is_instance_dictionary<T>::value) {
        Logger::instance().critical("<from_json> type error: {}",
            typeid(value).name());
    } else {
        Logger::instance().critical("<from_json> type error: {}",
            typeid(value).name());
        // static_assert(always_false<T>::value, "<from_json> type error");
    }
};

template <typename T>
std::u16string
parser_to_json(T& value, SerializationContext& context)
{
    if constexpr (std::is_pointer<T>::value) {
        if constexpr (std::is_base_of<iSerializable,
                          std::remove_pointer_t<T>>::value) {
            if (value) {
                // LOG(INFO) << "parser_to_json iSerializable*: " <<
                // UTF16_to_CP1251(value->get_packer().get_type_name());
                return value->get_packer().to_json(value, context);
            }
            return u"null";
        } else {
            if (value == nullptr) {
                return u"null";
            }
            using Type = std::remove_pointer_t<T>;
            return parser_to_json<Type>(*value, context);
        }
    } else if constexpr (std::is_base_of<iSerializable, T>::value) {
        // LOG(INFO) << UTF16_to_CP1251(value.get_packer().get_type_name());
        std::u16string result = value.to_json(context);
        if (result.empty()) {
            // разобрать данный момент
            std::u16string out = u"{}";
            return out;
        } else {
            auto out = u"{" + result + u"}";
            return out;
        }
    } else if constexpr (is_vector<T>::value || is_list<T>::value || is_map<T>::value) {
        using Value = typename T::value_type;
        std::u16string result = u"[";
        for (auto element : (value)) {
            if (result != u"[") {
                result += u",";
            }
            result += parser_to_json<Value>(element, context);
        }
        result += u"]";
        return result;
    } else if constexpr (is_pair<T>::value) {
        using Key = std::remove_const_t<typename T::first_type>;
        using Value = typename T::second_type;
        Key key = value.first;
        std::u16string result = u"[" + parser_to_json<Key>(key, context) + u"," + parser_to_json<Value>(value.second, context) + u"]";
        return result;
    } else if constexpr (std::is_enum<T>::value) {
        std::u16string out = u"\"" + cp1251_to_utf16(Dictionaries::get_dictionary<T>().get_string(value)) + u"\"";
        return out;
    } else if constexpr (std::is_same<T, int>::value || std::is_same<T, unsigned int>::value || std::is_same<T, std::size_t>::value) {
        return int_to_u16string(value);
    } else if constexpr (std::is_same<T, std::u16string>::value) {
        return u"\"" + value + u"\"";
    } else if constexpr (std::is_same<T, bool>::value) {
        if (value) {
            return u"true";
        } else {
            return u"false";
        }
    } else if constexpr (std::is_same<T, std::string>::value) {
        return u"\"" + cp1251_to_utf16(value) + u"\"";
    } else if constexpr (std::is_same<T, predicate_t>::value) {
        std::u16string out = int_to_u16string(value.index);
        return out;
    } else if constexpr (std::is_same<T, std::size_t>::value) {
        std::u16string out = int_to_u16string(value);
        return out;
    } else if constexpr (is_rgb<T>::value) {
        using type = typename T::type;
        std::u16string result = u"[" + parser_to_json<type>(value.r, context) + u"," + parser_to_json<type>(value.g, context) + u"," + parser_to_json<type>(value.b, context) + u"]";
        return result;
    } else if constexpr (std::is_same<T, float>::value) {
        std::u16string out = float_to_u16string(value);
        return out;
    } else if constexpr (std::is_same<T, optical_properties_t>::value) {
        std::u16string result = u"[" + parser_to_json<float>(value.attenuation.r, context) + u"," + parser_to_json<float>(value.attenuation.g, context) + u"," + parser_to_json<float>(value.attenuation.b, context) + u"]";
        return result;
    } else if constexpr (std::is_same<T, AI_FOV>::value) {
        std::u16string result = u"[" + parser_to_json<int>(value.radius, context) + u"," + parser_to_json<std::size_t>(value.qualifier->index, context) + u"," + parser_to_json<int>(value.start_angle, context) + u"," + parser_to_json<int>(value.end_angle, context) + u"]";
        return result;
    } else if constexpr (std::is_same<T, Icon>::value || std::is_same<T, TileManager>::value || is_instance_dictionary<T>::value) {
        // LOG(FATAL) << "<parser_to_json> type error: " << typeid(value).name();
        return u"";
    } else if constexpr (std::is_same<T, dimension_t<int>>::value) {
        std::u16string result = u"[" + parser_to_json<int>(value.w, context) + u"," + parser_to_json<int>(value.h, context) + u"]";
        return result;
    } else if constexpr (std::is_same<T, dimension3_t>::value) {
        std::u16string result = u"[" + parser_to_json<int>(value.dx, context) + u"," + parser_to_json<int>(value.dy, context) + u"," + parser_to_json<int>(value.dz, context) + u"]";
        return result;
    } else if constexpr (std::is_same<T, rectangle_t<int>>::value) {
        std::u16string result = u"[" + parser_to_json<int>(value.x, context) + u"," + parser_to_json<int>(value.y, context) + u"," + parser_to_json<int>(value.w, context) + u"," + parser_to_json<int>(value.h, context) + u"]";
        return result;
    } else if constexpr (std::is_same<T, atlas_tile_t>::value) {
        std::u16string result = u"";
        return result;
    } else {
        // LOG(FATAL) << "<parser_to_json> type error: " << typeid(value).name();
        // static_assert(always_false<T>::value, "<parser_to_json> for this type not
        // found");
    }
};

template <typename T>
void parser_from_binary(const std::string& value,
    T& prop,
    std::size_t& pos,
    SerializationContext& context)
{
    if constexpr (std::is_pointer<T>::value) {
        if constexpr (std::is_base_of<iSerializable,
                          std::remove_pointer_t<T>>::value) {
            const auto id = *reinterpret_cast<const std::size_t*>(&value[pos]);
            pos += sizeof(std::size_t);
            switch (id) {
            case 0: {
                prop = nullptr;
                break;
            }
            /*	case 1:
                            {
                                    LOG(INFO) << "form binaryyyyyyyyyyyy " <<
           typeid(T).name(); std::size_t index = *reinterpret_cast<const
           std::size_t*>(&value[pos]); pos += sizeof(std::size_t); prop =
           dynamic_cast<T>(m_items[index-1]); break;
                            }*/
            default: {
                // LOG(INFO) << "from binaryyyyyyyyyyyy " << typeid(T).name();
                prop = dynamic_cast<T>(Serialization::instance()
                                           .t_constructors_from_binary[id - 1]
                                           ->from_binary(value, pos, context));
                // LOG(INFO) << std::to_string(prop==nullptr);
                break;
            }
            }
        } else {
            const auto id = *reinterpret_cast<const char*>(&value[pos]);
            pos += sizeof(char);
            switch (id) {
            case 0: {
                prop = nullptr;
                break;
            }
            case 1: {
                using Type = typename std::remove_pointer_t<T>;
                T v = new Type();
                parser_from_binary<Type>(value, *v, pos, context);
                prop = v;
                break;
            }
            }
        }
    } else if constexpr (std::is_base_of<iSerializable, T>::value) {
        prop.from_binary(value, pos, context);
    } else if constexpr (is_vector<T>::value) {
        using Value = typename T::value_type;
        auto s = *reinterpret_cast<const std::size_t*>(&value[pos]);
        pos += sizeof(std::size_t);
        prop.resize(s);
        for (std::size_t i = 0; i < s; ++i) {
            Value v;
            parser_from_binary<Value>(value, prop[i], pos, context);
        }
    } else if constexpr (is_list<T>::value) {
        using Value = typename T::value_type;
        const auto s = *reinterpret_cast<const std::size_t*>(&value[pos]);
        pos += sizeof(std::size_t);
        for (std::size_t i = 0; i < s; ++i) {
            Value v;
            parser_from_binary<Value>(value, v, pos, context);
            prop.push_back(v);
        }
    } else if constexpr (is_map<T>::value) {
        using Key = typename T::key_type;
        using Value = typename T::mapped_type;
        const auto s = *reinterpret_cast<const std::size_t*>(&value[pos]);
        pos += sizeof(std::size_t);
        for (std::size_t i = 0; i < s; ++i) {
            Key k;
            Value v;
            parser_from_binary<Key>(value, k, pos, context);
            parser_from_binary<Value>(value, v, pos, context);
            prop[k] = v;
        }
    } else if constexpr (std::is_same<T, int>::value || std::is_same<T, std::size_t>::value || std::is_same<T, float>::value || std::is_same<T, bool>::value || std::is_enum<T>::value) {
        prop = *reinterpret_cast<const T*>(&value[pos]);
        pos += sizeof(T);
    } else if constexpr (std::is_same<T, std::string>::value) {
        const auto s = *reinterpret_cast<const std::size_t*>(&value[pos]);
        pos += sizeof(std::size_t);
        prop = std::string(reinterpret_cast<const char*>(&value[pos]), s);
        pos += s;
    } else if constexpr (std::is_same<T, std::u16string>::value) {
        const auto s = *reinterpret_cast<const std::size_t*>(&value[pos]);
        pos += sizeof(std::size_t);
        prop = std::u16string(reinterpret_cast<const char16_t*>(&value[pos]), s);
        pos += s * 2;
        // LOG(INFO) << "NAME: " << Parser::UTF16_to_CP1251(prop);
    } else if constexpr (is_rgb<T>::value) {
        using type = typename T::type;
        parser_from_binary<type>(value, prop.r, pos, context);
        parser_from_binary<type>(value, prop.g, pos, context);
        parser_from_binary<type>(value, prop.b, pos, context);
    } else if constexpr (std::is_same<T, optical_properties_t>::value) {
        parser_from_binary<float>(value, prop.attenuation.r, pos, context);
        parser_from_binary<float>(value, prop.attenuation.g, pos, context);
        parser_from_binary<float>(value, prop.attenuation.b, pos, context);
    } else if constexpr (std::is_same<T, AI_FOV>::value) {
        parser_from_binary<int>(value, prop.radius, pos, context);
        std::size_t x;
        parser_from_binary<std::size_t>(value, x, pos, context);
        prop.qualifier = Application::instance().m_ai_manager->m_fov_qualifiers[x];
        parser_from_binary<int>(value, prop.start_angle, pos, context);
        parser_from_binary<int>(value, prop.end_angle, pos, context);
    } else if constexpr (std::is_same<T, predicate_t>::value) {
        std::size_t result;
        parser_from_binary<std::size_t>(value, result, pos, context);
        prop = *Application::instance().m_ai_manager->m_path_qualifiers[result];
    } else if constexpr (std::is_same<T, dimension_t<int>>::value) {
        parser_from_binary<int>(value, prop.w, pos, context);
        parser_from_binary<int>(value, prop.h, pos, context);
    } else if constexpr (std::is_same<T, dimension3_t>::value) {
        parser_from_binary<int>(value, prop.dx, pos, context);
        parser_from_binary<int>(value, prop.dy, pos, context);
        parser_from_binary<int>(value, prop.dz, pos, context);
    } else if constexpr (std::is_same<T, Icon>::value || std::is_same<T, TileManager>::value || is_instance_dictionary<T>::value) {
        Logger::instance().critical("<from_binary> type error: {}",
            typeid(prop).name());
    } else {
        Logger::instance().critical("<from_binary> type error: {}",
            typeid(prop).name());
        // static_assert(always_false<T>::value, "<from_binary> type error");
    }
}

template <typename T>
std::string
parser_to_binary(T& value, SerializationContext& context)
{
    if constexpr (std::is_pointer<T>::value) {
        if constexpr (std::is_base_of<iSerializable,
                          std::remove_pointer_t<T>>::value) {
            if (value) {
                return value->get_packer().to_binary(value, context);
            }
            std::size_t id = 0;
            return std::string(reinterpret_cast<const char*>(&id),
                sizeof(std::size_t));
        } else {
            if (value == nullptr) {
                char s = 0;
                return std::string(reinterpret_cast<const char*>(&s), sizeof(char));
            } else {
                char s = 1;
                using Type = std::remove_pointer_t<T>;
                return std::string(reinterpret_cast<const char*>(&s), sizeof(char)) + parser_to_binary<Type>(*value, context);
            }
        }
    } else if constexpr (std::is_base_of<iSerializable, T>::value) {
        return value.to_binary(context);
    } else if constexpr (std::is_same<T, int>::value || std::is_same<T, std::size_t>::value || std::is_same<T, float>::value || std::is_same<T, bool>::value || std::is_enum<T>::value) {
        return std::string(reinterpret_cast<const char*>(&value), sizeof(T));
    }
    // else if constexpr(std::is_same<T, std::string>::value)
    //{
    //	std::size_t s = value.size();
    //	return std::string(reinterpret_cast<const char*>(&s),
    // sizeof(std::size_t)) + value;
    //}
    else if constexpr (std::is_same<T, std::u16string>::value) {
        std::size_t s = value.size();
        return std::string(reinterpret_cast<const char*>(&s), sizeof(std::size_t)) + std::string(reinterpret_cast<const char*>(&(value[0])), value.size() * 2);
    } else if constexpr (is_vector<T>::value || is_list<T>::value || is_map<T>::value) {
        using Value = typename T::value_type;
        std::size_t s = value.size();
        auto result = std::string(reinterpret_cast<const char*>(&s), sizeof(std::size_t));
        for (auto element : (value)) {
            result += parser_to_binary<Value>(element, context);
        }
        return result;
    } else if constexpr (is_pair<T>::value) {
        using Key = std::remove_const_t<typename T::first_type>;
        using Value = typename T::second_type;
        Key key = value.first;
        return parser_to_binary<Key>(key, context) + parser_to_binary<Value>(value.second, context);
    } else if constexpr (std::is_same<T, std::string>::value) {
        return value;
    } else if constexpr (is_rgb<T>::value) {
        using type = typename T::type;
        return parser_to_binary<type>(value.r, context) + parser_to_binary<type>(value.g, context) + parser_to_binary<type>(value.b, context);
    } else if constexpr (std::is_same<T, optical_properties_t>::value) {
        return parser_to_binary<float>(value.attenuation.r, context) + parser_to_binary<float>(value.attenuation.g, context) + parser_to_binary<float>(value.attenuation.b, context);
    } else if constexpr (std::is_same<T, AI_FOV>::value) {
        return parser_to_binary<int>(value.radius, context) + parser_to_binary<std::size_t>(value.qualifier->index, context) + parser_to_binary<int>(value.start_angle, context) + parser_to_binary<int>(value.end_angle, context);
    } else if constexpr (std::is_same<T, predicate_t>::value) {
        return parser_to_binary<std::size_t>(value.index, context);
    } else if constexpr (std::is_same<T, dimension_t<int>>::value) {
        return parser_to_binary<int>(value.w, context) + parser_to_binary<int>(value.h, context);
    } else if constexpr (std::is_same<T, dimension3_t>::value) {
        return parser_to_binary<int>(value.dx, context) + parser_to_binary<int>(value.dy, context) + parser_to_binary<int>(value.dz, context);
    } else if constexpr (std::is_same<T, Icon>::value || std::is_same<T, TileManager>::value || is_instance_dictionary<T>::value) {
        Logger::instance().critical("<to_binary> type error: ",
            typeid(value).name());
        return "";
    } else {
        Logger::instance().critical("<to_binary> type error: ",
            typeid(value).name());
        // static_assert(always_false<T>::value, "<to_binary> type error");
        return "";
    }
}

template <typename T>
void object_from_json(T& object, scheme_map_t* value, SerializationContext& context)
{
    constexpr auto properties = std::tuple_size<decltype(T::properties())>::value;
    for_sequence(std::make_index_sequence<properties> {}, [&](auto i) {
        constexpr auto property = std::get<i>(T::properties());
        typedef decltype(property) P;
        if (property.from_json_function) {
            (object.*(property.from_json_function.value()))(
                (*value)[property.name], object.*(property.member), context);
        } else
            parser_from_json<typename P::Type>(
                (*value)[property.name], object.*(property.member), context);
    });
}

template <typename T>
std::u16string
object_to_json(T& object, SerializationContext& context)
{
    std::u16string result;
    constexpr auto properties = std::tuple_size<decltype(T::properties())>::value;
    for_sequence(std::make_index_sequence<properties> {}, [&](auto i) {
        constexpr auto property = std::get<i>(T::properties());
        typedef decltype(property) P;
        if (i != 0) {
            result += u",";
        }
        if (property.to_json_function) {
            result += u"\"" + std::u16string(property.name) + u"\":" + (object.*(property.to_json_function.value()))(object.*(property.member), context);
        } else
            result += u"\"" + std::u16string(property.name) + u"\":" + parser_to_json<typename P::Type>(object.*(property.member), context);
    });
    return result;
}

template <typename T>
void object_from_binary(T& object,
    const std::string& value,
    std::size_t& pos,
    SerializationContext& context)
{
    constexpr auto properties = std::tuple_size<decltype(T::properties())>::value;
    for_sequence(std::make_index_sequence<properties> {}, [&](auto i) {
        constexpr auto property = std::get<i>(T::properties());
        typedef decltype(property) P;
        if (property.from_binary_function) {
            (object.*(property.from_binary_function.value()))(
                value, object.*(property.member), pos, context);
        } else
            parser_from_binary<typename P::Type>(
                value, object.*(property.member), pos, context);
    });
}

template <typename T>
std::string
object_to_binary(T& object, SerializationContext& context)
{
    std::string result;
    constexpr auto properties = std::tuple_size<decltype(T::properties())>::value;
    for_sequence(std::make_index_sequence<properties> {}, [&](auto i) {
        constexpr auto property = std::get<i>(T::properties());
        typedef decltype(property) P;
        if (property.to_binary_function) {
            result += (object.*(property.to_binary_function.value()))(
                object.*(property.member), context);
        } else
            result += parser_to_binary<typename P::Type>(object.*(property.member), context);
    });
    return result;
}

class iSerializable: public iJsonSerializable {
public:
    size_t m_serialization_index;

    iSerializable() { m_serialization_index = 0; }

    virtual void reset_serialization_index() { m_serialization_index = 0; };

    virtual iPacker& get_packer() = 0;

    virtual void from_binary(const std::string& value,
        std::size_t& pos,
        SerializationContext& context)
    {
        this->get_packer().from_binary(*this, value, pos, context);
    }

    virtual void from_json(scheme_map_t* value, SerializationContext& context)
    {
        return this->get_packer().from_json(*this, value, context);
    }

    virtual std::u16string to_json(SerializationContext& context)
    {
        return this->get_packer().to_json(*this, context);
    }

    virtual std::string to_binary(SerializationContext& context)
    {
        return this->get_packer().to_binary(*this, context);
    }
};

class SerializationLink : public iSerializable {
public:
    constexpr static auto properties() { return std::make_tuple(); }
};

template <typename T>
class Packer : public iPacker {
public:
    static Packer& instance()
    {
        static Packer self;
        return self;
    }

    // delete copy and move constructors and assign operators
    Packer(Packer const&) = delete; // Copy construct
    Packer(Packer&&) = delete; // Move construct
    Packer& operator=(Packer const&) = delete; // Copy assign
    Packer& operator=(Packer&&) = delete; // Move assign

    static std::u16string m_type_name;
    static std::size_t m_type_id;

    std::u16string get_type_name() override { return m_type_name; }

    std::size_t get_type_id() override { return m_type_id; }

    iSerializable* from_json(scheme_map_t* value,
        SerializationContext& context) override
    {
        if constexpr (!std::is_abstract<T>::value) {
            T* out = new T;
            context.m_items.push_back(out);
            out->from_json(value, context);
            return out;
        }
        return nullptr;
    }

    iSerializable* from_binary(const std::string& value,
        std::size_t& pos,
        SerializationContext& context) override
    {
        if constexpr (!std::is_abstract<T>::value) {
            T* out = new T;
            context.m_items.push_back(out);
            out->from_binary(value, pos, context);
            return out;
        }
        return nullptr;
    }

    void from_json(iSerializable& object,
        scheme_map_t* value,
        SerializationContext& context) override
    {
        object_from_json<T>(dynamic_cast<T&>(object), value, context);
    }

    std::u16string to_json(iSerializable& object,
        SerializationContext& context) override
    {

        return object_to_json<T>(dynamic_cast<T&>(object), context);
    }

    std::u16string to_json(iSerializable* value,
        SerializationContext& context) override
    {
        switch (value->m_serialization_index) {
        case 0: {
            context.m_object_index += 1;
            value->m_serialization_index = context.m_object_index;
            const auto result = value->to_json(context);
            if (result.empty()) {
                auto out = u"{\"$type\":\"" + value->get_packer().get_type_name() + u"\",\"$link_id\":" + int_to_u16string(value->m_serialization_index) + u"}";
                return out;
            }
            auto out = u"{\"$type\":\"" + value->get_packer().get_type_name() + u"\",\"$link_id\":" + int_to_u16string(value->m_serialization_index) + u"," + result + u"}";
            return out;
        }
        default: {
            if (context.m_object_index < value->m_serialization_index) {
                Logger::instance().critical(
                    std::to_string(context.m_object_index) + " | " + std::to_string(value->m_serialization_index) + " | " + typeid(value).name() + " | " + utf16_to_cp1251(value->get_packer().get_type_name()));
            }
            auto out = u"{\"$type\":\"link\",\"value\":" + int_to_u16string(value->m_serialization_index) + u"}";
            return out;
        }
        }
    }

    void from_binary(iSerializable& object,
        const std::string& value,
        std::size_t& pos,
        SerializationContext& context) override
    {
        object_from_binary<T>(dynamic_cast<T&>(object), value, pos, context);
    }

    std::string to_binary(iSerializable& object,
        SerializationContext& context) override
    {

        return object_to_binary<T>(dynamic_cast<T&>(object), context);
    }

    std::string to_binary(iSerializable* value,
        SerializationContext& context) override
    {
        switch (value->m_serialization_index) {
        case 0: {
            context.m_object_index += 1;
            value->m_serialization_index = context.m_object_index;
            auto id = value->get_packer().get_type_id() + 1;
            return std::string(reinterpret_cast<const char*>(&id),
                       sizeof(std::size_t))
                + value->to_binary(context);
        }
        default: {
            if (context.m_object_index < value->m_serialization_index) {
                Logger::instance().critical(
                    std::to_string(context.m_object_index) + " | " + std::to_string(value->m_serialization_index) + " | " + typeid(value).name() + " | " + utf16_to_cp1251(value->get_packer().get_type_name()));
            }
            auto id = Serialization::instance()
                          .t_constructors_from_json[u"link"]
                          ->get_type_id()
                + 1;
            return std::string(reinterpret_cast<const char*>(&id),
                       sizeof(std::size_t))
                + std::string(
                    reinterpret_cast<const char*>(&(value->m_serialization_index)),
                    sizeof(std::size_t));
        }
        }
    }

    Packer() = default;
};

template <typename T>
std::u16string Packer<T>::m_type_name = u"";

template <typename T>
std::size_t Packer<T>::m_type_id = 0;

template <typename T>
class Tree {
public:
    T m_value;

    std::list<Tree> m_nodes;

    Tree() {};

    Tree(T const& value)
        : m_value(value) {};

    Tree(T& value)
        : m_value(value) {};

    Tree* add_node(T& value)
    {
        m_nodes.push_back(Tree(value));
        return &m_nodes.back();
    }

    Tree* add_node(T const& value)
    {
        m_nodes.push_back(Tree(value));
        return &m_nodes.back();
    }
};

class Visitor_generic;

class Apply_info {
public:
    GameObject* m_unit;
    Object_interaction* m_object;

    Apply_info(GameObject* unit = nullptr, Object_interaction* object = nullptr);
};

class Object_interaction : public virtual iSerializable {
public:
    typedef std::function<void(Object_interaction*)> predicate;
    typedef std::function<void(Object_interaction*, bool)> predicate_ex;

    virtual interaction_message_type_e get_interaction_message_type() = 0;

    virtual void apply_effect(GameObject* unit, Object_interaction* object) {};

    virtual void apply(Apply_info& value) {};
    virtual Object_interaction* clone() = 0;
    virtual void do_predicate(Visitor& helper) { helper.visit(*this); };
    virtual void apply_visitor(Visitor_generic& visitor);

    constexpr static auto properties()
    {
        return std::make_tuple();
    }
};

// typedef iSerializable* (*constructor_from_json_t)(scheme_map_t*);
// typedef iSerializable* (*constructor_from_binary_t)(const std::string&,
// std::size_t&);

#endif // DEFINITION_H
