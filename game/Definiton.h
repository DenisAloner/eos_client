#ifndef DEFINITION_H
#define	DEFINITION_H

#include <functional>
#include "graphics/gl.hpp"
#include <list>

const float Pi = 3.14159265F;
const float cos22_5 = cos(22.5F*Pi / 180.0);
const float cos67_5 = cos(67.5F*Pi / 180.0);

class GameObject;

class GUI_Object;

const int tile_size_x = 32;
const int tile_size_y = 18;
const int tile_size_x_half = 16;
const int tile_size_y_half = 9;

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

enum entity_e
{
	cell,
	inventory_cell,
	body_part
};

enum action_e
{
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
	shoot
};

enum class object_state_e
{ 
	alive,
	dead,
	on,
	off,
	equip,
	growth_01,
	growth_02,
	growth_03,
	growth_04,
	growth_05,
	growth_06
};

enum ParameterKind
{
	ParameterKind_Simple,
	ParameterKind_Position,
	ParameterKind_MapCell,
	ParameterKind_MoveObjectByUnit,
	ParameterKind_Cancel,
	ParameterKind_Destination,
	parameter_kind_object,
	parameter_kind_owner,
	parameter_kind_unit_interaction,
	parameter_interaction_cell,
	parameter_accept,
	parameter_bow_shoot,
	parameter_direction,
	parameter_game_quit,
	parameter_new_game,
	parameter_load_game
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

class iSerializable
{
public:

	size_t m_serialization_index;

	virtual void reset_serialization_index() { m_serialization_index = 0; };

	virtual void save() = 0;
	virtual void load() = 0;
};

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
	slot_time
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
	vision_component
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
	head,
	hand,
	foot
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
	fast_move
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


class Object_interaction: public virtual iSerializable
{
public:

	typedef std::function<void(Object_interaction*)> predicat;

	interaction_message_type_e m_interaction_message_type;
	Object_interaction(){};
	virtual void apply_effect(GameObject* unit, Object_interaction* object){};
	virtual bool on_turn() = 0;
	virtual std::string get_description() = 0;
	virtual Object_interaction* clone() = 0;
	virtual void description(std::list<std::string>* info, int level) = 0;
	virtual void do_predicat(predicat func){ func(this); };
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
	parameter_mapcell,
	parameter_position,
	parameter_movegameobject,
	parameter_destination,
	parameter_gameobject,
	parameter_object_owner,
	parameter_unit_interaction,
	parameter_interaction_cell,
	parameter_direction,
	action_wrapper,
	parameter_bow_shoot,
	vision_list,
	vision_component,
};

struct predicat_t
{
	std::function<bool(GameObject*)> predicat;
	size_t index;

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

#endif //DEFINITION_H
