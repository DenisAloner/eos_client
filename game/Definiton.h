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
	view_t():l(0), r(0), u(0), d(0) {}
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
		none=-1,
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
	clockwise=1
	
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

	game_object_size_t(int x, int y,int z) : x(x), y(y),z(z) {}
	game_object_size_t() : x(0), y(0),z(0) {}
};

struct RGBA_t
{
	float R, G, B, A;

	RGBA_t(float R, float G, float B, float A) : R(R), G(G), B(B), A(A){};

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
	RGB_t(float R, float G, float B) : R(R), G(G), B(B){};
	RGB_t() : R(0.0), G(0.0), B(0.0){};
};

typedef std::map<std::u16string, std::u16string> scheme_map_t;
typedef std::list<std::u16string> scheme_list_t;
typedef std::vector<std::u16string> scheme_vector_t;

struct optical_properties_t
{
	RGB_t attenuation;
	optical_properties_t(RGB_t attenuation) : attenuation(attenuation){};
    optical_properties_t(): attenuation(RGB_t(1.0,1.0,1.0)){};
	//optical_properties_t(const optical_properties_t& value) :seethrough(value.seethrough),attenuation(value.attenuation){};
};

struct light_t
{
	int R, G, B;

	light_t(int R, int G, int B) : R(R), G(G), B(B){};
	light_t() : R(0), G(0), B(0){};
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
		float l = sqrtf(x*x + y*y);
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

	void add(T enm,std::string json_string,std::string description_string)
	{
		m_by_string[json_string]=enm;
		m_by_enum[enm] = json_string;
	}

	std::string get_string(T key)
	{
		auto value = m_by_enum.find(key);
		if (value == m_by_enum.end())
		{
			LOG(FATAL) << "Ёлемент отсутствует";
		}
		return value->second;
	}

	T get_enum(const std::string& key)
	{
		auto value = m_by_string.find(key);
		if (value == m_by_string.end())
		{
			LOG(FATAL) << "Ёлемент отсутствует";
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
	return std::tuple_cat(a,b);
}

template<typename T, typename P>
void property_from_json(T* object, scheme_map_t& data, typename std::enable_if<std::is_same<typename P::Type, typename P::PropType>::value, P&>::type property)
{
	
	Parser::from_json<typename P::Type>(data[property.name], (*object.*(property.member)));
}

template<typename T, typename P>
void property_from_json(T* object, scheme_map_t& data, typename std::enable_if<!std::is_same<typename P::Type, typename P::PropType>::value, P&>::type property)
{

	Parser::from_json<typename P::Type, P::PropType>(data[property.name], (*object.*(property.member)));
}

template<std::size_t iteration, typename T>
std::enable_if_t<(iteration > 0)> set_property(T* object, scheme_map_t& data) {
	constexpr auto property = std::get<iteration>(T::properties());
	using Type = typename decltype(property);
	property_from_json<T,Type>(object, data, property);
	set_property<iteration - 1, T>(object, data);
}

template<std::size_t iteration, typename T>
std::enable_if_t<(iteration == 0)> set_property(T* object, scheme_map_t& data) {
	constexpr auto property = std::get<iteration>(T::properties());
	using Type = typename decltype(property);
	property_from_json<T,Type>(object, data,property);
}

template<typename T>
void object_from_json(T* object, scheme_map_t* value) {
	set_property<std::tuple_size<decltype(T::properties())>::value - 1, T>(object, *value);
}

template<typename T,typename P>
std::u16string property_to_json(T* object,typename std::enable_if<std::is_same<typename P::Type,typename P::PropType>::value,P&>::type property) {
	
	return u"\"" + std::u16string(property.name) + u"\":" + Parser::to_json<typename P::Type>(*object.*(property.member));
}

template<typename T, typename P>
std::u16string property_to_json(T* object, typename std::enable_if<!std::is_same<typename P::Type, typename P::PropType>::value, P&>::type property) {
	return u"";
}

template<std::size_t iteration, typename T>
std::enable_if_t<(iteration > 0), std::u16string> get_property(T* object) {
	constexpr auto property = std::get<iteration>(T::properties());
	using Type = typename decltype(property);
	return property_to_json<T,Type>(object,property) + u"," + get_property<iteration - 1, T>(object);
}

template<std::size_t iteration, typename T>
std::enable_if_t<(iteration == 0), std::u16string> get_property(T* object) {
	constexpr auto property = std::get<iteration>(T::properties());
	using Type = typename decltype(property);
	return property_to_json<T, Type>(object, property);
}

template<typename T>
std::u16string object_to_json(T* object) {
	return get_property<std::tuple_size<decltype(T::properties())>::value - 1, T>(object);
}

class Packer_generic
{
public:

	virtual void from_json(iSerializable* object, scheme_map_t* value) = 0;
	virtual std::u16string to_json(iSerializable* object) = 0;
	virtual std::u16string get_type() = 0;
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
};

template<typename T> iSerializable* create_instance(scheme_map_t* value)
{
	T* out = new T;
	out->get_packer().from_json(out, value);
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

	std::u16string m_class_name;

	std::u16string get_type() override
	{
		return m_class_name;
	}

	void from_json(iSerializable* object, scheme_map_t* value) override
	{
		object_from_json<T>(dynamic_cast<T*>(object), value);
	}

	std::u16string to_json(iSerializable* object) override
	{

		return object_to_json<T>(dynamic_cast<T*>(object));
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

	std::u16string m_class_name;

	std::u16string get_type() override
	{
		return m_class_name;
	}

	void from_json(iSerializable* object, scheme_map_t* value) override
	{
	}

	std::u16string to_json(iSerializable* object) override;

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

	Tree(){};

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

class Object_interaction : public virtual iSerializable
{
public:

	typedef std::function<void(Object_interaction*)> predicat;
	typedef std::function<void(Object_interaction*, bool)> predicat_ex;

	interaction_message_type_e m_interaction_message_type;
	std::u16string m_namename;
	Object_interaction() { m_namename = u"base"; };
	virtual void apply_effect(GameObject* unit, Object_interaction* object) {};
	virtual bool on_turn() { return false; };
	virtual std::u16string get_description() { return u""; };
	virtual Object_interaction* clone() { return nullptr; };
	virtual void description(std::list<std::u16string>* info, int level) {};
	virtual void do_predicat(Visitor& helper) { helper.visit(*this); };
	virtual void apply_visitor(Visitor_generic& visitor);

	Packer_generic& get_packer() override
	{
		return Packer<Object_interaction>::Instance();
	}

	constexpr static auto properties() { return std::make_tuple(make_property(&Object_interaction::m_namename, u"m_value")); }

};

typedef iSerializable* (*instance_function_t)(scheme_map_t*);

class Parser
{
private:
	static std::size_t m_object_index;

public:

	struct icon_t {};
	struct icon_map_t {};
	struct tilemanager_ref_t {};

	static std::wstring_convert<std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>, wchar_t> m_convert;
	static std::map<std::u16string, instance_function_t> m_classes;

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

	static std::u16string serialize_object(iSerializable* value);
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
	static void register_class(std::u16string class_name, instance_function_t function);


	template<typename T>
	static void register_class(std::u16string class_name)
	{
		m_classes[class_name] = create_instance<T>;
		Packer<T>::Instance().m_class_name = class_name;
	}

	template<typename T>
	static void register_class(std::u16string class_name,instance_function_t function)
	{
		m_classes[class_name] = function;
		Packer<T>::Instance().m_class_name = class_name;
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

	template<typename T> static void from_json(const std::u16string value, typename std::enable_if<is_list<T>::value, T& >::type prop)
	{
		using Value = typename T::value_type;
		std::u16string temp = value;
		scheme_list_t* s = read_array(temp);
		if (s)
		{
			for (auto element : (*s))
			{
				Value v;
				from_json<Value>(element,v);
				prop.push_back(v);
			}
		}
		delete s;
	};

	/*template<typename T> static void from_json(const std::u16string value, typename std::enable_if<is_pair<T>::value, T& >::type prop)
	{
		LOG(INFO) << "ѕј–ј";
		using Key = typename T::first_type;
		using Value = typename T::second_type;
		std::u16string temp = value;
		scheme_vector_t* s = read_pair(temp);
		std::remove_const_t<Key> key;
		from_json<std::remove_const_t<Key>>((*s)[0], key);
		prop.first=key;
		from_json<Value>((*s)[1], prop.second);
	};*/

	template<typename T> static void from_json(const std::u16string value, typename std::enable_if<is_map<T>::value, T& >::type prop)
	{
		using Key = typename T::key_type;
		using Value = typename T::mapped_type;
		std::u16string temp = value;
		scheme_list_t* s = read_array(temp);
		Key k;
		Value v;
		if(s)
		{
			for (auto element : (*s))
			{
				scheme_vector_t* p = read_pair(element);
				from_json<Key>((*p)[0],k);
				from_json<Value>((*p)[1], v);
				prop[k]=v;
				delete p;
			}
			delete s;
		}
	};


	template<typename T> static void from_json(const std::u16string value, typename std::enable_if<std::is_pointer<T>::value&&std::is_base_of<iSerializable, std::remove_pointer_t<T>>::value, T& >::type prop)
	{
		std::u16string temp(value);
		scheme_map_t* s = read_object(temp);
		if (s)
		{
			prop = dynamic_cast<T>(m_classes[get_value((*s)[u"$type"])](s));
			LOG(INFO) << UTF16_to_CP866(prop->get_packer().get_type());
			delete s;
			return;
		}
		prop = nullptr;
	}

	template<typename T> static void from_json(const std::u16string value, typename std::enable_if<std::is_pointer<T>::value && !std::is_base_of<iSerializable, std::remove_pointer_t<T>>::value, T& >::type prop);

	template<typename T> static void from_json(const std::u16string value, typename std::enable_if<!std::is_pointer<T>::value&&!std::is_enum<T>::value && !is_pair<T>::value && !is_list<T>::value && !is_map<T>::value && !std::is_base_of<iSerializable, std::remove_pointer_t<T>>::value, T& >::type prop);


	template<typename T> static void from_json(const std::u16string value, typename std::enable_if<!std::is_pointer<T>::value&&std::is_base_of<iSerializable, std::remove_pointer_t<T>>::value, T&>::type prop)
	{
		std::u16string temp(value);
		scheme_map_t* s = read_object(temp);
		prop.get_packer().from_json(&prop, s);
		delete s;
	};

	template<typename T> static std::u16string to_json(typename std::enable_if< !std::is_enum<T>::value && !std::is_pointer<T>::value && !is_list<T>::value && !is_map<T>::value && !is_pair<T>::value && !std::is_base_of<iSerializable, std::remove_pointer_t<T>>::value, T >::type value);

	template<typename T> static std::u16string to_json(typename std::enable_if<std::is_pointer<T>::value&&std::is_base_of<iSerializable, std::remove_pointer_t<T>>::value, T>::type value)
	{
		if (value)
		{
			if (value->get_packer().is_singleton())
			{
				LOG(INFO) << UTF16_to_CP866(value->get_packer().get_type());
				std::u16string result = value->get_packer().to_json(value);
				if (result.empty())
				{
					std::u16string out = u"{\"$type\":\"" + value->get_packer().get_type() + u"\"}";
					return out;
				}
				else
				{
					std::u16string out = u"{\"$type\":\"" + value->get_packer().get_type() + u"\"," + result + u"}";
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
					LOG(INFO) << UTF16_to_CP866(value->get_packer().get_type());
					std::u16string result = value->get_packer().to_json(value);
					if (result.empty())
					{
						std::u16string out = u"{\"$type\":\"" + value->get_packer().get_type() + u"\",\"$link_id\":" + to_u16string(value->m_serialization_index) + u"}";
						return out;
					}
					else
					{
						std::u16string out = u"{\"$type\":\"" + value->get_packer().get_type() + u"\",\"$link_id\":" + to_u16string(value->m_serialization_index) + u"," + result + u"}";
						return out;
					}
					break;
				}
				default:
				{
					std::u16string out = u"{\"$type\":\"link\",\"value\":" + to_u16string(value->m_serialization_index) + u"}";
					return out;

					break;
				}
				}
			}
		}
		else
		{
			return u"null";
		}
	}

	template<typename T> static std::u16string to_json(typename std::enable_if<!std::is_pointer<T>::value&&std::is_base_of<iSerializable, T>::value, T>::type value)
	{
		return serialize_object(&value);
	}

	template<typename T> static std::u16string to_json(typename std::enable_if< std::is_pointer<T>::value && !is_list<T>::value && !is_map<T>::value && !is_pair<T>::value && !std::is_base_of<iSerializable, std::remove_pointer_t<T>>::value, T >::type value)
	{
		if (value == nullptr)
		{
			return u"null";
		}
		else
		{
			using Type = typename std::remove_pointer_t<T>&;
			return Parser::to_json<Type>(*value);
		}
	}

	template<typename T> static std::u16string to_json(typename std::enable_if< is_list<T>::value || is_map<T>::value, T >::type value)
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
	};

	template<typename T> static std::u16string to_json(typename std::enable_if< is_pair<T>::value, T >::type value)
	{
		using Key = typename T::first_type;
		using Value = typename T::second_type;
		std::u16string result = u"[" + Parser::to_json<std::remove_const_t<Key>>(value.first) + u"," + Parser::to_json<Value>(value.second) + u"]";
		return result;
	};

	template<typename T> static Dictonary<T>& get_dictonary();
    
	template<> static Dictonary<object_tag_e>& get_dictonary<object_tag_e>()
	{
		return m_json_object_tag;
	}

	template<> static Dictonary<interaction_e>& get_dictonary<interaction_e>()
	{
		return m_json_interaction_e;
	}

	template<> static Dictonary<body_part_e>& get_dictonary<body_part_e>()
	{
		return m_json_body_part_e;
	}

	template<> static Dictonary<feature_list_type_e>& get_dictonary<feature_list_type_e>()
	{
		return m_json_feature_list_type_e;
	}

	template<> static Dictonary<entity_e>& get_dictonary<entity_e>()
	{
		return m_json_entity_e;
	}

	template<> static Dictonary<ai_type_e>& get_dictonary<ai_type_e>()
	{
		return m_json_ai_type_e;
	}

	template<> static Dictonary<action_e::type>& get_dictonary<action_e::type>()
	{
		return m_json_action_e;
	}

	template<> static Dictonary<object_state_e>& get_dictonary<object_state_e>()
	{
		return m_json_object_state_e;
	}

	template<> static Dictonary<effect_e>& get_dictonary<effect_e>()
	{
		return m_json_effect_e;
	}

	template<typename T> static inline void from_json(const std::u16string value, typename std::enable_if<std::is_enum<T>::value, T&>::type prop) 
	{
		std::size_t start_pos = value.find(u'"');
		if (start_pos != std::string::npos)
		{
			std::size_t end_pos = value.find(u'"', start_pos + 1);
			if (end_pos != std::string::npos)
			{
				prop=get_dictonary<T>().get_enum(UTF16_to_CP866(value.substr(start_pos + 1, end_pos - start_pos - 1)));
				return;
			}
		}
		prop=static_cast<T>(0);
	}

	template<typename T> static inline std::u16string to_json(typename std::enable_if<std::is_enum<T>::value, T>::type value)
	{
		std::u16string out = u"\"" + CP866_to_UTF16(get_dictonary<T>().get_string(value)) + u"\"";
		return out;
	}

	template<typename T,typename P> static void from_json(const std::u16string value, typename std::enable_if<!std::is_same<T,P>::value, T&>::type prop);
	template<typename T, typename P> static std::u16string to_json(typename std::enable_if<!std::is_same<T, P>::value, T&>::type value);

};

#endif //DEFINITION_H
