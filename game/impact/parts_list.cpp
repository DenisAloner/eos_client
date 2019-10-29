#include "parts_list.h"
#include "GameObject.h"

Parts_list::Parts_list()
{
    m_list_type = feature_list_type_e::parts;
}

Parts_list* Parts_list::clone()
{
	auto result = new Parts_list();
    for (auto& m_item : m_items)
    {
        result->m_items.push_back(m_item->clone());
    }
    return result;
}

//void Parts_list::update_list(Object_interaction* list)
//{
//
//	switch (list->get_interaction_message_type)
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
//	case interaction_message_type_e::interaction_timer:
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

Parts_list::Update_visitor::Update_visitor(Parts_list& owner)
    : m_owner(owner)
    , was_changed(false)
{
}

void Parts_list::Update_visitor::visit(Object_interaction& value)
{
    if (was_changed) {
        return;
    }
    switch (value.get_interaction_message_type()) {
    case interaction_message_type_e::part: {
	    auto& part = dynamic_cast<Object_part&>(value);
        if (part.m_item) {
	        auto tag_equippable = static_cast<ObjectTag::Equippable*>(part.m_item->get_tag(object_tag_e::equippable));
            if (tag_equippable) {
                auto i = new Instruction_game_owner();
                i->m_value = &part;
                tag_equippable->m_condition->apply_effect(nullptr, i);
                if (!i->m_result) {
	                auto obj = part.m_item;
	                auto cell = static_cast<MapCell*>(part.get_owner(entity_e::cell));
                    part.m_item = nullptr;
                    i->m_value = &part;
                    tag_equippable->m_value->apply_effect(nullptr, i);
                    cell->m_map->add_object(obj, *cell);
                    obj->m_owner = cell;
                    was_changed = true;
                    return;
                };

                auto tag_requirements = part.m_attributes.get_tag(object_tag_e::requirements_to_object);
                if (tag_requirements) {
                    i->m_result = false;
                    i->m_value = part.m_item;
                    tag_requirements->apply_effect(nullptr, i);
                    if (!i->m_result) {
	                    auto obj = part.m_item;
	                    auto cell = static_cast<MapCell*>(part.get_owner(entity_e::cell));
                        part.m_item = nullptr;
                        i->m_value = &part;
	                    auto p = new Parameter(parameter_type_e::destination, obj, obj, cell, &part);
	                    auto ip = new Instruction_slot_parameter();
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
    default: {
        break;
    }
    }
}

bool Parts_list::update()
{
    Update_visitor uh(*this);
    do_predicate(uh);
    //do
    //{
    //	uh.was_changed = false;
    //	do_predicate(uh);
    //} while (uh.was_changed);

    return uh.was_changed;
}

void Parts_list::equip(Object_interaction* item)
{
    m_items.push_back(item);

    //for (auto current = m_items.begin(); current != m_items.end(); ++current)
    //{
    //	//Object_part* a = dynamic_cast<Object_part*>(*current);
    //	LOG(INFO) <<std::to_string((int)((*current)->get_interaction_message_type));
    //
    //}
}

iPacker& Parts_list::get_packer()
{
    return Packer<Parts_list>::instance();
};

void Parts_list::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
}