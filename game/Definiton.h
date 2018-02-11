#ifndef DEFINITION_H
#define	DEFINITION_H

#include <functional>
#include "graphics/gl.hpp"
#include <list>
#include <log.h>

#include <string>
#include <map>
#include <experimental/string>
#include <locale>
#include <codecvt>
#include <unordered_map>

const float Pi = 3.14159265F;
const float cos22_5 = cos(22.5F*Pi / 180.0F);
const float cos67_5 = cos(67.5F*Pi / 180.0F);

class GameObject;
class GUI_Object;
class Action;
class Parameter;

const int tile_size_x = 38;
const int tile_size_y = 22;
const int tile_size_x_half = 19;
const int tile_size_y_half = 11;


struct position_t
{
	int x;
	int y;

	position_t(int x, int y) : x(x), y(y) {}
	position_t() : x(0), y(0) {}
};

struct dimension_t
{
	int w;
	int h;

	dimension_t(int w, int h) : w(w), h(h) {}
	dimension_t() : w(0), h(0) {}
};

struct view_t
{
	int l;
	int r;
	int u;
	int d;

	view_t(int l, int r, int u, int d) :l(l), r(r), u(u), d(d) {}
	view_t() :l(0), r(0), u(0), d(0) {}
};

struct rectangle_t : public position_t, public dimension_t
{
	rectangle_t(int x, int y, int w, int h) : position_t(x, y), dimension_t(w, h) {}
	rectangle_t(int x, int y, const dimension_t &dimension) : position_t(x, y), dimension_t(dimension) {}
	rectangle_t() : position_t(), dimension_t() {}

	inline int right()
	{
		return x + w;
	}

	inline int bottom()
	{
		return y + h;
	}
};

struct area_t
{
	position_t p1;
	position_t p2;

	area_t(position_t p1, position_t p2) :p1(p1), p2(p2) {}
};

struct fposition_t
{
	float x;
	float y;

	fposition_t(float x, float y) : x(x), y(y) {}
	fposition_t() : x(0.0f), y(0.0f) {}
};

struct fdimension_t
{
	float w;
	float h;

	fdimension_t(float w, float h) : w(w), h(h) {}
	fdimension_t() : w(0.0f), h(0.0f) {}
};

struct frectangle_t : public fposition_t, public fdimension_t
{
	frectangle_t(float x, float y, float w, float h) : fposition_t(x, y), fdimension_t(w, h) {}
	frectangle_t(float x, float y, const fdimension_t &dimension) : fposition_t(x, y), fdimension_t(dimension) {}
	frectangle_t() : fposition_t(), fdimension_t() {}
};

struct MouseEventArgs
{
	position_t position;
	int key;
	int value;

	MouseEventArgs(const position_t &position, int key) : position(position), key(key), value(0) {}
	MouseEventArgs(const position_t &position, int key, int value) : position(position), key(key), value(value) {}
};

enum mouse_key_e
{
	mk_none = 0,
	mk_left = 1,
	mk_right = 2,
	mk_middle = 3
};

struct VoidEventArgs
{
};

enum class entity_e
{
	cell,
	inventory_cell,
	body_part,
	game_object
};

class action_e
{
public:
	enum type
	{
		none = -1,
		move,
		move_step,
		push,
		turn,
		open_inventory,
		cell_info,
		set_motion_path,
		pick,
		open,
		hit,
		hit_melee,
		equip,
		show_parameters,
		use,
		save,
		autoexplore,
		shoot,
		load,
		move_out,
		max
	};
};


enum class object_state_e
{
	alive,
	dead,
	on,
	off,
	equip
};

enum class parameter_type_e
{
	position,
	mapcell,
	//ParameterKind_MoveObjectByUnit,
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

enum class object_direction_e :int
{
	down,
	downleft,
	left,
	topleft,
	top,
	topright,
	right,
	downright,
	none
};

enum class rotate_direction_e :int
{
	counterclockwise = -1,
	clockwise = 1

};

object_direction_e operator+(object_direction_e lhs, const rotate_direction_e& rhs);

struct tile_t
{
	GLuint unit;
	GLdouble coordinat[4];
};

struct game_object_size_t
{
	int x;
	int y;
	int z;

	game_object_size_t(int x, int y, int z) : x(x), y(y), z(z) {}
	game_object_size_t() : x(0), y(0), z(0) {}
};

struct RGBA_t
{
	float R, G, B, A;

	RGBA_t(float R, float G, float B, float A) : R(R), G(G), B(B), A(A) {};

	float RGBA_t::operator[](const std::size_t& index)
	{
		switch (index)
		{
		case 0: return R;
		case 1: return G;
		case 2: return B;
		case 3: return A;
		}
	}
};

struct RGB_t
{
	float R, G, B;
	RGB_t(float R, float G, float B) : R(R), G(G), B(B) {};
	RGB_t() : R(0.0), G(0.0), B(0.0) {};
};

typedef std::map<std::u16string, std::u16string> scheme_map_t;
typedef std::list<std::u16string> scheme_list_t;
typedef std::vector<std::u16string> scheme_vector_t;

struct optical_properties_t
{
	RGB_t attenuation;
	optical_properties_t(RGB_t attenuation) : attenuation(attenuation) {};
	optical_properties_t() : attenuation(RGB_t(1.0, 1.0, 1.0)) {};
	//optical_properties_t(const optical_properties_t& value) :seethrough(value.seethrough),attenuation(value.attenuation){};
};

struct light_t
{
	int R, G, B;

	light_t(int R, int G, int B) : R(R), G(G), B(B) {};
	light_t() : R(0), G(0), B(0) {};
};

enum class interaction_message_type_e
{
	single,
	list,
	action,
	part,
	set_attribute_map,
	select_location,
	mover,
	allocator,
	slot_time,
	tag,
	instruction_slot_parameter,
	instruction_game_owner,
	game_object,
	instruction_result,
	effect
};

enum class interaction_e
{
	total_damage,
	damage,
	buff,
	use,
	health,
	strength,
	intelligence,
	dexterity,
	hunger,
	thirst,
	demand_weapon_skill,
	evasion_skill,
	tag,
	poison,
	action,
	body,
	skill_sword,
	skill_bow,
	weapon_damage,
	strength_bonus,
	weapon_range,
	vision,
	vision_component,
	skill_unarmed_combat,
	equip
};

enum class effect_e
{
	value,
	limit,
	start_angle,
	end_angle
};

enum class body_part_e
{
	//eye,
	mouth,
	//ear,
	//nose,
	//teeth,
	//tongue,
	wrist,
	finger,
	head,
	hand,
	foot,
	waist,
	container
};

enum class effect_prefix_e
{
	physical_damage,
	poison_damage,
	parameter_change,
	state_change,
	action
};

enum class object_tag_e
{
	none,
	poison_resist,
	purification_from_poison,
	mortal,
	pass_able,
	pick_able,
	seethrough_able,
	activator,
	fast_move,
	equippable,
	ring,
	requirements_to_object,
	cursed,
	can_transfer_object,
	footwear
};

enum class feature_list_type_e
{
	generic,
	parameter,
	tag,
	action,
	parts,
	vision,
	vision_component
};

enum class ai_type_e
{
	trap,
	non_humanoid
};

class Object_interaction;

class Visitor
{
public:
	virtual void visit(Object_interaction& value) = 0;
};

class GameTask;
class GameObject;

struct tag_t {

	GameTask* task;

	tag_t(GameTask* task) : task(task) {};
	tag_t() : task(nullptr) {};
};

struct f2dvector_t
{
	float x;
	float y;

	f2dvector_t(float x, float y) : x(x), y(y) {}
	f2dvector_t() : x(0.0f), y(0.0f) {}

	void normalize()
	{
		float l = sqrtf(x*x + y * y);
		x = x / l;
		y = y / l;
	}
};

enum class type_e
{
	null,
	link,
	light_t,
	optical_properties_t,
	object_state,
	interaction_list,
	parameter_list,
	action_list,
	parts_list,
	tag_list,
	object_part,
	slot_set_state,
	slot_select_cell,
	slot_allocator,
	slot_mover,
	interaction_copyist,
	interaction_prefix,
	interaction_addon,
	interaction_time,
	interaction_timer,
	effect,
	tag_label,
	tag_poison_resist,
	tag_mortal,
	tag_purification_from_poison,
	tag_fast_move,
	tag_activator,
	action,
	ai_enemy,
	gameobject,
	impact_copy_chance,
	gamemap,
	mapcell,
	inventory_cell,
	action_wrapper,
	vision_list,
	vision_component,
	game_world,
	parameter,
	object_owner,
	direction
};

struct predicat_t
{
	std::function<bool(GameObject*)> predicat;
	size_t index;

	predicat_t() :predicat(), index(0) {};
	predicat_t(std::function<bool(GameObject*)> predicat, size_t index) :predicat(predicat), index(index) {};
};

struct cell_t
{
	int value;
	bool opaque;
};

struct common_action_t
{
	bool pick;

	common_action_t()
	{
		pick = false;
	}
};

struct AI_FOV
{
	int radius;
	predicat_t* qualifier;
	int start_angle;
	int end_angle;

	AI_FOV(int radius, predicat_t* qualifier, int start_angle, int end_angle) : radius(radius), qualifier(qualifier), start_angle(start_angle), end_angle(end_angle) {};
	AI_FOV() : radius(0), qualifier(nullptr), start_angle(0), end_angle(0) {};
};

struct Action_helper_t
{
	Action* action;
	Parameter* parameter;

	Action_helper_t(Action* action = nullptr, Parameter* parameter = nullptr) :action(action), parameter(parameter) { /*if ((!action) || (!parameter)) { LOG(FATAL) << "Action_helper_t инициализирован нулевым указателем"; }*/ };
};

struct font_symbol_t
{
	GLuint id;
	dimension_t size;
	dimension_t bearing;

	font_symbol_t(GLuint id = 0, dimension_t size = dimension_t()) :id(id), size(size) {};
};

enum game_log_message_type_e
{
	message_time,
	message_action_interaction,
	message_battle
};

struct game_log_message_t
{
	game_log_message_type_e type;
	std::u16string value;
	game_log_message_t(game_log_message_type_e type, std::u16string& value) :type(type), value(value) {};
};

class GUI_TextFormat
{
public:

	GUI_TextFormat(int _SizeX, int _SizeY, RGBA_t _Color) : m_symbol_size(dimension_t(_SizeX, _SizeY)), m_color(_Color) {};
	RGBA_t m_color;
	dimension_t m_symbol_size;

};

enum class mode_t
{
	equip,
	unequip,
	check
};

enum class animation_e
{
	idle,
	move
};


template<typename T>
class Dictonary
{
public:

	std::unordered_map<std::string, T> m_by_string;
	std::unordered_map<T, std::string> m_by_enum;


	Dictonary(std::initializer_list<std::tuple<T, std::string>> l)
	{
		for (auto element : l)
		{
			add(std::get<0>(element), std::get<1>(element));
		}
	}

	void add(T enm, std::string json_string)
	{
		m_by_string[json_string] = enm;
		m_by_enum[enm] = json_string;
	}

	void add(T enm, std::string json_string, std::string description_string)
	{
		m_by_string[json_string] = enm;
		m_by_enum[enm] = json_string;
	}

	std::string get_string(T key)
	{
		auto value = m_by_enum.find(key);
		if (value == m_by_enum.end())
		{
			LOG(FATAL) << "Элемент отсутствует";
		}
		return value->second;
	}

	T get_enum(const std::string& key)
	{
		auto value = m_by_string.find(key);
		if (value == m_by_string.end())
		{
			LOG(FATAL) << "Элемент отсутствует";
		}
		return value->second;
	}

};


class InstanceDictonaryElement
{
public:

	std::size_t m_index;
	std::string m_json_string;

};


class Icon :public InstanceDictonaryElement
{
public:

	GLuint m_value;
};

template<typename T>
class InstanceDictonary
{
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
		if (value == m_by_string.end())
		{
			LOG(FATAL) << "Элемент отсутствует";
		}
		return value->second;
	}
};

class iSerializable;

template<typename Class, typename T, typename P>
struct Property {
	constexpr Property(T Class::*aMember, const char16_t* aName) : member{ aMember }, name{ aName } {}

	using Type = T;
	using PropType = P;

	T Class::*member;
	const char16_t* name;
};

template<typename Class, typename T>
constexpr auto make_property(T Class::*member, const char16_t* name) {
	return Property<Class, T, T>{member, name};
}

template<typename P, typename Class, typename T>
constexpr auto make_property(T Class::*member, const char16_t* name) {
	return Property<Class, T, P>{member, name};
}

template<typename A, typename B>
constexpr auto make_union(A a, B b) {
	return std::tuple_cat(a, b);
}

template <typename T, T... S, typename F>
constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f) {
	using unpack_t = int[];
	(void)unpack_t {
		(static_cast<void>(f(std::integral_constant<T, S>{})), 0)..., 0
	};
}

template<typename T> void object_from_json(T& object, scheme_map_t* value)
{
	constexpr auto nbProperties = std::tuple_size<decltype(T::properties())>::value;
	for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i) {
		constexpr auto property = std::get<i>(T::properties());
		typedef decltype(property) P;
		if constexpr(std::is_same<typename P::Type, typename P::PropType>::value)
			Parser::from_json<typename P::Type>((*value)[property.name], object.*(property.member));
		else
			Parser::from_json<typename P::Type, P::PropType>((*value)[property.name], object.*(property.member));
	});
}

template<typename T> std::u16string object_to_json(T& object)
{
	std::u16string result;
	constexpr auto nbProperties = std::tuple_size<decltype(T::properties())>::value;
	for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i) {
		constexpr auto property = std::get<i>(T::properties());
		typedef decltype(property) P;
		if (i != 0) { result += u","; }
		if constexpr(std::is_same<typename P::Type, typename P::PropType>::value)
			result += u"\"" + std::u16string(property.name) + u"\":" + Parser::to_json<typename P::Type>(object.*(property.member));
		else
			result += u"\"" + std::u16string(property.name) + u"\":" + Parser::to_json<typename P::Type, P::PropType>(object.*(property.member));
	});
	return result;
}

template<typename T> void object_from_binary(T& object, const std::string& value, std::size_t& pos)
{
	constexpr auto nbProperties = std::tuple_size<decltype(T::properties())>::value;
	for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i) {
		constexpr auto property = std::get<i>(T::properties());
		typedef decltype(property) P;
		if constexpr(std::is_same<typename P::Type, typename P::PropType>::value)
			Parser::from_binary<typename P::Type>(value, object.*(property.member), pos);
		else
			Parser::from_binary<typename P::Type, P::PropType>(value, object.*(property.member), pos);
	});
}

template<typename T> std::string object_to_binary(T& object)
{
	std::string result;
	constexpr auto nbProperties = std::tuple_size<decltype(T::properties())>::value;
	for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i) {
		constexpr auto property = std::get<i>(T::properties());
		typedef decltype(property) P;
		if constexpr(std::is_same<typename P::Type, typename P::PropType>::value)
			result += Parser::to_binary<typename P::Type>(object.*(property.member));
		else
			result += Parser::to_binary<typename P::Type, P::PropType>(object.*(property.member));
	});
	return result;
}

class Packer_generic
{
public:

	virtual void from_json(iSerializable& object, scheme_map_t* value) = 0;
	virtual std::u16string to_json(iSerializable& object) = 0;
	virtual void from_binary(iSerializable& object, const std::string& value, std::size_t& pos) = 0;
	virtual std::string to_binary(iSerializable& object) = 0;
	virtual std::u16string get_type_name() = 0;
	virtual unsigned int get_type_id() = 0;
	virtual bool is_singleton()
	{
		return false;
	}

};

class iSerializable
{
public:

	size_t m_serialization_index;

	virtual void reset_serialization_index() { m_serialization_index = 0; };

	virtual void save() = 0;
	virtual void load() = 0;

	virtual Packer_generic& get_packer();
	//virtual Packer_generic& get_packer() const;

	virtual void from_binary(const std::string& value, std::size_t& pos)
	{
		this->get_packer().from_binary(*this, value, pos);
	}

	virtual void from_binary_const(const std::string& value, const std::size_t pos = 0)
	{
		std::size_t p = pos;
		this->get_packer().from_binary(*this, value, p);
	}


	virtual void from_json(scheme_map_t* value)
	{
		return this->get_packer().from_json(*this, value);
	}

	virtual std::u16string to_json()
	{
		return this->get_packer().to_json(*this);
	}

	virtual std::string to_binary()
	{
		return this->get_packer().to_binary(*this);
	}

};

template<typename T> iSerializable* create_instance(scheme_map_t* value)
{
	T* out = new T;
	out->from_json(value);
	return out;
}

template<typename T> iSerializable* create_instance(const std::string& value, std::size_t& pos)
{
	T* out = new T;
	Parser::m_items.push_back(out);
	out->from_binary(value, pos);
	return out;
}


template<typename T>
class Packer :public Packer_generic
{
public:

	static Packer& Instance()
	{
		static Packer self;
		return self;
	}

	// delete copy and move constructors and assign operators
	Packer(Packer const&) = delete;             // Copy construct
	Packer(Packer &&) = delete;                  // Move construct
	Packer& operator=(Packer const&) = delete;  // Copy assign
	Packer& operator=(Packer &&) = delete;      // Move assign

	std::u16string m_type_name;
	unsigned int m_type_id;

	std::u16string get_type_name() override
	{
		return m_type_name;
	}

	unsigned int get_type_id() override
	{
		return m_type_id;
	}

	void from_json(iSerializable& object, scheme_map_t* value) override
	{
		object_from_json<T>(dynamic_cast<T&>(object), value);
	}

	std::u16string to_json(iSerializable& object) override
	{

		return object_to_json<T>(dynamic_cast<T&>(object));
	}

	void from_binary(iSerializable& object, const std::string& value, std::size_t& pos) override
	{
		object_from_binary<T>(dynamic_cast<T&>(object), value, pos);
	}

	std::string to_binary(iSerializable& object) override
	{

		return object_to_binary<T>(dynamic_cast<T&>(object));
	}

	Packer()
	{
	}

};

template<>
class Packer<Action> :public Packer_generic
{
public:

	static Packer& Instance()
	{
		static Packer self;
		return self;
	}

	// delete copy and move constructors and assign operators
	Packer(Packer const&) = delete;             // Copy construct
	Packer(Packer &&) = delete;                  // Move construct
	Packer& operator=(Packer const&) = delete;  // Copy assign
	Packer& operator=(Packer &&) = delete;      // Move assign

	std::u16string m_type_name;
	unsigned int m_type_id;

	std::u16string get_type_name() override
	{
		return m_type_name;
	}

	unsigned int get_type_id() override
	{
		return m_type_id;
	}

	void from_json(iSerializable& object, scheme_map_t* value) override
	{
	}

	std::u16string to_json(iSerializable& object) override;


	void from_binary(iSerializable& object, const std::string& value, std::size_t& pos) override
	{
	}

	std::string to_binary(iSerializable& object) override;

	bool is_singleton() override
	{
		return true;
	}

	Packer()
	{
	}

};

template<typename T>
class Tree
{
public:

	T m_value;

	std::list<Tree> m_nodes;

	Tree() {};

	Tree(T const& value) :m_value(value) {};

	Tree(T& value) :m_value(value) {};

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

class Apply_info
{
public:

	GameObject * m_unit;
	Object_interaction* m_object;

	Apply_info(GameObject* unit = nullptr, Object_interaction* object = nullptr);

};


class Object_interaction : public virtual iSerializable
{
public:

	typedef std::function<void(Object_interaction*)> predicat;
	typedef std::function<void(Object_interaction*, bool)> predicat_ex;

	interaction_message_type_e m_interaction_message_type;
	std::u16string m_namename;
	Object_interaction() { m_namename = u"base"; };
	virtual void apply_effect(GameObject* unit, Object_interaction* object) {};

	virtual void apply(Apply_info& value) {};
	virtual Object_interaction* clone() { return nullptr; };
	virtual void do_predicat(Visitor& helper) { helper.visit(*this); };
	virtual void apply_visitor(Visitor_generic& visitor);

	Packer_generic& get_packer() override
	{
		return Packer<Object_interaction>::Instance();
	}

	constexpr static auto properties()
	{
		return std::make_tuple(make_property(&Object_interaction::m_namename, u"m_value"));
	}

};

typedef iSerializable* (*constructor_from_json_t)(scheme_map_t*);
typedef iSerializable* (*constructor_from_binary_t)(const std::string&, std::size_t&);

class Parser
{

private:
	static std::size_t m_object_index;
	static unsigned int m_type_id_counter;


public:

	struct tile_t {};
	struct InstanceDictonary_t {};
	struct from_InstanceDictonary_t {};

	static std::wstring_convert<std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>, wchar_t> m_convert;
	static std::map<std::u16string, constructor_from_json_t> m_constructors_from_json;
	static std::vector<constructor_from_binary_t> m_constructors_from_binary;
	static std::vector<iSerializable*> m_items;

	static Dictonary<interaction_e> m_json_interaction_e;
	static Dictonary<object_tag_e> m_json_object_tag;
	static Dictonary<body_part_e> m_json_body_part_e;
	static Dictonary<ai_type_e> m_json_ai_type_e;
	static Dictonary<feature_list_type_e> m_json_feature_list_type_e;
	static Dictonary<entity_e> m_json_entity_e;
	static Dictonary<action_e::type> m_json_action_e;
	static Dictonary<object_state_e> m_json_object_state_e;
	static Dictonary<effect_e> m_json_effect_e;

	static std::unordered_map<effect_e, std::u16string> m_string_effect_e;
	static std::unordered_map<interaction_e, std::u16string> m_string_interaction_e;
	static std::unordered_map<object_tag_e, std::u16string> m_string_object_tag_e;

	Parser();
	~Parser();

	//static std::u16string serialize_object(const iSerializable* value);
	static scheme_map_t* read_object(std::u16string& value);
	static scheme_list_t* read_array(std::u16string& value);
	static scheme_vector_t* read_pair(std::u16string& value);
	static void print_u16string(std::u16string value);
	static scheme_map_t* parse_object(std::u16string& value);
	static scheme_list_t* parse_array(std::u16string& value);
	static scheme_vector_t* parse_pair(std::u16string& value);
	static std::u16string get_value(const std::u16string& value);
	static std::u16string to_u16string(int const& i);
	static std::u16string float_to_u16string(float const& i);
	static int to_int(const std::u16string& value);
	static float to_float(const std::u16string& value);
	static std::string UTF16_to_CP866(std::u16string const& value);
	static std::u16string CP866_to_UTF16(std::string const& value);
	static void reset_object_counter();

	static void register_class(std::u16string class_name, constructor_from_json_t function);

	template<typename T>
	static void register_class(std::u16string class_name)
	{
		m_constructors_from_json[class_name] = create_instance<T>;
		m_constructors_from_binary.push_back(create_instance<T>);
		Packer<T>::Instance().m_type_name = class_name;
		Packer<T>::Instance().m_type_id = m_type_id_counter;
		m_type_id_counter += 1;
	}

	template<typename T>
	static void register_class(std::u16string class_name, constructor_from_json_t function)
	{
		m_constructors_from_json[class_name] = function;
		//m_constructors_from_binary.push_back(function);
		Packer<T>::Instance().m_type_name = class_name;
		Packer<T>::Instance().m_type_id = m_type_id_counter;
		m_type_id_counter += 1;
	}

	template <class T>
	struct is_map {
		static constexpr bool value = false;
	};

	template<class Key, class Value>
	struct is_map<std::map<Key, Value>> {
		static constexpr bool value = true;
	};

	template <class T>
	struct is_list {
		static constexpr bool value = false;
	};

	template<class Value>
	struct is_list<std::list<Value>> {
		static constexpr bool value = true;
	};

	template <class T>
	struct is_pair {
		static constexpr bool value = false;
	};

	template<class Key, class Value>
	struct is_pair<std::pair<Key, Value>> {
		static constexpr bool value = true;
	};

	template<class T> struct always_false : std::false_type {};

	template<typename T> static void from_json(const std::u16string value, T& prop)
	{
		if constexpr (std::is_pointer<T>::value)
		{
			if constexpr(std::is_base_of<iSerializable, std::remove_pointer_t<T>>::value)
			{
				std::u16string temp(value);
				scheme_map_t* s = read_object(temp);
				if (s)
				{
					prop = dynamic_cast<T>(m_constructors_from_json[get_value((*s)[u"$type"])](s));
					LOG(INFO) << UTF16_to_CP866(prop->get_packer().get_type_name());
					delete s;
					return;
				}
				prop = nullptr;
			}
			else
			{
				if (get_value(value) == u"null")
				{
					prop = nullptr;
				}
				else
				{
					using Type = std::remove_pointer_t<T>;
					T v = new Type();
					Parser::from_json<Type>(value,*v);
					prop = v;

					/*using Type = std::remove_pointer_t<T>;
					T v = new Type();
					from_json<Type&>(value, *v);
					prop = v;*/
				}
			}
		}
		else if constexpr(std::is_enum<T>::value)
		{
			std::size_t start_pos = value.find(u'"');
			if (start_pos != std::string::npos)
			{
				std::size_t end_pos = value.find(u'"', start_pos + 1);
				if (end_pos != std::string::npos)
				{
					prop = get_dictonary<T>().get_enum(UTF16_to_CP866(value.substr(start_pos + 1, end_pos - start_pos - 1)));
					return;
				}
			}
			prop = static_cast<T>(0);
		}
		else if constexpr(std::is_base_of<iSerializable, T>::value)
		{
			std::u16string temp(value);
			scheme_map_t* s = read_object(temp);
			prop.from_json(s);
			delete s;
		}
		else if constexpr(is_list<T>::value)
		{
			using Value = typename T::value_type;
			std::u16string temp = value;
			scheme_list_t* s = read_array(temp);
			if (s)
			{
				for (auto element : (*s))
				{
					Value v;
					from_json<Value>(element, v);
					prop.push_back(v);
				}
			}
			delete s;
		}
		else if constexpr(is_map<T>::value)
		{
			using Key = typename T::key_type;
			using Value = typename T::mapped_type;
			std::u16string temp = value;
			scheme_list_t* s = read_array(temp);
			Key k;
			Value v;
			if (s)
			{
				for (auto element : (*s))
				{
					scheme_vector_t* p = read_pair(element);
					from_json<Key>((*p)[0], k);
					from_json<Value>((*p)[1], v);
					prop[k] = v;
					delete p;
				}
				delete s;
			}
		}
		else if constexpr(std::is_same<T, int>::value)
		{
			const char16_t* opening_symbol = nullptr;
			for (std::size_t i = 0; i < value.size(); ++i)
			{
				switch (value[i])
				{
				case u'0':case u'1':case u'2':case u'3':case u'4':case u'5':case u'6':case u'7':case u'8':case u'9':
				{
					if (opening_symbol == nullptr)
					{
						opening_symbol = &value[i];
					}
					break;
				}
				default:
				{
					if (opening_symbol != nullptr)
					{
						break;
					}
					break;
				}
				}
			}
			if (opening_symbol != nullptr)
			{
				static std::wstring_convert<std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>, wchar_t> convert;
				std::u16string value(opening_symbol, &value[value.size() - 1] - opening_symbol + 1);
				std::wstring ws = convert.from_bytes(
					reinterpret_cast<const char*> (value.data()),
					reinterpret_cast<const char*> (value.data() + value.size()));
				prop = std::stoi(ws);
			}
			else
			{
				prop = 0;
			}
		}
		else if constexpr(std::is_same<T, std::u16string>::value)
		{
			std::size_t start_pos = value.find(u'"');
			if (start_pos != std::string::npos)
			{
				std::size_t end_pos = value.find(u'"', start_pos + 1);
				if (end_pos != std::string::npos)
				{
					prop = value.substr(start_pos + 1, end_pos - start_pos - 1);
					return;
				}
			}
			prop = value;
		}
		else if constexpr(std::is_same<T, std::string>::value)
		{
			std::size_t start_pos = value.find(u'"');
			if (start_pos != std::string::npos)
			{
				std::size_t end_pos = value.find(u'"', start_pos + 1);
				if (end_pos != std::string::npos)
				{
					prop = UTF16_to_CP866(value.substr(start_pos + 1, end_pos - start_pos - 1));
					return;
				}
			}
			prop = UTF16_to_CP866(value);
		}
		else if constexpr(std::is_same<T, std::size_t>::value)
		{
			std::size_t result = to_int(value);
			prop = result;
		}
		else if constexpr(std::is_same<T, float>::value)
		{
			float result = to_float(value);
			prop = result;
		}
		else if constexpr(std::is_same<T, AI_FOV>::value)
		{
			std::u16string temp = value;
			scheme_vector_t* s = read_pair(temp);
			from_json<int>((*s)[0], prop.radius);
			int x;
			from_json<int>((*s)[1], x);
			prop.qualifier = Application::instance().m_ai_manager->m_fov_qualifiers[x];
			from_json<int>((*s)[2], prop.start_angle);
			from_json<int>((*s)[3], prop.end_angle);
		}
		else if constexpr(std::is_same<T, game_object_size_t>::value)
		{
			std::u16string temp = value;
			scheme_vector_t* s = read_pair(temp);
			from_json<int>((*s)[0], prop.x);
			from_json<int>((*s)[1], prop.y);
			from_json<int>((*s)[2], prop.z);
		}
		else if constexpr(std::is_same<T, predicat_t>::value)
		{
			LOG(INFO) << Parser::UTF16_to_CP866(get_value(value)) << Parser::UTF16_to_CP866(value);
			int result = to_int(value);
			prop = *Application::instance().m_ai_manager->m_path_qualifiers[result];
		}
		else if constexpr(std::is_same<T, light_t>::value)
		{
			std::u16string temp = value;
			scheme_vector_t* s = read_pair(temp);
			from_json<int>((*s)[0], prop.R);
			from_json<int>((*s)[1], prop.G);
			from_json<int>((*s)[2], prop.B);
		}
		else if constexpr(std::is_same<T, optical_properties_t>::value)
		{
			LOG(INFO) << Parser::UTF16_to_CP866(get_value(value)) << Parser::UTF16_to_CP866(value);
			std::u16string temp = value;
			scheme_vector_t* s = read_pair(temp);
			from_json<float>((*s)[0], prop.attenuation.R);
			from_json<float>((*s)[1], prop.attenuation.G);
			from_json<float>((*s)[2], prop.attenuation.B);
		}
		else
		{
			//LOG(FATAL) << typeid(prop).name();
			static_assert(always_false<T>::value, "<from_json> type error");
		}

	};

	template<typename T> static std::u16string to_json(T& value)
	{
		if constexpr(std::is_pointer<T>::value)
		{
			if constexpr(std::is_base_of<iSerializable, std::remove_pointer_t<T>>::value)
			{
				if (value)
				{
					if (value->get_packer().is_singleton())
					{
						LOG(INFO) << UTF16_to_CP866(value->get_packer().get_type_name());
						std::u16string result = value->to_json();
						if (result.empty())
						{
							std::u16string out = u"{\"$type\":\"" + value->get_packer().get_type_name() + u"\"}";
							return out;
						}
						else
						{
							std::u16string out = u"{\"$type\":\"" + value->get_packer().get_type_name() + u"\"," + result + u"}";
							return out;
						}
					}
					else
					{
						switch (value->m_serialization_index)
						{
						case 0:
						{
							m_object_index += 1;
							value->m_serialization_index = m_object_index;
							LOG(INFO) << UTF16_to_CP866(value->get_packer().get_type_name());
							std::u16string result = value->to_json();
							if (result.empty())
							{
								std::u16string out = u"{\"$type\":\"" + value->get_packer().get_type_name() + u"\",\"$link_id\":" + to_u16string(value->m_serialization_index) + u"}";
								return out;
							}
							else
							{
								std::u16string out = u"{\"$type\":\"" + value->get_packer().get_type_name() + u"\",\"$link_id\":" + to_u16string(value->m_serialization_index) + u"," + result + u"}";
								return out;
							}
						}
						default:
						{
							std::u16string out = u"{\"$type\":\"link\",\"value\":" + to_u16string(value->m_serialization_index) + u"}";
							return out;
						}
						}
					}
				}
				else
				{
					return u"null";
				}

			}
			else
			{
				if (value == nullptr)
				{
					return u"null";
				}
				else
				{
					using Type = std::remove_pointer_t<T>;
					return Parser::to_json<Type>(*value);
				}
			}
		}
		else if constexpr(std::is_base_of<iSerializable, T>::value)
		{
			LOG(INFO) << UTF16_to_CP866(value.get_packer().get_type_name());
			std::u16string result = value.to_json();
			if (result.empty())
			{
				// разобрать данный момент
				std::u16string out = u"{}";
				return out;
			}
			else
			{
				std::u16string out = u"{" + result + u"}";
				return out;
			}
		}
		else if constexpr(is_list<T>::value || is_map<T>::value)
		{
			using Value = typename T::value_type;
			std::u16string result = u"[";
			for (auto element : (value))
			{
				if (result != u"[") { result += u","; }
				result += Parser::to_json<Value>(element);
			}
			result += u"]";
			return result;
		}
		else if constexpr(is_pair<T>::value)
		{
			using Key = std::remove_const_t<typename T::first_type>;
			using Value = typename T::second_type;
			Key key = value.first;
			std::u16string result = u"[" + Parser::to_json<Key>(key) + u"," + Parser::to_json<Value>(value.second) + u"]";
			return result;
		}
		else if constexpr(std::is_enum<T>::value)
		{
			std::u16string out = u"\"" + CP866_to_UTF16(get_dictonary<T>().get_string(value)) + u"\"";
			return out;
		}
		else if constexpr(std::is_same<T, int>::value)
		{
			return to_u16string(value);
		}
		else if constexpr(std::is_same<T, std::u16string>::value)
		{
			return u"\"" + value + u"\"";
		}
		else if constexpr(std::is_same<T, std::string>::value)
		{
			return u"\"" + CP866_to_UTF16(value) + u"\"";
		}
		else if constexpr(std::is_same<T, game_object_size_t >::value)
		{
			std::u16string result = u"[" + to_json<int>(value.x) + u"," + to_json<int>(value.y) + u"," + to_json<int>(value.z) + u"]";
			return result;
		}
		else if constexpr(std::is_same<T, predicat_t>::value)
		{
			std::u16string out = to_u16string(value.index);
			return out;
		}
		else if constexpr(std::is_same<T, std::size_t>::value)
		{
			std::u16string out = to_u16string(value);
			return out;
		}
		else if constexpr(std::is_same<T, light_t>::value)
		{
			std::u16string result = u"[" + to_json<int>(value.R) + u"," + to_json<int>(value.G) + u"," + to_json<int>(value.B) + u"]";
			return result;
		}
		else if constexpr(std::is_same<T, float>::value)
		{
			std::u16string out = float_to_u16string(value);
			return out;
		}
		else if constexpr(std::is_same<T, optical_properties_t>::value)
		{
			std::u16string result = u"[" + to_json<float>(value.attenuation.R) + u"," + to_json<float>(value.attenuation.G) + u"," + to_json<float>(value.attenuation.B) + u"]";
			return result;
		}
		else if constexpr(std::is_same<T, AI_FOV>::value)
		{
			std::u16string result = u"[" + to_json<int>(value.radius) + u"," + to_json<std::size_t>(value.qualifier->index) + u"," + to_json<int>(value.start_angle) + u"," + to_json<int>(value.end_angle) + u"]";
			return result;
		}
		else static_assert(always_false<T>::value, "<to_json> for this type not found");
	};

	template<typename T> static void from_binary(const std::string& value, T& prop, std::size_t& pos)
	{
		if constexpr (std::is_pointer<T>::value)
		{
			if constexpr(std::is_base_of<iSerializable, std::remove_pointer_t<T>>::value)
			{
				unsigned int id = *reinterpret_cast<const unsigned int*>(&value[pos]);
				pos += sizeof(unsigned int);
				switch (id)
				{
				case 0:
				{
					prop = nullptr;
					break;
				}
				case 1:
				{
					std::size_t index = (*reinterpret_cast<const std::size_t*>(&value[pos])) - 2;
					pos += sizeof(std::size_t);
					prop = dynamic_cast<T>(m_items[index]);
					break;
				}
				default:
				{
					prop = dynamic_cast<T>(m_constructors_from_binary[id](value, pos));
					break;
				}
				}
			}
			else
			{
				char id = *reinterpret_cast<const char*>(&value[pos]);
				pos += sizeof(char);
				switch (id)
				{
				case 0:
				{
					prop = nullptr;
					break;
				}
				case 1:
				{
					using Type = typename std::remove_pointer_t<T>;
					T v = new Type();
					from_binary<Type>(value, *v, pos);
					prop = v;
					break;
				}
				}
			}
		}
		else if constexpr(is_list<T>::value)
		{
			using Value = typename T::value_type;
			std::size_t s = *reinterpret_cast<const std::size_t*>(&value[pos]);
			pos += sizeof(std::size_t);
			Value v;
			for (std::size_t i = 0; i < s; ++i)
			{
				from_binary<Value>(value, v, pos);
				prop.push_back(v);
			}
		}
		else if constexpr(is_map<T>::value)
		{
			using Key = typename T::key_type;
			using Value = typename T::mapped_type;
			std::size_t s = *reinterpret_cast<const std::size_t*>(&value[pos]);
			pos += sizeof(std::size_t);
			Key k;
			Value v;
			for (std::size_t i = 0; i < s; ++i)
			{
				from_binary<Key>(value, k, pos);
				from_binary<Value>(value, v, pos);
				prop[k] = v;
			}
		}
		else if constexpr(std::is_same<T, int>::value || std::is_same<T, float>::value || std::is_enum<T>::value)
		{
			prop = *reinterpret_cast<const T*>(&value[pos]);
			pos += sizeof(T);
		}
		else if constexpr(std::is_same<T, std::string>::value)
		{
			std::size_t s = *reinterpret_cast<const std::size_t*>(&value[pos]);
			pos += sizeof(std::size_t);
			prop = std::string(reinterpret_cast<const char*>(&value[pos]), s);
			pos += s;
		}
		else if constexpr(std::is_same<T, std::u16string>::value)
		{
			std::size_t s = *reinterpret_cast<const std::size_t*>(&value[pos]);
			pos += sizeof(std::size_t);
			prop = std::u16string(reinterpret_cast<const char16_t*>(&value[pos]), s);
			pos += s * 2;
		}
		else if constexpr(std::is_same<T, game_object_size_t>::value)
		{
			from_binary<int>(value, prop.x, pos);
			from_binary<int>(value, prop.y, pos);
			from_binary<int>(value, prop.z, pos);
		}
		else if constexpr(std::is_same<T, light_t>::value)
		{
			from_binary<int>(value, prop.R, pos);
			from_binary<int>(value, prop.G, pos);
			from_binary<int>(value, prop.B, pos);
		}
		else if constexpr(std::is_same<T, optical_properties_t>::value)
		{
			from_binary<float>(value, prop.attenuation.R, pos);
			from_binary<float>(value, prop.attenuation.G, pos);
			from_binary<float>(value, prop.attenuation.B, pos);
		}
	}

	template<typename T> static std::string to_binary(T& value)
	{
		LOG(INFO) << "to_binary";
		if constexpr(std::is_pointer<T>::value)
		{
			if constexpr(std::is_base_of<iSerializable, std::remove_pointer_t<T>>::value)
			{
				if (value)
				{
					LOG(INFO) << UTF16_to_CP866(value->get_packer().get_type_name());
					switch (value->m_serialization_index)
					{
					case 0:
					{
						LOG(INFO) << std::to_string(sizeof(unsigned int));
						m_object_index += 1;
						value->m_serialization_index = m_object_index;
						unsigned int id = value->get_packer().get_type_id();
						return std::string(reinterpret_cast<const char*>(&id), sizeof(unsigned int)) + value->to_binary();
					}
					default:
					{
						LOG(INFO) << std::to_string(sizeof(unsigned int) + sizeof(std::size_t));
						unsigned int id = 1;
						return std::string(reinterpret_cast<const char*>(&id), sizeof(unsigned int)) + std::string(reinterpret_cast<const char*>(&(value->m_serialization_index)), sizeof(std::size_t));
					}
					}
				}
				LOG(INFO) << std::to_string(sizeof(unsigned int));
				unsigned int id = 0;
				return std::string(reinterpret_cast<const char*>(&id), sizeof(unsigned int));
			}
			else
			{
				if (value == nullptr)
				{
					char s = 0;
					return std::string(reinterpret_cast<const char*>(&s), sizeof(char));
				}
				else
				{
					char s = 1;
					using Type = typename std::remove_pointer_t<T>;
					return std::string(reinterpret_cast<const char*>(&s), sizeof(char)) + Parser::to_binary<Type>(*value);
				}
			}
		}
		else if constexpr(std::is_same<T, int>::value || std::is_same<T, unsigned int>::value || std::is_same<T, float>::value || std::is_enum<T>::value)
		{
			LOG(INFO) << std::to_string(sizeof(T));
			return std::string(reinterpret_cast<const char*>(&value), sizeof(T));
		}
		else if constexpr(std::is_same<T, std::string>::value)
		{
			std::size_t s = value.size();
			return std::string(reinterpret_cast<const char*>(&s), sizeof(std::size_t)) + value;
		}
		else if constexpr(std::is_same<T, std::u16string>::value)
		{
			std::size_t s = value.size();
			return std::string(reinterpret_cast<const char*>(&s), sizeof(std::size_t)) + std::string(reinterpret_cast<const char*>(&(value[0])), value.size() * 2);
		}
		else if constexpr(is_list<T>::value || is_map<T>::value)
		{
			LOG(INFO) << std::to_string(sizeof(std::size_t));
			using Value = typename T::value_type;
			std::size_t s = value.size();
			std::string result = std::string(reinterpret_cast<const char*>(&s), sizeof(std::size_t));
			for (auto element : (value))
			{
				result += Parser::to_binary<Value>(element);
			}
			return result;
		}
		else if constexpr(is_pair<T>::value)
		{
			using Key = std::remove_const_t<typename T::first_type>;
			using Value = typename T::second_type;
			Key key = value.first;
			return Parser::to_binary<Key>(key) + Parser::to_binary<Value>(value.second);
		}
		else if constexpr(std::is_same<T, std::string>::value)
		{
			return value;
		}
		else if constexpr(std::is_same<T, game_object_size_t>::value)
		{
			return to_binary<int>(value.x) + to_binary<int>(value.y) + to_binary<int>(value.z);
		}
		else if constexpr(std::is_same<T, light_t>::value)
		{
			return to_binary<int>(value.R) + to_binary<int>(value.G) + to_binary<int>(value.B);
		}
		else if constexpr(std::is_same<T, optical_properties_t>::value)
		{
			return to_binary<float>(value.attenuation.R) + to_binary<float>(value.attenuation.G) + to_binary<float>(value.attenuation.B);
		}
		else
		{
			LOG(FATAL) << "<to_binary> type error: " <<typeid(value).name();
			//static_assert(always_false<T>::value, "<to_binary> type error");
			return "";
		}
	}

	template<typename T> constexpr static Dictonary<T>& get_dictonary()
	{
		if constexpr(std::is_same<T, object_tag_e>::value)
			return m_json_object_tag;
		else if constexpr(std::is_same<T, interaction_e>::value)
			return m_json_interaction_e;
		else if constexpr(std::is_same<T, body_part_e>::value)
			return m_json_body_part_e;
		else if constexpr(std::is_same<T, feature_list_type_e>::value)
			return m_json_feature_list_type_e;
		else if constexpr(std::is_same<T, entity_e>::value)
			return  m_json_entity_e;
		else if constexpr(std::is_same<T, ai_type_e>::value)
			return m_json_ai_type_e;
		else if constexpr(std::is_same<T, action_e::type>::value)
			return m_json_action_e;
		else if constexpr(std::is_same<T, object_state_e>::value)
			return m_json_object_state_e;
		else if constexpr(std::is_same<T, effect_e>::value)
			return m_json_effect_e;
		else static_assert(always_false<T>::value, "<get_dictonary> type error");
	}

	template<typename T, typename P> static void from_json(const std::u16string value, typename std::enable_if<!std::is_same<T, P>::value, T&>::type prop)
	{
		if constexpr(std::is_same<T, Icon*>::value&&std::is_same<P, from_InstanceDictonary_t>::value)
		{
			std::string&& name = UTF16_to_CP866(get_value(value));
			prop = GameObjectManager::m_config.m_icons.get_by_string(name);
		}
		else if constexpr(std::is_same<T, TileManager*>::value&&std::is_same<P, from_InstanceDictonary_t>::value)
		{
			if (get_value(value) == u"null")
			{
				prop = nullptr;
			}
			else
			{
				std::string&& result = UTF16_to_CP866(get_value(value));
				prop = Application::instance().m_game_object_manager->m_config.m_tile_managers.get_by_string(result);
			}
		}
		else if constexpr(std::is_same<T, std::vector<GLuint>>::value&&std::is_same<P, tile_t>::value)
		{
			std::u16string temp = value;
			scheme_list_t* s = read_array(temp);
			if (s)
			{
				prop.resize(s->size());
				int i = 0;
				for (auto element : (*s))
				{
					std::string&& name = UTF16_to_CP866(get_value(element));
					prop[i] = Application::instance().m_graph->png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\" + name + ".png");
					i += 1;
				}
				delete s;
			}
		}
		else if constexpr(std::is_same<T, InstanceDictonary<Icon*>>::value&&std::is_same<P, InstanceDictonary_t>::value)
		{
			std::u16string temp = value;
			scheme_list_t* s = read_array(temp);
			if (s)
			{
				for (auto element : (*s))
				{
					std::string&& name = UTF16_to_CP866(get_value(element));
					Icon* icon = new Icon;
					icon->m_value = Application::instance().m_graph->load_texture(FileSystem::instance().m_resource_path + "Tiles\\" + name + ".bmp");
					prop.add(icon, name);
				}
				delete s;
			}
		}
		else if constexpr(std::is_same<T, InstanceDictonary<TileManager*>>::value&&std::is_same<P, InstanceDictonary_t>::value)
		{
			std::u16string temp = value;
			scheme_list_t* s = read_array(temp);
			std::string k;
			TileManager* v = nullptr;
			if (s)
			{
				for (auto element : (*s))
				{
					scheme_vector_t* p = read_pair(element);
					from_json<std::string>((*p)[0], k);
					from_json<TileManager*>((*p)[1], v);
					prop.add(v, k);
				}
				delete s;
			}
		}
	}

	template<typename T, typename P> static void from_binary(const std::string& value, typename std::enable_if<!std::is_same<T, P>::value, T&>::type prop, std::size_t& pos)
	{
		if constexpr(std::is_same<T, TileManager*>::value&&std::is_same<P, from_InstanceDictonary_t>::value)
		{
			std::size_t s = *reinterpret_cast<const std::size_t*>(&value[pos]);
			pos += sizeof(std::size_t);
			if (s == 0)
			{
				prop = nullptr;
			}
			else
			{
				prop = Application::instance().m_game_object_manager->m_config.m_tile_managers.m_by_index[s - 1];
			}
		}
		else if constexpr(std::is_same<T, Icon*>::value&&std::is_same<P, from_InstanceDictonary_t>::value)
		{
			std::size_t s = *reinterpret_cast<const std::size_t*>(&value[pos]);
			pos += sizeof(std::size_t);
			if (s == 0)
			{
				prop = nullptr;
			}
			else
			{
				prop = Application::instance().m_game_object_manager->m_config.m_icons.m_by_index[s - 1];
			}
		}
	}

	template<typename T, typename P> static std::u16string to_json(typename std::enable_if<!std::is_same<T, P>::value, T&>::type value)
	{
		if constexpr(std::is_same<T, Icon*>::value&&std::is_same<P, from_InstanceDictonary_t>::value)
		{
			if (value)
			{
				return u"\"" + CP866_to_UTF16(value->m_json_string) + u"\"";

			}
			else
			{
				return u"null";
			}
		}
		else if constexpr(std::is_same<T, TileManager*>::value&&std::is_same<P, from_InstanceDictonary_t>::value)
		{
			if (value)
			{
				return u"\"" + CP866_to_UTF16(value->m_json_string) + u"\"";

			}
			else
			{
				return u"null";
			}
		}
		else if constexpr(std::is_same<T, std::vector<GLuint>>::value&&std::is_same<P, tile_t>::value)
		{
			return u"";
		}
		else if constexpr(std::is_same<T, InstanceDictonary<Icon*>>::value&&std::is_same<P, InstanceDictonary_t>::value)
		{
			return u"";
		}
		else if constexpr(std::is_same<T, InstanceDictonary<TileManager*>>::value&&std::is_same<P, InstanceDictonary_t>::value)
		{
			return u"";
		}
		return u"";
	}

	template<typename T, typename P> static std::string to_binary(typename std::enable_if<!std::is_same<T, P>::value, T&>::type value)
	{
		if constexpr(std::is_same<T, TileManager*>::value&&std::is_same<P, from_InstanceDictonary_t>::value)
		{
			std::size_t s;
			if (value) s = value->m_index + 1; else s = 0;
			return std::string(reinterpret_cast<const char*>(&s), sizeof(std::size_t));

		}
		else if constexpr(std::is_same<T, Icon*>::value&&std::is_same<P, from_InstanceDictonary_t>::value)
		{
			std::size_t s;
			if (value) s = value->m_index + 1; else s = 0;
			return std::string(reinterpret_cast<const char*>(&s), sizeof(std::size_t));
		}
		return "";
	}

};

#endif //DEFINITION_H
