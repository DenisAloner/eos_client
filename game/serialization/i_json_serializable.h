#ifndef I_JSON_SERIALIZABLE_H
#define I_JSON_SERIALIZABLE_H

#include <string>
#include <cassert>

#define VISIT(Type)                                     \
    virtual void visit(Type& value) { assert(false); }; \
    virtual void visit(Type* value) { assert(false); }

#define VISIT_OVERLOAD(Type)          \
    void visit(Type& value) override; \
    void visit(Type* value) override

class Object_state;
class AI;
class AI_enemy;
class AI_trap;
class Config;
class Instruction_arg_extract;
class Instruction_check_owner_type;
class Instruction_check_part_type;
class Instruction_game_owner;
class Instruction_game_owner;
struct dimension3_t;
struct optical_properties_t;
struct predicate_t;
struct gui_style_t;
struct atlas_tile_t;
class GameWorld;
class Action;
class GameMap;
class GameObject;
class Attribute_map;
class Parameter_list;
class Interaction_list;
class Action_list;
class MapCell;
class InventoryCell;
class ObjectPart;
class TileManager;
class Icon;
class Effect;
class Instruction_result;
class Instruction_check_tag;
class Instruction_get_owner;
class Instruction_slot_link;
class Instruction_slot_parameter;
class Interaction_addon;
class Interaction_copyist;
class Interaction_prefix;
namespace ObjectTag {
class Activator;
class Label;
class Equippable;
class Fast_move;
class Mortal;
class Purification_from_poison;
class Requirements_to_object;
class Can_transfer_object;
class Poison_resist;
}
class Interaction_time;
class Interaction_timer;
class Slot_set_state;
class Parameter;
class GraphicalController;
class Action_wrapper;

class iVisitor {
public:
    VISIT(int);
    VISIT(const int);
    VISIT(float);
    VISIT(std::u16string);
    VISIT(const std::u16string);
    VISIT(dimension3_t);
    VISIT(gui_style_t);
    VISIT(atlas_tile_t);
    VISIT(optical_properties_t);
    VISIT(Action);
    VISIT(GameWorld);
    VISIT(GameMap);
    VISIT(GameObject);
    VISIT(Attribute_map);
    VISIT(Object_state);
    VISIT(Interaction_list);
    VISIT(Parameter_list);
    VISIT(Action_list);
    VISIT(MapCell);
    VISIT(InventoryCell);
    VISIT(ObjectPart);
    VISIT(AI_enemy);
    VISIT(AI_trap);
    VISIT(predicate_t);
    VISIT(TileManager);
    VISIT(Icon);
    VISIT(Effect);
    VISIT(Instruction_arg_extract);
    VISIT(Instruction_check_owner_type);
    VISIT(Instruction_check_part_type);
    VISIT(Instruction_result);
    VISIT(Instruction_check_tag);
    VISIT(Instruction_game_owner);
    VISIT(Instruction_get_owner);
    VISIT(Instruction_slot_link);
    VISIT(Instruction_slot_parameter);
    VISIT(Interaction_addon);
    VISIT(Interaction_copyist);
    VISIT(Interaction_prefix);
    VISIT(ObjectTag::Activator);
    VISIT(ObjectTag::Label);
    VISIT(ObjectTag::Equippable);
    VISIT(ObjectTag::Fast_move);
    VISIT(ObjectTag::Mortal);
    VISIT(ObjectTag::Purification_from_poison);
    VISIT(ObjectTag::Requirements_to_object);
    VISIT(ObjectTag::Can_transfer_object);
    VISIT(ObjectTag::Poison_resist);
    VISIT(Interaction_time);
    VISIT(Interaction_timer);
    VISIT(Slot_set_state);
    VISIT(Parameter);
    VISIT(Config);
    VISIT(GraphicalController);
    VISIT(Action_wrapper);
};

#define IJSONSERIALIZABLE()                          \
    void accept_reference(iVisitor& value) override; \
    void accept_pointer(iVisitor& value) override;

#define IJSONSERIALIZABLE_IMPL(Class)                                     \
    void Class::accept_reference(iVisitor& value) { value.visit(*this); } \
    void Class::accept_pointer(iVisitor& value) { value.visit(this); }

class iJsonSerializable {
public:
    virtual ~iJsonSerializable() = default;
    virtual void accept_reference(iVisitor& value) = 0;
    virtual void accept_pointer(iVisitor& value) = 0;
};

#endif
