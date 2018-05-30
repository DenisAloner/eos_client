#ifndef DEFINITION_CPP
#define	DEFINITION_CPP

#include "Definiton.h"
#include "Application.h"

object_direction_e operator+(object_direction_e lhs, const rotate_direction_e& rhs)
{
	int result = static_cast<int>(lhs) + static_cast<int>(rhs);
	switch (result)
	{
	case -1: return object_direction_e::downright;
	case 8: return object_direction_e::down;
	default: return static_cast<object_direction_e>(result);
	}
}

void Parser_context::reset()
{
	m_items.clear();
	m_object_index = 0;
}


Apply_info::Apply_info(GameObject* unit, Object_interaction* object):m_unit(unit),m_object(object)
{
}

void Object_interaction::apply_visitor(Visitor_generic& visitor)
{
}

Packer_generic& Object_interaction::get_packer()
{
	return Packer<Object_interaction>::Instance();
}

iSerializable* Packer<Object_interaction>::from_json(scheme_map_t* value,Parser_context& context)
{
	//LOG(INFO) << Parser::UTF16_to_CP1251(Parser::get_value((*value)[u"value"])) << " :: " << std::to_string(GameObjectManager::m_config.m_templates.size());
	return GameObjectManager::m_config->m_templates[Parser::UTF16_to_CP1251(Parser::get_value((*value)[u"value"]))]->clone();
}


std::u16string Packer<MapCell>::to_json(iSerializable* value, Parser_context& context)
{
	if (value)
	{
		MapCell& obj = *dynamic_cast<MapCell*>(value);
		std::u16string out = u"{\"$type\":\"" + value->get_packer().get_type_name() + u"\",\"value\":[" + Parser::
			int_to_u16string(obj.x) + u"," + Parser::int_to_u16string(obj.y) + u"]}";
		return out;
	}
	else
	{
		std::u16string out = u"null";
		return out;
	}
}

std::string Packer<MapCell>::to_binary(iSerializable* value, Parser_context& context)
{
	if (value)
	{
		MapCell& obj = *dynamic_cast<MapCell*>(value);
		std::size_t id = value->get_packer().get_type_id() + 1;
		return std::string(reinterpret_cast<const char*>(&id), sizeof(std::size_t)) + Parser::to_binary<int>(obj.x,context) + Parser::to_binary<int>(obj.y,context);
	}
	else
	{
		std::size_t id = 0;
		return std::string(reinterpret_cast<const char*>(&id), sizeof(std::size_t));
	}
}

iSerializable* Packer<MapCell>::from_json(scheme_map_t* value, Parser_context& context)
{
	scheme_vector_t* s = Parser::read_pair((*value)[u"value"]);
	int x;
	int y;
	Parser::from_json<int>((*s)[0], x,context);
	Parser::from_json<int>((*s)[1], y,context);
	return &context.m_game_world.m_maps.front()->get(y, x);
}

iSerializable* Packer<MapCell>::from_binary(const std::string& value, std::size_t& pos,Parser_context& context)
{
	int x;
	int y;
	Parser::from_binary<int>(value, x,pos,context);
	Parser::from_binary<int>(value, y, pos,context);
	return &context.m_game_world.m_maps.front()->get(y, x);
}

iSerializable* Packer<Parser::Link>::from_json(scheme_map_t* value, Parser_context& context)
{
	std::size_t id;
	Parser::from_json<std::size_t>((*value)[u"value"], id,context);
	return context.m_items[id - 1];
}

iSerializable* Packer<Parser::Link>::from_binary(const std::string& value, std::size_t& pos, Parser_context& context)
{
	std::size_t id;
	Parser::from_binary<std::size_t>(value, id, pos,context);
	return context.m_items[id - 1];
}

std::u16string Packer<GameObject>::to_json(iSerializable* value, Parser_context& context)
{
	std::u16string out = u"{\"$type\":\"" + value->get_packer().get_type_name() + u"\",\"value\":" + Parser::int_to_u16string(value->m_serialization_index) + u"}";
	return out;
}

std::string Packer<GameObject>::to_binary(iSerializable* value, Parser_context& context)
{
	std::size_t id = value->get_packer().get_type_id() + 1;
	std::size_t index = value->m_serialization_index;
	return std::string(reinterpret_cast<const char*>(&id), sizeof(std::size_t)) + std::string(reinterpret_cast<const char*>(&index), sizeof(std::size_t));
}

iSerializable* Packer<GameObject>::from_json(scheme_map_t* value, Parser_context& context)
{
	LOG(INFO) << "iSerializable* Packer<GameObject>::from_json(scheme_map_t* value)";
	const int i = Parser::to_int32((*value)[u"value"]);
	const auto it = std::next(context.m_game_world.m_object_manager.m_items.begin(), i);
	return &(*it);
}

iSerializable* Packer<GameObject>::from_binary(const std::string& value, std::size_t& pos, Parser_context& context)
{
	std::size_t i;
	Parser::from_binary<std::size_t>(value, i, pos,context);
	const auto it = std::next(context.m_game_world.m_object_manager.m_items.begin(), i);
	return &(*it);
}

Dictonary<object_tag_e> Parser::m_json_object_tag = {
	{ object_tag_e::poison_resist,"poison_resist" },
	{ object_tag_e::purification_from_poison,"purification_from_poison" },
	{ object_tag_e::mortal,"mortal" },
	{ object_tag_e::pick_able,"pick_able" },
	{ object_tag_e::pass_able,"pass_able" },
	{ object_tag_e::seethrough_able,"seethrough_able" },
	{ object_tag_e::activator,"activator" },
	{ object_tag_e::fast_move,"fast_move" },
	{ object_tag_e::equippable,"equippable" },
	{ object_tag_e::ring,"ring" },
	{ object_tag_e::requirements_to_object,"requirements_to_object" },
	{ object_tag_e::cursed,"cursed" },
	{ object_tag_e::can_transfer_object,"can_transfer_object" },
	{ object_tag_e::footwear,"footwear" }
};

Dictonary<body_part_e> Parser::m_json_body_part_e = {
	{ body_part_e::mouth,"mouth" },
	{ body_part_e::wrist,"wrist" },
	{ body_part_e::finger,"finger" },
	{ body_part_e::head,"head" },
	{ body_part_e::hand,"hand" },
	{ body_part_e::foot,"foot" },
	{ body_part_e::waist,"waist" },
	{ body_part_e::container,"container" }
};

Dictonary<ai_type_e> Parser::m_json_ai_type_e = {
	{ ai_type_e::non_humanoid,"non_humanoid" },
	{ ai_type_e::trap,"trap" }
};

Dictonary<interaction_e> Parser::m_json_interaction_e = {
	{ interaction_e::total_damage, "total_damage" },
	{ interaction_e::damage, "damage" },
	{ interaction_e::buff, "buff" },
	{ interaction_e::use, "use" },
	{ interaction_e::health, "health" },
	{ interaction_e::strength, "strength" },
	{ interaction_e::intelligence, "intelligence" },
	{ interaction_e::dexterity, "dexterity" },
	{ interaction_e::hunger, "hunger" },
	{ interaction_e::thirst, "thirst" },
	{ interaction_e::poison, "poison" },
	{ interaction_e::action, "action" },
	{ interaction_e::tag, "tag" },
	{ interaction_e::body, "body" },
	{ interaction_e::weapon_damage, "weapon_damage" },
	{ interaction_e::skill_sword, "skill_sword" },
	{ interaction_e::skill_bow, "skill_bow" },
	{ interaction_e::strength_bonus, "strength_bonus" },
	{ interaction_e::demand_weapon_skill, "demand_weapon_skill" },
	{ interaction_e::evasion_skill, "evasion_skill" },
	{ interaction_e::weapon_range, "weapon_range" },
	{ interaction_e::vision, "vision" },
	{ interaction_e::vision_component, "vision_component" },
	{ interaction_e::skill_unarmed_combat, "skill_unarmed_combat" },
	{ interaction_e::equip, "equip" }
};

Dictonary<feature_list_type_e> Parser::m_json_feature_list_type_e = {
	{ feature_list_type_e::generic,"generic" },
	{ feature_list_type_e::parameter,"parameter" },
	{ feature_list_type_e::tag,"tag" },
	{ feature_list_type_e::action,"action", },
	{ feature_list_type_e::parts,"parts" },
	{ feature_list_type_e::vision,"vision" },
	{ feature_list_type_e::vision_component,"vision_component" }
};

Dictonary<entity_e> Parser::m_json_entity_e = {
	{ entity_e::game_object,"gameobject" },
	{ entity_e::body_part,"object_part" },
	{ entity_e::cell,"cell" }
};

Dictonary<action_e::type> Parser::m_json_action_e = {
	{action_e::move,"move"},
	{action_e::move_step,"move_step"},
	{action_e::push,"push"},
	{action_e::turn,"turn"},
	{action_e::open_inventory,"open_inventory"},
	{action_e::cell_info,"cell_info"},
	{action_e::set_motion_path,"set_motion_path"},
	{action_e::pick,"pick"},
	{action_e::open,"open"},
	{action_e::hit,"hit"},
	{action_e::hit_melee,"hit_melee"},
	{action_e::equip,"equip"},
	{action_e::show_parameters,"how_parameters"},
	{action_e::use,"use"},
	{action_e::save,"save"},
	{action_e::autoexplore,"autoexplore"},
	{action_e::shoot,"shoot"},
	{action_e::load,"load"},
	{action_e::move_out,"move_out" }
};

Dictonary<object_state_e> Parser::m_json_object_state_e = {
	{ object_state_e::alive,"alive" },
	{ object_state_e::dead, "dead" },
	{ object_state_e::on, "on" },
	{ object_state_e::off, "off" },
	{ object_state_e::equip, "equip" }
};

Dictonary<effect_e> Parser::m_json_effect_e = {
	{ effect_e::value,"value" },
	{ effect_e::limit,"limit" },
	{ effect_e::start_angle,"start_angle" },
	{ effect_e::end_angle,"end_angle" }
};

std::unordered_map<effect_e, std::u16string>  Parser::m_string_effect_e = {
	{ effect_e::value,u"модификатор значения" },
	{ effect_e::limit,u"модификатор лимита" },
	{ effect_e::start_angle, u"модификатор начального угла обзора" },
	{ effect_e::end_angle, u"модификатор конечного угла обзора" }
};


std::unordered_map<interaction_e, std::u16string>  Parser::m_string_interaction_e = {
	{ interaction_e::total_damage, u"общий дополнительный урон" },
	{ interaction_e::damage, u"урон" },
	{ interaction_e::buff, u"баффы" },
	{ interaction_e::use, u"применение" },
	{ interaction_e::health, u"здоровье" },
	{ interaction_e::strength, u"сила" },
	{ interaction_e::intelligence, u"интеллект" },
	{ interaction_e::dexterity, u"ловкость" },
	{ interaction_e::hunger,  u"голод" },
	{ interaction_e::thirst,  u"жажда" },
	{ interaction_e::poison,  u"яд" },
	{ interaction_e::action,  u"действия" },
	{ interaction_e::tag,  u"метки" },
	{ interaction_e::body,  u"тело" },
	{ interaction_e::weapon_damage,  u"урон оружия" },
	{ interaction_e::skill_sword,  u"владение мечом" },
	{ interaction_e::skill_bow,  u"владение луком" },
	{ interaction_e::strength_bonus,  u"бонус силы" },
	{ interaction_e::demand_weapon_skill,  u"требование к владению оружием" },
	{ interaction_e::evasion_skill, u"навык уклонения" },
	{ interaction_e::weapon_range,  u"дальность" },
	{ interaction_e::vision,  u"зрение" },
	{ interaction_e::vision_component,  u"поле зрения" },
	{ interaction_e::skill_unarmed_combat,  u"владение безоружным боем" },
	{ interaction_e::equip,  u"экипировка" }
};

std::unordered_map<object_tag_e, std::u16string>  Parser::m_string_object_tag_e = {
	{ object_tag_e::poison_resist , u"сопротивление к яду" },
	{ object_tag_e::purification_from_poison , u"очищение от яда" },
	{ object_tag_e::mortal , u"смертное существо" },
	{ object_tag_e::pass_able , u"не является преградой" },
	{ object_tag_e::pick_able , u"можно взять" },
	{ object_tag_e::seethrough_able , u"не загораживает обзор" },
	{ object_tag_e::activator , u"активирует/деактивирует механизмы" },
	{ object_tag_e::fast_move , u"быстрое передвижение" },
	{ object_tag_e::equippable , u"можно одеть" },
	{ object_tag_e::ring , u"кольцо" },
	{ object_tag_e::requirements_to_object , u"требования к предмету" },
	{ object_tag_e::cursed , u"наложено проклятье" },
	{ object_tag_e::can_transfer_object , u"может перекладывать предметы" },
	{ object_tag_e::footwear , u"обувь" }
};

Parser::Parser()
{
	/*m_constructors_from_json[u"A_class"] = A::create_instance2;
	m_constructors_from_json[u"B_class"] = B::create_instance2;*/
}

Parser::~Parser()
{
}

//std::u16string Parser::serialize_object(iSerializable* value)
//{
//	if (value)
//	{
//		LOG(INFO) << UTF16_to_CP1251((*value).get_packer().get_type_name());
//		std::u16string result = value->get_packer().to_json(value);
//		if (result.empty())
//		{
//			std::u16string out = u"{\"$type\":\"" + value->get_packer().get_type_name() + u"\"}";
//			return out;
//		}
//		else
//		{
//			std::u16string out = u"{\"$type\":\"" + value->get_packer().get_type_name() + u"\"," + result + u"}";
//			return out;
//		}
//	}
//	else
//	{
//		return u"null";
//	}
//}

scheme_map_t* Parser::read_object(std::u16string& value)
{
	const char16_t* opening_symbol = nullptr;
	int count = 0;
	for (int i = 0; i < value.size(); ++i)
	{
		switch (value[i])
		{
		case u'{':
		{
			if (opening_symbol == nullptr)
			{
				opening_symbol = &value[i];
			}
			count += 1;
			break;
		}
		case u'}':
		{
			if (opening_symbol != nullptr)
			{
				count = count - 1;
				if (count == 0)
				{
					std::u16string sub_value(opening_symbol + 1, &value[i] - opening_symbol - 1);
					//LOG(INFO) << "read_object -> " << UTF16_to_CP1251(sub_value);
					return parse_object(sub_value);
				}
			}
			break;
		}
		default:;
		}
	}
	return nullptr;
}

scheme_list_t* Parser::read_array(std::u16string& value)
{
	const char16_t* opening_symbol = nullptr;
	int count = 0;
	for (int i = 0; i < value.size(); ++i)
	{
		switch (value[i])
		{
		case u'[':
		{
			if (opening_symbol == nullptr)
			{
				opening_symbol = &value[i];
			}
			count += 1;
			break;
		}
		case u']':
		{
			if (opening_symbol != nullptr)
			{
				count = count - 1;
				if (count == 0)
				{
					std::u16string sub_value(opening_symbol + 1, &value[i] - opening_symbol - 1);
					return parse_array(sub_value);
				}
			}
			break;
		}
		default:;
		}
	}
	return nullptr;
}

scheme_vector_t* Parser::read_pair(std::u16string& value)
{
	const char16_t* opening_symbol = nullptr;
	int count = 0;
	for (int i = 0; i < value.size(); ++i)
	{
		switch (value[i])
		{
		case u'[':
		{
			if (opening_symbol == nullptr)
			{
				opening_symbol = &value[i];
			}
			count += 1;
			break;
		}
		case u']':
		{
			if (opening_symbol != nullptr)
			{
				count = count - 1;
				if (count == 0)
				{
					std::u16string sub_value(opening_symbol + 1, &value[i] - opening_symbol - 1);
					return parse_pair(sub_value);
				}
			}
			break;
		}
		default:;
		}
	}
	return nullptr;
}

std::u16string* get_token(std::u16string value, std::size_t& i)
{
	std::size_t opening_pos = i;
	std::size_t count = 0;
	bool expr = false;
	for (i; i < value.size(); ++i)
	{
		switch (value[i])
		{
		case u'{':
		{
			if (expr)
			{
				if (value[opening_pos] == u'{')
				{
					count += 1;
				}
			}
			else
			{
				opening_pos = i;
				count += 1;
				expr = true;
			}
			break;
		}
		case u'}':
		{
			if (value[opening_pos] == u'{')
			{
				count -= 1;
				if (count == 0)
				{
					std::size_t pos = value.find(u',', i);
					std::u16string* result = new std::u16string(value.substr(opening_pos, i - opening_pos + 1));
					if (pos != std::string::npos)
					{
						i = pos;
					}
					else
					{
						i = value.size() - 1;
					}
					return result;
				}
			}
			break;
		}
		case u'[':
		{
			if (expr)
			{
				if (value[opening_pos] == u'[')
				{
					count += 1;
				}
			}
			else
			{
				opening_pos = i;
				count += 1;
				expr = true;
			}
			break;
		}
		case u']':
		{
			if (value[opening_pos] == u'[')
			{
				count -= 1;
				if (count == 0)
				{
					std::size_t pos = value.find(u',', i);
					std::u16string* result = new std::u16string(value.substr(opening_pos, i - opening_pos + 1));
					if (pos != std::string::npos)
					{
						i = pos;
					}
					else
					{
						i = value.size() - 1;
					}
					return result;
				}
			}
			break;
		}
		case u'"':
		{
			std::size_t pos = value.find(u'"', i + 1);
			if (pos == std::string::npos)
			{
				return nullptr;
			}
			else
			{
				i = pos;
			}
			break;
		}
		case u',':
		{
			if (!expr)
			{
				return new std::u16string(value.substr(opening_pos, i - opening_pos));
			}
			break;
		}
		}
	}
	if (!expr)
	{
		return new std::u16string(value.substr(opening_pos, i - opening_pos + 1));
	}
	return nullptr;
}

scheme_map_t* Parser::parse_object(std::u16string& value)
{
	std::u16string* fname = nullptr;
	scheme_map_t* result = nullptr;
	for (std::size_t i = 0; i < value.size(); ++i)
	{
		switch (value[i])
		{
		case u'"':
		{
			//LOG(INFO) << "parse_object i -> " << UTF16_to_CP1251(std::u16string{ value[i] }) << "   index: " << std::to_string(i);
			std::size_t j;
			for (j = i + 1; j < value.size(); ++j)
			{
				//LOG(INFO) << "parse_object j -> " << UTF16_to_CP1251(std::u16string{ value[j] }) << "   index: " << std::to_string(j);
				if (value[j] == u'"')
				{
					fname = new std::u16string(value.substr(i + 1, j - i - 1));
					//LOG(INFO) << "parse_object -> " << UTF16_to_CP1251(*fname);
					break;
				}
			}
			i = j + 1;
			//LOG(INFO) << "parse_object sep_pos -> "<< "   index: " << std::to_string(i);
			if (fname)
			{
				auto sep_pos = value.find(u':', i);
				if (sep_pos != std::string::npos)
				{
					sep_pos += 1;
					std::u16string* fvalue = get_token(value, sep_pos);
					if (!fvalue)
					{
						return result;
					}
					if (result == nullptr) { result = new scheme_map_t; }
					(*result)[*fname] = *fvalue;
					i = sep_pos-1;
				}
				else
				{
					return result;
				}
			}
			else
			{
				return result;
			}
			break;
		}
		}
	}
	return result;
}

scheme_list_t* Parser::parse_array(std::u16string& value)
{
	std::u16string* fname = nullptr;
	scheme_list_t* result = nullptr;
	for (std::size_t i = 0; i < value.size(); ++i)
	{
		std::u16string* fvalue = get_token(value, i);
		if (!fvalue)
		{
			return result;
		}
		if (result == nullptr) { result = new scheme_list_t; }
		LOG(INFO) << "parse_array -> " << Parser::UTF16_to_CP1251(*fvalue);
		result->push_back(*fvalue);
	}
	return result;
}

scheme_vector_t* Parser::parse_pair(std::u16string& value)
{
	std::u16string* fname = nullptr;
	scheme_vector_t* result = nullptr;
	for (std::size_t i = 0; i < value.size(); ++i)
	{
		std::u16string* fvalue = get_token(value, i);
		if (!fvalue)
		{
			return result;
		}
		if (result == nullptr) { result = new scheme_vector_t; }
		result->push_back(*fvalue);
	}
	return result;
}

bool isalpha(char16_t symbol)
{
	return (symbol > 0x3F && symbol < 0x5B) || (symbol > 0x61 && symbol < 0x7B);
}

std::u16string Parser::get_value(const std::u16string& value)
{
	std::size_t start_pos = value.find(u'"');
	if (start_pos != std::string::npos)
	{
		std::size_t end_pos = value.find(u'"', start_pos + 1);
		if (end_pos != std::string::npos)
		{
			std::u16string* result = new std::u16string(value.substr(start_pos + 1, end_pos - start_pos - 1));
			return *result;
		}
	}
	const char16_t* opening_symbol = nullptr;
	for (std::size_t i = 0; i < value.size(); ++i)
	{
		if (isalpha(value[i]))
		{
			if (opening_symbol == nullptr)
			{
				opening_symbol = &value[i];
			}
		}
		else
		{
			if (opening_symbol != nullptr)
			{
				break;
			}
		}

	}
	if (opening_symbol != nullptr)
	{
		std::u16string out(opening_symbol, &value[value.size() - 1] - opening_symbol + 1);
		return out;
	}
	return u"";
}

std::u16string Parser::int_to_u16string(const int &value) {
	std::string temp = std::to_string(value);
	std::u16string result(temp.size(), 0);
	for (std::size_t i = 0; i < temp.size(); ++i)
	{
		result[i] = temp[i];
	}
	return result;
}

std::u16string Parser::float_to_u16string(const float& i)
{
	return CP1251_to_UTF16(std::to_string(i));
}

int Parser::to_int32(const std::u16string& value)
{
	int result = 0;
	const char16_t* opening_symbol = nullptr;
	std::size_t i;
	for (i = 0; i < value.size(); ++i)
	{
		switch (value[i])
		{
		case u'-':case u'0':case u'1':case u'2':case u'3':case u'4':case u'5':case u'6':case u'7':case u'8':case u'9':
		{
			if (opening_symbol == nullptr)
			{
				opening_symbol = &value[i];
			}
			break;
		}
		default: if (opening_symbol != nullptr) goto transform;
		}
	}
transform:
	if (opening_symbol != nullptr)
	{
		const std::size_t len = &value[i - 1] - opening_symbol + 1;
		if (opening_symbol[0] == u'-')
		{
			switch (len - 1) {
			case 10:    result -= (opening_symbol[len - 10] - u'0') * 1000000000;
			case 9:     result -= (opening_symbol[len - 9] - u'0') * 100000000;
			case 8:     result -= (opening_symbol[len - 8] - u'0') * 10000000;
			case 7:     result -= (opening_symbol[len - 7] - u'0') * 1000000;
			case 6:     result -= (opening_symbol[len - 6] - u'0') * 100000;
			case 5:     result -= (opening_symbol[len - 5] - u'0') * 10000;
			case 4:     result -= (opening_symbol[len - 4] - u'0') * 1000;
			case 3:     result -= (opening_symbol[len - 3] - u'0') * 100;
			case 2:     result -= (opening_symbol[len - 2] - u'0') * 10;
			case 1:     result -= (opening_symbol[len - 1] - u'0');
			}
		}
		else
		{
			switch (len) {
			case 10:    result += (opening_symbol[len - 10] - u'0') * 1000000000;
			case 9:     result += (opening_symbol[len - 9] - u'0') * 100000000;
			case 8:     result += (opening_symbol[len - 8] - u'0') * 10000000;
			case 7:     result += (opening_symbol[len - 7] - u'0') * 1000000;
			case 6:     result += (opening_symbol[len - 6] - u'0') * 100000;
			case 5:     result += (opening_symbol[len - 5] - u'0') * 10000;
			case 4:     result += (opening_symbol[len - 4] - u'0') * 1000;
			case 3:     result += (opening_symbol[len - 3] - u'0') * 100;
			case 2:     result += (opening_symbol[len - 2] - u'0') * 10;
			case 1:     result += (opening_symbol[len - 1] - u'0');
			}
		}
	}
	return result;
}

float Parser::to_float(const std::u16string& value) {
	float result = 0.0;
	const char16_t* opening_symbol = nullptr;
	std::size_t i;
	for (i = 0; i < value.size(); ++i)
	{
		switch (value[i])
		{
		case u'0':case u'1':case u'2':case u'3':case u'4':case u'5':case u'6':case u'7':case u'8':case u'9':case u'.':case u'-':
		{
			if (opening_symbol == nullptr)
			{
				opening_symbol = &value[i];
			}
			break;
		}
		default: if (opening_symbol != nullptr) goto parse;
		}
	}
parse:
	if (opening_symbol != nullptr)
	{
		std::string v = UTF16_to_CP1251(std::u16string(opening_symbol, &value[i - 1] - opening_symbol + 1));
		if (v != "-")
		{
			std::replace(v.begin(), v.end(), '.', *localeconv()->decimal_point);
			result = std::stof(v);
		}
	}
	return result;
}

std::string Parser::UTF16_to_CP1251(std::u16string const& value)
{
	std::string out(value.length(), '0');
	for (int i = 0; i<value.length(); ++i)
	{
		if (value[i]>0x409 && value[i]<0x450)
		{/*
			if (value[i]<0x440)
			{*/
				out[i] = value[i] - 0x410 + 0xC0;
		/*	}
			else
			{
				out[i] = value[i] - 0x440 + 0xE0;
			}
*/
		}
		else
		{
			out[i] = value[i];
		}
	}
	return out;
}

std::u16string Parser::CP1251_to_UTF16(std::string const& value)
{
	std::u16string out(value.length(), '0');
	for (int i = 0; i<value.length(); ++i)
	{
		if (value[i]>0x7F && value[i]<0xB0)
		{
			if (value[i]<0x440)
			{
				out[i] = value[i] - 0x80 + 0x410;
			}
			else
			{
				out[i] = value[i] - 0xE0 + 0x440;
			}

		}
		else
		{
			out[i] = value[i];
		}
	}
	return out;
}

Parser::Initializator::Initializator()
{
	register_packer<Instruction_arg_extract>(u"instruction_arg_extract");
	register_packer<TileManager_Single>(u"tilemanager_single");
	register_packer<TileManager_rotate8_animate>(u"tilemanager_rotate8_animate");
	register_packer<TileManager_equilateral_animate>(u"tilemanager_equilateral_animate");
	register_packer<Instruction_get_owner>(u"instruction_get_owner");
	register_packer<Instruction_get_owner_top>(u"instruction_get_owner_top");
	register_packer<Effect>(u"effect");
	register_packer<Interaction_time>(u"interaction_time");
	register_packer<ObjectTag::Can_transfer_object>(u"can_transfer_object");
	register_packer<Instruction_check_owner_type>(u"instruction_check_owner_type");
	register_packer<GameObject>(u"game_object");
	register_packer<Inventory_cell>(u"inventory_cell");
	register_packer<Object_part>(u"object_part");
	register_packer<Attribute_map>(u"attribute_map");
	register_packer<Object_state>(u"object_state");
	register_packer<Object_state_equip>(u"object_state_equip");
	register_packer<Interaction_list>(u"interaction_list");
	register_packer<Parameter_list>(u"parameter_list");
	register_packer<Vision_list>(u"vision_list");
	register_packer<Vision_component>(u"vision_component");
	register_packer<Tag_list>(u"tag_list");
	register_packer<Parts_list>(u"parts_list");
	register_packer<Action_list>(u"action_list");
	register_packer<Instruction_slot_link>(u"interaction_slot_link");
	register_packer<ObjectTag::Label>(u"label");
	register_packer<ObjectTag::Mortal>(u"objecttag_mortal");
	register_packer<ObjectTag::Equippable>(u"equippable");
	register_packer<ObjectTag::Requirements_to_object>(u"requirements_to_object");
	register_packer<Instruction_check_part_type>(u"instruction_check_part_type");
	register_packer<Instruction_check_tag>(u"instruction_check_tag");
	register_packer<Config>(u"config");
	register_packer<AI_enemy>(u"ai_enemy");
	register_packer<GameMap>(u"game_map");
	register_packer<MapCell>(u"map_cell");
	register_packer<Action>(u"action");
	register_packer<Object_interaction>(u"template");
	register_packer<Object_manager>(u"object_manager");
	register_packer<Game_world>(u"game_world");
	register_packer<Link>(u"link");
}

#endif //DEFINITION_CPP