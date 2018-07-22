#include "Effect.h"
#include "Application.h"

// Interaction_list

Interaction_list::Interaction_list()
{
	m_interaction_message_type = interaction_message_type_e::list;
	m_list_type = feature_list_type_e::generic;
}

bool Interaction_list::update()
{
	return false;
}

Interaction_list* Interaction_list::clone()
{
	Interaction_list* result = new Interaction_list();
	result->m_list_type = m_list_type;
	Object_interaction* c;
	for (auto item = m_items.begin(); item != m_items.end(); ++item)
	{
		c = (*item)->clone();
		if (c) { result->m_items.push_back(c); }

	}
	return result;
}

void Interaction_list::apply_effect(GameObject* unit, Object_interaction* object)
{
	if (object)
	{
		switch (object->m_interaction_message_type)
		{
		case interaction_message_type_e::instruction_game_owner:
		{
			Instruction_game_owner* i = static_cast<Instruction_game_owner*>(object);
			for (auto item = m_items.begin(); item != m_items.end(); ++item)
			{
				i->m_result = false;
				(*item)->apply_effect(unit, object);
				if (!i->m_result)
				{
					return;
				}
			}
			i->m_result = true;
			break;
		}
		default:
		{
			for (auto item = m_items.begin(); item != m_items.end(); ++item)
			{
				(*item)->apply_effect(unit, object);
			}
			break;
		}
		}
	}
	else
	{
		for (auto item = m_items.begin(); item != m_items.end(); ++item)
		{
			(*item)->apply_effect(unit, object);
		}
	}
}

void Interaction_list::do_predicat(Visitor& helper)
{
	helper.visit(*this);
	for (auto item = m_items.begin(); item != m_items.end(); ++item)
	{
		(*item)->do_predicat(helper);
	}
}

void Interaction_list::apply_visitor(Visitor_generic& visitor)
{
	visitor.visit(*this);
}

void Interaction_list::reset_serialization_index()
{
	m_serialization_index = 0;
	for (auto& m_item : m_items)
	{
		if(m_item) m_item->reset_serialization_index();
	}
}

Packer_generic& Interaction_list::get_packer()
{
	return Packer<Interaction_list>::Instance();
}

// Patameter_list

Parameter_list::Parameter_list(interaction_e subtype) :m_subtype(subtype)
{
	m_list_type = feature_list_type_e::parameter;
};

Parameter_list::Parameter_list()
{
	m_list_type = feature_list_type_e::parameter;
};

//void Parameter_list::update_list(Object_interaction* list)
//{
//
//	switch (list->m_interaction_message_type)
//	{
//	case interaction_message_type_e::list:
//	{
//		Interaction_list* list_item = static_cast<Interaction_list*>(list);
//		for (auto current = list_item->m_items.begin(); current != list_item->m_items.end(); ++current)
//		{
//			update_list((*current));
//		}
//		break;
//	}
//	case interaction_message_type_e::slot_time:
//	{
//		LOG(INFO) << "buff in parameter";
//		Interaction_time* item = static_cast<Interaction_time*>(list);
//		update_list(item->m_value);
//		break;
//	}
//	default:
//	{
//		Effect* item;
//		item = static_cast<Effect*>(list);
//		switch (item->m_subtype)
//		{
//		case effect_e::value:
//		{
//			m_value = m_value + item->m_value;
//			break;
//		}
//		case effect_e::limit:
//		{
//			m_limit = m_limit + item->m_value;
//			break;
//		}
//		}
//	}
//	}
//
//}

Parameter_list::Update_visitor::Update_visitor(Parameter_list& owner): m_owner(owner) {}

void Parameter_list::Update_visitor::visit(Object_interaction& value)
{
	switch (value.m_interaction_message_type)
	{
	case interaction_message_type_e::effect:
		{
		Effect& item= static_cast<Effect&>(value);
		switch (item.m_subtype)
		{
		case effect_e::value:
		{
			m_owner.m_value += item.m_value;
			break;
		}
		case effect_e::limit:
		{
			m_owner.m_limit += item.m_value;
			break;
		}
		default:
			{
			break;
			}
		}
		break;
		}
	default:
	{
		break;
	}
	}

}

bool Parameter_list::update()
{
	////LOG(INFO) << "ÒÈÏ ÏÀÐÀÌÅÒÐÀ " << std::to_string((int)m_subtype);
	m_value = m_basic_value;
	m_limit = m_basic_limit;
	Update_visitor uh(*this);
	do_predicat(uh);
	return false;
}

Parameter_list* Parameter_list::clone()
{
	Parameter_list* result = new Parameter_list(m_subtype);
	result->m_list_type = m_list_type;
	result->m_basic_limit = m_basic_limit;
	result->m_basic_value = m_basic_value;
	result->m_value = m_value;
	result->m_limit = m_limit;
	for (auto item = m_items.begin(); item != m_items.end(); ++item)
	{
		result->add((*item)->clone());
	}
	return result;
}

void Parameter_list::apply_visitor(Visitor_generic& visitor)
{
	visitor.visit(*this);
}

Packer_generic& Parameter_list::get_packer()
{
	return Packer<Parameter_list>::Instance();
}

// Vision_list

Vision_list::Vision_list()
{
	m_list_type = feature_list_type_e::vision;
};

void Vision_list::update_list(Object_interaction* list)
{
	switch (list->m_interaction_message_type)
	{
	case interaction_message_type_e::list:
	{
		switch (static_cast<Interaction_list*>(list)->m_list_type)
		{
		case feature_list_type_e::vision_component:
		{
			Vision_component* list_item = static_cast<Vision_component*>(list);
			list_item->update();
			m_max_radius = max(m_max_radius, list_item->m_value.radius);
			break;
		}
		default:
		{
			Interaction_list* list_item = static_cast<Interaction_list*>(list);
			for (auto current = list_item->m_items.begin(); current != list_item->m_items.end(); ++current)
			{
				update_list((*current));
			}
			break;
		}
		}
		break;
	}
	}
}

bool Vision_list::update()
{
	//LOG(INFO) << "ÒÈÏ ÏÀÐÀÌÅÒÐÀ " << std::to_string((int)m_subtype);
	m_max_radius = 0;
	update_list(this);
	return false;
}

Vision_list* Vision_list::clone()
{
	Vision_list* result = new Vision_list;
	result->m_list_type = m_list_type;
	for (auto item = m_items.begin(); item != m_items.end(); ++item)
	{
		result->add((*item)->clone());
	}
	return result;
}

void Vision_list::apply_visitor(Visitor_generic& visitor)
{
	visitor.visit(*this);
}

void Vision_list::equip(Object_interaction* item)
{
	switch (item->m_interaction_message_type)
	{
	case interaction_message_type_e::list:
	{
		switch (static_cast<Interaction_list*>(item)->m_list_type)
		{
		case feature_list_type_e::vision_component:
		{
			m_items.push_back(item);
			break;
		}
		default:
		{
			Interaction_list* list_item = static_cast<Interaction_list*>(item);
			for (auto current = list_item->m_items.begin(); current != list_item->m_items.end(); ++current)
			{
				equip(*current);
			}
			break;
		}
		}
		break;
	}
	default:
	{
		for (auto current = m_items.begin(); current != m_items.end(); ++current)
		{
			static_cast<Vision_component*>(*current)->equip(item);
		}
		break;
	}
	}
}

void Vision_list::unequip(Object_interaction* item)
{
	switch (item->m_interaction_message_type)
	{
	case interaction_message_type_e::list:
	{
		switch (static_cast<Interaction_list*>(item)->m_list_type)
		{
		case feature_list_type_e::vision_component:
		{
			m_items.remove(item);
			break;
		}
		default:
		{
			Interaction_list* list_item = static_cast<Interaction_list*>(item);
			for (auto current = list_item->m_items.begin(); current != list_item->m_items.end(); ++current)
			{
				unequip(*current);
			}
			break;
		}
		}
		break;
	}
	default:
	{
		for (auto current = m_items.begin(); current != m_items.end(); ++current)
		{
			static_cast<Vision_component*>(*current)->unequip(item);
		}
		break;
	}
	}
}



Packer_generic& Vision_list::get_packer()
{
	return Packer<Vision_list>::Instance();
}


// Vision_component

Vision_component::Vision_component()
{
	m_list_type = feature_list_type_e::vision_component;
};

void Vision_component::update_list(Object_interaction* list)
{

	switch (list->m_interaction_message_type)
	{
	case interaction_message_type_e::list:
	{
		Interaction_list* list_item = static_cast<Interaction_list*>(list);
		for (auto current = list_item->m_items.begin(); current != list_item->m_items.end(); ++current)
		{
			update_list((*current));
		}
		break;
	}
	case interaction_message_type_e::slot_time:
	{
		LOG(INFO) << "buff in parameter";
		Interaction_time* item = static_cast<Interaction_time*>(list);
		update_list(item->m_value);
		break;
	}
	default:
	{
		Effect* item;
		item = static_cast<Effect*>(list);
		switch (item->m_subtype)
		{
		case effect_e::limit:
		{
			m_value.radius += item->m_value;
			break;
		}
		case effect_e::start_angle:
		{
			m_value.start_angle += item->m_value;
			break;
		}
		case effect_e::end_angle:
		{
			m_value.end_angle += item->m_value;
			break;
		}
		}
	}
	}
}

bool Vision_component::update()
{
	//LOG(INFO) << "ÒÈÏ ÏÀÐÀÌÅÒÐÀ " << std::to_string((int)m_subtype);
	m_value = m_basic_value;
	update_list(this);
	return false;
}

Vision_component* Vision_component::clone()
{
	Vision_component* result = new Vision_component();
	result->m_basic_value = AI_FOV(m_basic_value.radius, m_basic_value.qualifier, m_basic_value.start_angle, m_basic_value.end_angle);
	result->m_value = AI_FOV(m_value.radius, m_value.qualifier, m_value.start_angle, m_value.end_angle);
	for (auto item = m_items.begin(); item != m_items.end(); ++item)
	{
		result->add((*item)->clone());
	}
	return result;
}

void Vision_component::apply_visitor(Visitor_generic& visitor)
{
	visitor.visit(*this);
}

Packer_generic& Vision_component::get_packer()
{
	return Packer<Vision_component>::Instance();
}

// Tag_list

Tag_list::Tag_list()
{
	m_interaction_message_type = interaction_message_type_e::list;
	m_list_type = feature_list_type_e::tag;
}

void Tag_list::apply_visitor(Visitor_generic& visitor)
{
	visitor.visit(*this);
}

Tag_list* Tag_list::clone()
{
	Tag_list* result = new Tag_list();
	for (auto item = m_items.begin(); item != m_items.end(); ++item)
	{
		result->m_items.push_back((*item)->clone());
	}
	return result;
}

Tag_list::Update_visitor::Update_visitor():was_changed(false)
{
}

void Tag_list::Update_visitor::visit(Object_interaction& value)
{
	if (was_changed) { return; }
}

bool Tag_list::update()
{
	Update_visitor uh;
	do_predicat(uh);
	return uh.was_changed;
}

Packer_generic& Tag_list::get_packer()
{
	return Packer<Tag_list>::Instance();
}

// Parts_list

Parts_list::Parts_list()
{
	m_interaction_message_type = interaction_message_type_e::list;
	m_list_type = feature_list_type_e::parts;
}

Parts_list* Parts_list::clone()
{
	Parts_list* result = new Parts_list();
	for (auto item = m_items.begin(); item != m_items.end(); ++item)
	{
		result->m_items.push_back((*item)->clone());
	}
	return result;
}

//void Parts_list::update_list(Object_interaction* list)
//{
//
//	switch (list->m_interaction_message_type)
//	{
//	case interaction_message_type_e::list:
//	{
//		Interaction_list* list_item = static_cast<Interaction_list*>(list);
//		for (auto current = list_item->m_items.begin(); current != list_item->m_items.end(); ++current)
//		{
//			update_list((*current));
//		}
//		break;
//	}
//	case interaction_message_type_e::slot_time:
//	{
//		LOG(INFO) << "buff in parameter";
//		Interaction_time* item = static_cast<Interaction_time*>(list);
//		update_list(item->m_value);
//		break;
//	}
//	case interaction_message_type_e::part:
//	{
//		Object_part* part = static_cast<Object_part*>(list);
//		LOG(INFO) << part->m_name;
//		if (part->m_items)
//		{
//			LOG(INFO) << part->m_items->m_name;
//			ObjectTag::Equippable* tag_equippable = static_cast<ObjectTag::Equippable*>(part->m_items->get_tag(object_tag_e::equippable));
//			if (tag_equippable)
//			{
//				Instruction_game_owner* i = new Instruction_game_owner();
//				i->m_value = part;
//				tag_equippable->m_condition->apply_effect(nullptr, i);
//				if (!i->m_result)
//				{
//					GameObject* obj = part->m_items;
//					MapCell* cell = static_cast<MapCell*>(part->get_owner(entity_e::cell));
//					part->m_items = nullptr;
//					i->m_value = part;
//					tag_equippable->m_value->apply_effect(nullptr, i);
//					cell->m_map->add_object(obj, cell);
//					obj->m_owner = cell;
//					return;
//				};
//				
//				Object_tag* tag_requirements = part->m_object_state.get_tag(object_tag_e::requirements_to_object);
//				if (tag_requirements)
//				{
//					i->m_result = false;
//					i->m_value = part->m_items;
//					tag_requirements->apply_effect(nullptr, i);
//					if (!i->m_result) 
//					{ 
//						GameObject* obj = part->m_items;
//						MapCell* cell = static_cast<MapCell*>(part->get_owner(entity_e::cell));
//						part->m_items = nullptr;
//						i->m_value = part;
//						tag_equippable->m_value->apply_effect(nullptr, i);
//						cell->m_map->add_object(obj, cell);
//						obj->m_owner = cell;
//						return; 
//					};
//				}
//			}
//		}
//	}
//	}
//}

Parts_list::Update_visitor::Update_visitor(Parts_list& owner) : m_owner(owner),was_changed(false) {}

void Parts_list::Update_visitor::visit(Object_interaction& value)
{
	if (was_changed) { return; }
	switch (value.m_interaction_message_type)
	{
	case interaction_message_type_e::part:
		{
			Object_part& part = dynamic_cast<Object_part&>(value);
			if (part.m_item)
			{
				ObjectTag::Equippable* tag_equippable = static_cast<ObjectTag::Equippable*>(part.m_item->get_tag(object_tag_e::equippable));
				if (tag_equippable)
				{
					Instruction_game_owner* i = new Instruction_game_owner();
					i->m_value = &part;
					tag_equippable->m_condition->apply_effect(nullptr, i);
					if (!i->m_result)
					{
						GameObject* obj = part.m_item;
						MapCell* cell = static_cast<MapCell*>(part.get_owner(entity_e::cell));
						part.m_item = nullptr;
						i->m_value = &part;
						tag_equippable->m_value->apply_effect(nullptr, i);
						cell->m_map->add_object(obj, *cell);
						obj->m_owner = cell;
						was_changed = true;
						return;
					};

					Object_tag* tag_requirements = part.m_attributes.get_tag(object_tag_e::requirements_to_object);
					if (tag_requirements)
					{
						i->m_result = false;
						i->m_value = part.m_item;
						tag_requirements->apply_effect(nullptr, i);
						if (!i->m_result)
						{
							GameObject* obj = part.m_item;
							MapCell* cell = static_cast<MapCell*>(part.get_owner(entity_e::cell));
							part.m_item = nullptr;
							i->m_value = &part;
							Parameter* p = new Parameter(parameter_type_e::destination, obj, obj, cell, &part);
							Instruction_slot_parameter* ip = new Instruction_slot_parameter();
							ip->m_parameter = p;
							tag_equippable->m_value->apply_effect(nullptr, ip);
							cell->m_map->add_object(obj, *cell);
							obj->m_owner = cell;
							was_changed = true;
							return;
						};
					}
				}
			}
			break;
		}
	default:
	{
		break;
	}
	}

}

bool Parts_list::update()
{
	Update_visitor uh(*this);
	do_predicat(uh);
	//do
	//{
	//	uh.was_changed = false;
	//	do_predicat(uh);
	//} while (uh.was_changed);

	return uh.was_changed;
}

void Parts_list::equip(Object_interaction* item)
{
	LOG(INFO) << std::to_string((int)m_list_type); m_items.push_back(item); LOG(INFO) << std::to_string(m_items.size());

	for (auto current = m_items.begin(); current != m_items.end(); ++current)
	{
		//Object_part* a = dynamic_cast<Object_part*>(*current);
		LOG(INFO) <<std::to_string((int)((*current)->m_interaction_message_type));
		
	}
}

Packer_generic& Parts_list::get_packer()
{
	return Packer<Parts_list>::Instance();
};


void Parts_list::apply_visitor(Visitor_generic& visitor)
{
	visitor.visit(*this);
}

// Action_list

Action_list::Action_list() 
{
	m_interaction_message_type = interaction_message_type_e::list;
	m_list_type = feature_list_type_e::action;
};

Interaction_list* Action_list::clone()
{
	Action_list* result = new Action_list();
	for (auto item = m_items.begin(); item != m_items.end(); ++item)
	{
		result->m_items.push_back((*item)->clone());
	}
	return result;
}

void Action_list::apply_visitor(Visitor_generic& visitor)
{
	visitor.visit(*this);
}

Packer_generic& Action_list::get_packer()
{
	return Packer<Action_list>::Instance();
}


// Slot_set_state

Slot_set_state* Slot_set_state::clone()
{
	Slot_set_state* effect = new Slot_set_state();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_value = m_value;
	return effect;
}

void Slot_set_state::apply_effect(GameObject* unit, Object_interaction* object)
{
	unit->set_state(m_value);
}

Packer_generic& Slot_set_state::get_packer()
{
	return Packer<Slot_set_state>::Instance();
}

// Interaction_slot

Interaction_slot::Interaction_slot()
{
}

void Interaction_slot::do_predicat(Visitor& helper)
{
	helper.visit(*this);
	m_value->do_predicat(helper);
}

void Interaction_slot::reset_serialization_index()
{
	m_serialization_index = 0;
	if (m_value&&m_value->m_serialization_index != 0)
	{
		m_value->reset_serialization_index();
	}
}

// Interaction_copyist

Interaction_copyist::Interaction_copyist()
{
}

Object_interaction* Interaction_copyist::clone()
{
	LOG(INFO) << "clone";
	Interaction_copyist* effect = new Interaction_copyist();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_subtype = m_subtype;
	effect->m_value = m_value->clone();
	return effect;
}

void Interaction_copyist::apply_effect(GameObject* unit, Object_interaction* object)
{
	auto i = unit->get_effect(m_subtype);
	if (i)
	{
		m_value->apply_effect(unit, i);
	}
}

Packer_generic& Interaction_copyist::get_packer()
{
	return Packer<Interaction_copyist>::Instance();
}

// Interaction_prefix

Interaction_prefix::Interaction_prefix() {};

Interaction_prefix* Interaction_prefix::clone()
{
	Interaction_prefix* effect = new Interaction_prefix();
	effect->m_subtype = m_subtype;
	effect->m_value = m_value->clone();
	return effect;
}

void Interaction_prefix::apply_effect(GameObject* unit, Object_interaction* object)
{
	Tag_list* i = static_cast<Tag_list*>(unit->get_effect(interaction_e::tag));
	if (i)
	{
		for (auto item = i->m_items.begin(); item != i->m_items.end(); ++item)
		{
			(*item)->apply_effect(unit, this);
		}
	}
	// TODO ñäåëàòü âåðñèþ áåç ïðèìåíåíèÿ è ñ ïðèìåíåíèåì ìîäèôèöèðîâàííîãî ýôôåêòà
	/*if (m_value) { m_value->apply_effect(unit, object); }*/
}

Packer_generic& Interaction_prefix::get_packer()
{
	return Packer<Interaction_prefix>::Instance();
}

// Interaction_addon

Interaction_addon::Interaction_addon() {};

Object_interaction* Interaction_addon::clone()
{
	Interaction_addon* effect = new Interaction_addon();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_subtype = m_subtype;
	effect->m_value = m_value->clone();
	return effect;
}

void Interaction_addon::apply_effect(GameObject* unit, Object_interaction* object)
{
	unit->add_from(m_subtype, m_value->clone());
}

Packer_generic& Interaction_addon::get_packer()
{
	return Packer<Interaction_addon>::Instance();
}

// Interaction_time

Interaction_time::Interaction_time()
{

}

Object_interaction* Interaction_time::clone()
{
	Interaction_time* effect = new Interaction_time();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_turn = m_turn;
	effect->m_value = m_value->clone();
	return effect;
}

void Interaction_time::apply_effect(GameObject* unit, Object_interaction* object)
{
	m_value->apply_effect(unit, object);
}


Packer_generic& Interaction_time::get_packer()
{
	return Packer<Interaction_time>::Instance();
}

// Interaction_timer

Interaction_timer::Interaction_timer()
{
	m_interaction_message_type = interaction_message_type_e::slot_time;
}

Object_interaction* Interaction_timer::clone()
{
	Interaction_timer* effect = new Interaction_timer();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_turn = m_turn;
	effect->m_period = m_period;
	effect->m_value = m_value->clone();
	return effect;
}

void Interaction_timer::apply_effect(GameObject* unit, Object_interaction* object)
{
	if (m_turn == m_period - 1)
	{
		m_value->apply_effect(unit, object);
	}
}

Packer_generic& Interaction_timer::get_packer()
{
	return Packer<Interaction_timer>::Instance();
}


// Effect

Effect::Effect(): m_value(0), m_subtype()
{
	m_interaction_message_type = interaction_message_type_e::effect;
}

Effect* Effect::clone()
{
	Effect* effect = new Effect();
	effect->m_subtype = m_subtype;
	effect->m_value = m_value;
	return effect;
}

void Effect::apply_effect(GameObject* unit, Object_interaction* object)
{
	switch (m_subtype)
	{
	case effect_e::value:
	{
		Parameter_list* item = static_cast<Parameter_list*>(object);
		item->m_basic_value = item->m_basic_value + m_value;
		if (item->m_basic_value <=1)
		{ 
			item->m_basic_value = 0;
			Interaction_copyist* copyist = new Interaction_copyist();
			copyist->m_subtype = item->m_subtype;
			copyist->m_value = clone();
			Interaction_prefix* prefix = new Interaction_prefix();
			prefix->m_value = copyist;
			prefix->m_subtype = effect_prefix_e::parameter_change;
			prefix->apply_effect(unit, object);
		}
		break;
	}
	case effect_e::limit:
	{
		Parameter_list* item = static_cast<Parameter_list*>(object);
		item->m_basic_limit = item->m_basic_limit + m_value;
		break;
	}
	}
}

Packer_generic& Effect::get_packer()
{
	return Packer<Effect>::Instance();
}

// Object_tag

Object_tag::Object_tag(object_tag_e key) :m_type(key)
{
	m_interaction_message_type = interaction_message_type_e::tag;
};

// ObjectTag::Poison_resist

ObjectTag::Poison_resist::Poison_resist() :Object_tag(object_tag_e::poison_resist){};

ObjectTag::Poison_resist* ObjectTag::Poison_resist::clone()
{
	ObjectTag::Poison_resist* effect = new ObjectTag::Poison_resist();
	return effect;
}

void ObjectTag::Poison_resist::apply_effect(GameObject* unit, Object_interaction* object)
{
	Interaction_prefix* prefix = static_cast<Interaction_prefix*>(object);
	switch (prefix->m_subtype)
	{
	case effect_prefix_e::poison_damage:
	{
		static_cast<Effect*>(prefix->m_value)->m_value /= 2;
		break;
	}
	}
}

Packer_generic& ObjectTag::Poison_resist::get_packer()
{
	return Packer<Poison_resist>::Instance();
}

// ObjectTag::Mortal

ObjectTag::Mortal::Mortal() :Object_tag(object_tag_e::mortal) {};

ObjectTag::Mortal* ObjectTag::Mortal::clone()
{
	ObjectTag::Mortal* effect = new ObjectTag::Mortal();
	return effect;
}

void ObjectTag::Mortal::apply_effect(GameObject* unit, Object_interaction* object)
{
	Interaction_prefix* prefix = static_cast<Interaction_prefix*>(object);
	switch (prefix->m_subtype)
	{
	case effect_prefix_e::parameter_change:
	{
		Interaction_copyist* copyist = static_cast<Interaction_copyist*>(prefix->m_value);
		if (copyist->m_subtype == interaction_e::health)
		{
			auto feat = unit->get_parameter(interaction_e::health);
			if (feat)
			{
				if (feat->m_basic_value == 0)
				{
					unit->set_state(object_state_e::dead);
				}
			}
		}
		break;
	}
	}
}

// ObjectTag::Purification_from_poison

ObjectTag::Purification_from_poison::Purification_from_poison() :Object_tag(object_tag_e::purification_from_poison){};

ObjectTag::Purification_from_poison* ObjectTag::Purification_from_poison::clone()
{
	ObjectTag::Purification_from_poison* effect = new ObjectTag::Purification_from_poison();
	return effect;
}

void ObjectTag::Purification_from_poison::apply_effect(GameObject* unit, Object_interaction* object)
{
	Interaction_prefix* prefix = static_cast<Interaction_prefix*>(object);
	switch (prefix->m_subtype)
	{
	case effect_prefix_e::poison_damage:
	{
		Parameter_list* p = unit->get_parameter(interaction_e::poison);
			if (p)
			{
				Effect* e = static_cast<Effect*>(prefix->m_value);
				p->m_basic_value -= e->m_value;
				if (p->m_basic_value > p->m_basic_limit)
				{
					e->m_value = p->m_basic_limit - p->m_basic_value;
					p->m_basic_value = p->m_basic_limit;
				}
				else
				{
					e->m_value = 0;
				}
			}
		break;
	}
	}
}


Packer_generic& ObjectTag::Purification_from_poison::get_packer()
{
	return Packer<Purification_from_poison>::Instance();
}

// ObjectTag::Activator::Activator

ObjectTag::Activator::Activator() :Object_tag(object_tag_e::activator){};

ObjectTag::Activator* ObjectTag::Activator::clone()
{
	ObjectTag::Activator* effect = new ObjectTag::Activator();
	effect->m_link = m_link;
	return effect;
}

void ObjectTag::Activator::apply_effect(GameObject* unit, Object_interaction* object)
{
	Interaction_prefix* prefix = static_cast<Interaction_prefix*>(object);
	switch (prefix->m_subtype)
	{
	case effect_prefix_e::state_change:
	{
		Slot_set_state* e = static_cast<Slot_set_state*>(prefix->m_value);
		if (e)
		{
			for (auto l = m_link.begin(); l != m_link.end(); ++l)
			{
				(*l)->set_state(e->m_value);
			}
		}
		break;
	}
	}
}

void ObjectTag::Activator::reset_serialization_index()
{
	m_serialization_index = 0;
	for (auto item = m_link.begin(); item != m_link.end(); ++item)
	{
		if ((*item)->m_serialization_index > 0)
		{
			(*item)->reset_serialization_index();
		}
	}
}

Packer_generic& ObjectTag::Activator::get_packer()
{
	return Packer<Activator>::Instance();
}

// ObjectTag::Fast_move

ObjectTag::Fast_move::Fast_move() :Object_tag(object_tag_e::fast_move) {};

ObjectTag::Fast_move* ObjectTag::Fast_move::clone()
{
	ObjectTag::Fast_move* effect = new ObjectTag::Fast_move();
	return effect;
}

void ObjectTag::Fast_move::apply_effect(GameObject* unit, Object_interaction* object)
{
	Interaction_prefix* prefix = static_cast<Interaction_prefix*>(object);
	switch (prefix->m_subtype)
	{
	case effect_prefix_e::action:
	{
		Action_wrapper* e = static_cast<Action_wrapper*>(prefix->m_value);
		if (e)
		{
			if (e->m_action->m_kind == action_e::move)
			{
				if (e->m_decay != 1)
				{
					e->m_decay /= 2;
				}
			}
		}
		break;
	}
	}
}

Packer_generic& ObjectTag::Fast_move::get_packer()
{
	return Packer<Fast_move>::Instance();
}

// ObjectTag::Label

ObjectTag::Label::Label(object_tag_e type) :Object_tag(type){};

ObjectTag::Label* ObjectTag::Label::clone()
{
	ObjectTag::Label* effect = new ObjectTag::Label(m_type);
	return effect;
}

void ObjectTag::Label::apply_effect(GameObject* unit, Object_interaction* object){}

void ObjectTag::Label::apply_visitor(Visitor_generic& visitor)
{
	visitor.visit(*this);
};

Packer_generic& ObjectTag::Label::get_packer()
{
	return Packer<Label>::Instance();
}

Interaction_list* Effect_functions::create_feature_list(feature_list_type_e key, interaction_e name)
{
	Interaction_list* result;
	switch (key)
	{
	case feature_list_type_e::generic:
	{
		result = new Interaction_list();
		break;
	}
	case feature_list_type_e::tag:
	{
		result = new Tag_list();
		break;
	}
	case feature_list_type_e::action:
	{
		result = new Action_list();
		break;
	}
	case feature_list_type_e::parameter:
	{
		result = new Parameter_list(name);
		break;
	}
	case feature_list_type_e::parts:
	{
		result = new Parts_list();
		break;
	}
	case feature_list_type_e::vision:
	{
		result = new Vision_list();
		break;
	}
	case feature_list_type_e::vision_component:
	{
		result = new Vision_component();
		break;
	}
	}
	return result;
}

// Interaction_copyist

Instruction_slot_link::Instruction_slot_link()
{
	m_enable = false;
}


Instruction_slot_link* Instruction_slot_link::clone()
{
	Instruction_slot_link* effect = new Instruction_slot_link();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_subtype = m_subtype;
	effect->m_value = m_value->clone();
	effect->m_enable = m_enable;
	return effect;
}

void Instruction_slot_link::apply_effect(GameObject* unit, Object_interaction* object)
{
	switch (object->m_interaction_message_type)
	{
	case interaction_message_type_e::instruction_game_owner:
	{
		Instruction_game_owner* parameter = static_cast<Instruction_game_owner*>(object);
		Object_part* part = static_cast<Object_part*>(parameter->m_value);
		auto i = part->m_attributes.get_list(m_subtype);
		if (i)
		{
			if (m_enable)
			{
				i->remove(m_value);
				m_enable = false;
			}
			else
			{
				i->add(m_value);
				m_enable = true;
			}
		}
		break;
	}
	case interaction_message_type_e::instruction_result:
	{
		Instruction_result* ir = static_cast<Instruction_result*>(object);
		Parameter_argument_t& a = ir->m_value;
		switch (a.kind)
		{
		case type_e::gameobject:
		{
			auto i = a.m_object->m_active_state->get_list(m_subtype);
			if (i)
			{
				if (m_enable)
				{
					i->remove(m_value);
					m_enable = false;
				}
				else
				{
					i->add(m_value);
					m_enable = true;
				}
			}
			break;
		}
		case type_e::object_owner:
		{
			Interaction_list* i = nullptr;
			switch (a.m_owner->m_kind)
			{
			case entity_e::body_part:
			{
				i = static_cast<Object_part*>(a.m_owner)->m_attributes.get_list(m_subtype);
				break;
			}
			case entity_e::game_object:
			{
				i = static_cast<GameObject*>(a.m_owner)->m_active_state->get_list(m_subtype);
				break;
			}
			}
			if (i)
			{
				if (m_enable)
				{
					i->remove(m_value);
					m_enable = false;
				}
				else
				{
					i->add(m_value);
					m_enable = true;
				}
			}
			break;
		}
		}
		break;
	}
	default:
	{
		Instruction_slot_parameter* parameter = static_cast<Instruction_slot_parameter*>(object);
		Parameter& p(*(parameter->m_parameter));
		Object_part* part = static_cast<Object_part*>(p[1].m_object->m_owner);
		auto i = part->m_attributes.get_list(m_subtype);
		if (i)
		{
			if (m_enable)
			{
				i->remove(m_value);
				m_enable = false;
			}
			else
			{
				i->add(m_value);
				m_enable = true;
			}
		}
		break;
	}
	}
}

Packer_generic& Instruction_slot_link::get_packer()
{
	return Packer<Instruction_slot_link>::Instance();
}


// Instruction_slot_parameter

Instruction_slot_parameter::Instruction_slot_parameter()
{
	m_value = nullptr;
	m_interaction_message_type = interaction_message_type_e::instruction_slot_parameter;
}

Object_interaction* Instruction_slot_parameter::clone()
{
	Instruction_slot_parameter* effect = new Instruction_slot_parameter();
	effect->m_interaction_message_type = m_interaction_message_type;
	//effect->m_value = m_value->clone();
	return effect;
}

void Instruction_slot_parameter::apply_effect(GameObject* unit, Object_interaction* object)
{
	/*if (m_enable)
	{
		m_value->apply_effect(unit, object);
		m_enable = false;
	}
	else
	{
		Object_part* o = static_cast<Object_part*>(object);
		if (o->m_object_state.get_stat(m_subtype))
		{
			m_value->apply_effect(unit, object);
			m_enable = true;
		}
	}*/
}

Packer_generic& Instruction_slot_parameter::get_packer()
{
	return Packer<Instruction_slot_parameter>::Instance();
}

// ObjectTag::Equippable

ObjectTag::Equippable::Equippable() : Object_tag(object_tag_e::equippable)
{
	m_value = nullptr;
	m_condition = nullptr;
};

ObjectTag::Equippable* ObjectTag::Equippable::clone()
{
	ObjectTag::Equippable* result = new ObjectTag::Equippable;
	result->m_interaction_message_type = m_interaction_message_type;
	if(m_value){ result->m_value = m_value->clone(); }
	result->m_condition = m_condition->clone();
	return result;
}


void ObjectTag::Equippable::apply_effect(GameObject* unit, Object_interaction* object) 
{
	switch (object->m_interaction_message_type)
	{
	case interaction_message_type_e::instruction_slot_parameter:
	{
		
		Instruction_slot_parameter* parameter = static_cast<Instruction_slot_parameter*>(object);
		Parameter& p(*parameter->m_parameter);

		switch (p[2].m_owner->m_kind)
		{
		case entity_e::cell:
		{
			break;
		}
		case entity_e::inventory_cell:
		{
			break;
		}
		case entity_e::body_part:
		{
			Instruction_game_owner* i = new Instruction_game_owner();
			i->m_value = p[2].m_owner;
			m_condition->apply_effect(unit, i);
			if (!i->m_result) { return; };
			if (p[2].m_owner->m_kind == entity_e::body_part)
			{
				Object_part* part = static_cast<Object_part*>(p[2].m_owner);
				Object_tag* t = part->m_attributes.get_tag(object_tag_e::requirements_to_object);
				if (t)
				{
					i->m_result = false;
					i->m_value = p[1].m_object;
					t->apply_effect(unit, i);
					if (!i->m_result) { return; };
				}
			}
			break;
		}
		}

		switch (p[1].m_object->m_owner->m_kind)
		{
		case entity_e::cell:
		{
			//static_cast<MapCell*>(p->m_object->m_owner)->m_items.remove(p->m_object);
			static_cast<MapCell*>(p[1].m_object->m_owner)->m_map->remove_object(p[1].m_object);
			break;
		}
		case entity_e::inventory_cell:
		{
			static_cast<Inventory_cell*>(p[1].m_object->m_owner)->m_item = nullptr;
			break;
		}
		case entity_e::body_part:
		{
			Object_part* part = static_cast<Object_part*>(p[1].m_object->m_owner);
			part->m_item = nullptr;
			part = nullptr;
			m_value->apply_effect(unit, object);
			break;
		}
		}

		switch (p[2].m_owner->m_kind)
		{
		case entity_e::cell:
		{
			//static_cast<MapCell*>(p->m_owner)->m_items.push_back(p->m_object);
			static_cast<MapCell*>(p[2].m_owner)->m_map->add_object(p[1].m_object, *static_cast<MapCell*>(p[2].m_owner));
			p[1].m_object->m_owner = p[2].m_owner;
			break;
		}
		case entity_e::inventory_cell:
		{
			static_cast<Inventory_cell*>(p[2].m_owner)->m_item = p[1].m_object;
			p[1].m_object->m_owner = p[2].m_owner;
			break;
		}
		case entity_e::body_part:
		{
			Object_part* part = static_cast<Object_part*>(p[2].m_owner);
			part->m_item = p[1].m_object;
			p[1].m_object->m_owner = part;
			if (m_value) { m_value->apply_effect(unit, object); }
			break;
		}
		}
		break;
	}
	}
};

void ObjectTag::Equippable::reset_serialization_index()
{

	m_serialization_index = 0;
	if(m_value&&m_value->m_serialization_index!=0)
	{
		m_value->reset_serialization_index();
	}
	if (m_condition&&m_condition->m_serialization_index != 0)
	{
		m_condition->reset_serialization_index();
	}
}

Packer_generic& ObjectTag::Equippable::get_packer()
{
	return Packer<Equippable>::Instance();
}


// ObjectTag::Requirements_to_object

ObjectTag::Requirements_to_object::Requirements_to_object() : Object_tag(object_tag_e::requirements_to_object) 
{
	m_value = nullptr;
	m_result = false;
};

ObjectTag::Requirements_to_object* ObjectTag::Requirements_to_object::clone()
{
	ObjectTag::Requirements_to_object* result = new ObjectTag::Requirements_to_object;
	result->m_interaction_message_type = m_interaction_message_type;
	result->m_value = m_value->clone();
	result->m_result = m_result;
	return result;
}

void ObjectTag::Requirements_to_object::apply_effect(GameObject* unit, Object_interaction* object)
{
	switch (object->m_interaction_message_type)
	{
	case interaction_message_type_e::instruction_game_owner:
	{
		m_value->apply_effect(unit, object);
		Instruction_game_owner* i = static_cast<Instruction_game_owner*>(object);
		m_result = i->m_result;
		break;
	}
	default:
	{
		break;
	}
	}
}

void ObjectTag::Requirements_to_object::apply_visitor(Visitor_generic& visitor)
{
	visitor.visit(*this);
};

void ObjectTag::Requirements_to_object::reset_serialization_index()
{
	m_serialization_index = 0;
	if (m_value)
	{
		m_value->reset_serialization_index();
	}
}

Packer_generic& ObjectTag::Requirements_to_object::get_packer()
{
	return Packer<Requirements_to_object>::Instance();
}

// ObjectTag::Can_transfer_object

ObjectTag::Can_transfer_object::Can_transfer_object() : Object_tag(object_tag_e::can_transfer_object)
{
	m_value = nullptr;
	m_result = false;
};

ObjectTag::Can_transfer_object* ObjectTag::Can_transfer_object::clone()
{
	ObjectTag::Can_transfer_object* result = new ObjectTag::Can_transfer_object;
	result->m_interaction_message_type = m_interaction_message_type;
	result->m_value = m_value->clone();
	result->m_result = m_result;
	return result;
}

void ObjectTag::Can_transfer_object::apply_effect(GameObject* unit, Object_interaction* object)
{
	switch (object->m_interaction_message_type)
	{
	case interaction_message_type_e::instruction_slot_parameter:
	{
		m_value->apply_effect(unit, object);
		/*Instruction_game_owner* i = static_cast<Instruction_game_owner*>(object);
		m_result = i->m_result;*/
		break;
	}
	default:
	{
		break;
	}
	}
}

void ObjectTag::Can_transfer_object::reset_serialization_index()
{
	m_serialization_index = 0;
	if (m_value&&m_value->m_serialization_index != 0)
	{
		m_value->reset_serialization_index();
	}
};

Packer_generic& ObjectTag::Can_transfer_object::get_packer()
{
	return Packer<Can_transfer_object>::Instance();
}

// Instruction_game_owner

Instruction_game_owner::Instruction_game_owner() 
{
	m_value = nullptr;
	m_result = false;
	m_interaction_message_type = interaction_message_type_e::instruction_game_owner;
};

// Instruction_check_tag

Packer_generic& Instruction_game_owner::get_packer()
{
	return Packer<Instruction_game_owner>::Instance();
}

Instruction_check_tag::Instruction_check_tag()
{
};

Instruction_check_tag* Instruction_check_tag::clone()
{
	Instruction_check_tag* result = new Instruction_check_tag();
	result->m_interaction_message_type = m_interaction_message_type;
	result->m_value = m_value;
	return result;
}

void Instruction_check_tag::apply_effect(GameObject* unit, Object_interaction* object)
{
	switch (object->m_interaction_message_type)
	{
		
	case interaction_message_type_e::instruction_game_owner:
	{
		Instruction_game_owner* i = static_cast<Instruction_game_owner*>(object);
		switch (i->m_value->m_kind)
		{
		case entity_e::game_object:
		{
			GameObject* obj = static_cast<GameObject*>(i->m_value);
			i->m_result = obj->get_stat(m_value);
			break;
		}
		}
		break;
	}
	}
}

void Instruction_check_tag::apply_visitor(Visitor_generic& visitor)
{
	visitor.visit(*this);
}

Packer_generic& Instruction_check_tag::get_packer()
{
	return Packer<Instruction_check_tag>::Instance();
};

// Instruction_check_part_type

Instruction_check_part_type::Instruction_check_part_type()
{
};

Instruction_check_part_type* Instruction_check_part_type::clone()
{
	Instruction_check_part_type* result = new Instruction_check_part_type();
	result->m_interaction_message_type = m_interaction_message_type;
	result->m_value = m_value;
	return result;
}

void Instruction_check_part_type::apply_effect(GameObject* unit, Object_interaction* object)
{
	switch (object->m_interaction_message_type)
	{
	case interaction_message_type_e::instruction_slot_parameter:
	{
		break;
	}
	default:
	{
		Instruction_game_owner* i = static_cast<Instruction_game_owner*>(object);
		Object_part* op = static_cast<Object_part*>(i->m_value);
		i->m_result = (op->m_part_kind == m_value);
		break;
	}
	}
}

Packer_generic& Instruction_check_part_type::get_packer()
{
	return Packer<Instruction_check_part_type>::Instance();
};

// Instruction_arg_extract

Instruction_arg_extract::Instruction_arg_extract()
{
	m_value = nullptr;
	m_index = 0;
	m_result = false;
};

Instruction_arg_extract* Instruction_arg_extract::clone()
{
	Instruction_arg_extract* result = new Instruction_arg_extract();
	result->m_interaction_message_type = m_interaction_message_type;
	result->m_index = m_index;
	result->m_value = m_value->clone();
	result->m_result = m_result;
	return result;
}

void Instruction_arg_extract::apply_effect(GameObject* unit, Object_interaction* object)
{
	switch (object->m_interaction_message_type)
	{
	case interaction_message_type_e::instruction_slot_parameter:
	{
		Instruction_slot_parameter* p = static_cast<Instruction_slot_parameter*>(object);
		Parameter_argument_t& a = p->m_parameter->operator[](m_index);
		switch (a.kind)
		{
		case type_e::gameobject:
			{
			Instruction_result* i = new Instruction_result();
			i->m_value = Parameter_argument_t(type_e::gameobject);
			i->m_value.set(a.m_object);
			m_value->apply_effect(unit, i);
			p->m_result = i->m_result;
			break;
			}
		case type_e::object_part:
		{
			Instruction_result* i = new Instruction_result();
			i->m_value = Parameter_argument_t(type_e::object_part);
			i->m_value.set(a.m_part);
			m_value->apply_effect(unit, i);
			p->m_result = i->m_result;
			break;
		}
		}
		break;
	}
	default:
	{
		break;
	}
	}
};

void Instruction_arg_extract::reset_serialization_index()
{
	m_serialization_index = 0;
	if (m_value&&m_value->m_serialization_index != 0)
	{
		m_value->reset_serialization_index();
	}
}

Packer_generic& Instruction_arg_extract::get_packer()
{
	return Packer<Instruction_arg_extract>::Instance();
}

// Instruction_get_owner

Instruction_get_owner::Instruction_get_owner()
{
	m_value = nullptr;
	m_result = false;
};

Instruction_get_owner* Instruction_get_owner::clone()
{
	Instruction_get_owner* result = new Instruction_get_owner();
	result->m_value = m_value->clone();
	result->m_result = m_result;
	return result;
}

void Instruction_get_owner::apply_effect(GameObject* unit, Object_interaction* object)
{
	switch (object->m_interaction_message_type)
	{
	case interaction_message_type_e::instruction_result:
	{
		Instruction_result* ir = static_cast<Instruction_result*>(object);
		Parameter_argument_t& a = ir->m_value;
		switch(a.kind)
		{
		case type_e::gameobject:
		{
			Instruction_result* i = new Instruction_result();
			i->m_value = Parameter_argument_t(type_e::object_owner);
			i->m_value.set(a.m_object->m_owner);
			m_value->apply_effect(unit, i);
			ir->m_result = i->m_result;
			break;
		}
		case type_e::object_part:
		{
			Instruction_result* i = new Instruction_result();
			i->m_value = Parameter_argument_t(type_e::object_owner);
			i->m_value.set(a.m_part->m_owner);
			m_value->apply_effect(unit, i);
			ir->m_result = i->m_result;
			break;
		}
		}
		break;
	}
	default:
	{
		break;
	}
	}
};

void Instruction_get_owner::reset_serialization_index()
{
	m_serialization_index = 0;
	if (m_value&&m_value->m_serialization_index != 0)
	{
		m_value->reset_serialization_index();
	}
}

// Instruction_get_owner_top

Packer_generic& Instruction_get_owner::get_packer()
{
	return Packer<Instruction_get_owner>::Instance();
}

Instruction_get_owner_top::Instruction_get_owner_top()
{
	m_value = nullptr;
	m_result = false;
};

Instruction_get_owner_top* Instruction_get_owner_top::clone()
{
	Instruction_get_owner_top* result = new Instruction_get_owner_top();
	result->m_value = m_value->clone();
	result->m_result = m_result;
	return result;
}

void Instruction_get_owner_top::apply_effect(GameObject* unit, Object_interaction* object)
{
	switch (object->m_interaction_message_type)
	{
	case interaction_message_type_e::instruction_result:
	{
		Instruction_result* ir = static_cast<Instruction_result*>(object);
		Parameter_argument_t& a = ir->m_value;
		switch (a.kind)
		{
		case type_e::gameobject:
		{
			Instruction_result* i = new Instruction_result();
			i->m_value = Parameter_argument_t(type_e::object_owner);
			i->m_value.set(a.m_object->get_owner(entity_e::game_object));
			m_value->apply_effect(unit, i);
			ir->m_result = i->m_result;
			break;
		}
		case type_e::object_part:
		{
			Instruction_result* i = new Instruction_result();
			i->m_value = Parameter_argument_t(type_e::object_owner);
			i->m_value.set(a.m_part->get_owner(entity_e::game_object));
			m_value->apply_effect(unit, i);
			ir->m_result = i->m_result;
			break;
		}
		case type_e::object_owner:
		{
			Instruction_result* i = new Instruction_result();
			i->m_value = Parameter_argument_t(type_e::object_owner);
			i->m_value.set(a.m_part->get_owner(entity_e::game_object));
			m_value->apply_effect(unit, i);
			ir->m_result = i->m_result;
			break;
		}
		}
		break;
	}
	default:
	{
		break;
	}
	}
}

Packer_generic& Instruction_get_owner_top::get_packer()
{
	return Packer<Instruction_get_owner_top>::Instance();
};

// Instruction_check_owner_type

Instruction_check_owner_type::Instruction_check_owner_type()
{
};

Instruction_check_owner_type* Instruction_check_owner_type::clone()
{
	Instruction_check_owner_type* result = new Instruction_check_owner_type();
	result->m_interaction_message_type = m_interaction_message_type;
	result->m_value = m_value;
	return result;
}

void Instruction_check_owner_type::apply_effect(GameObject* unit, Object_interaction* object)
{
	switch (object->m_interaction_message_type)
	{
	case interaction_message_type_e::instruction_result:
	{
		Parameter_argument_t& a = static_cast<Instruction_result*>(object)->m_value;
		switch (a.kind)
		{
		case type_e::object_owner:
		{
			static_cast<Instruction_result*>(object)->m_result = (a.m_owner->m_kind == m_value);
			break;
		}
		}
		break;
	}
	default:
	{
		break;
	}
	}
}

Packer_generic& Instruction_check_owner_type::get_packer()
{
	return Packer<Instruction_check_owner_type>::Instance();
};

void ObjectTag::Mortal::apply_visitor(Visitor_generic& visitor)
{
	visitor.visit(*this);
}

Packer_generic& ObjectTag::Mortal::get_packer()
{
	return Packer<Mortal>::Instance();
}
