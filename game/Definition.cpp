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

Packer_generic& iSerializable::get_packer()
{
	return Packer<Object_interaction>::Instance();
}

std::u16string Packer<Action>::to_json(iSerializable* object)
{
	return object_to_json<Action>(dynamic_cast<Action*>(object));
};

std::map<std::u16string, instance_function_t> Parser::m_classes = {};

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

std::unordered_map<effect_e, std::string>  Parser::m_string_effect_e = {
	{ effect_e::value,"модификатор значения" },
	{ effect_e::limit,"модификатор лимита" },
	{ effect_e::start_angle, "модификатор начального угла обзора" },
	{ effect_e::end_angle, "модификатор конечного угла обзора" }
};


std::unordered_map<interaction_e, std::string>  Parser::m_string_interaction_e = {
	{ interaction_e::total_damage, "общий дополнительный урон" },
	{ interaction_e::damage, "урон" },
	{ interaction_e::buff, "баффы" },
	{ interaction_e::use, "применение" },
	{ interaction_e::health, "здоровье" },
	{ interaction_e::strength, "сила" },
	{ interaction_e::intelligence, "интеллект" },
	{ interaction_e::dexterity, "ловкость" },
	{ interaction_e::hunger,  "голод" },
	{ interaction_e::thirst,  "жажда" },
	{ interaction_e::poison,  "яд" },
	{ interaction_e::action,  "действия" },
	{ interaction_e::tag,  "метки" },
	{ interaction_e::body,  "тело" },
	{ interaction_e::weapon_damage,  "урон оружия" },
	{ interaction_e::skill_sword,  "владение мечом" },
	{ interaction_e::skill_bow,  "владение луком" },
	{ interaction_e::strength_bonus,  "бонус силы" },
	{ interaction_e::demand_weapon_skill,  "требование к владению оружием" },
	{ interaction_e::evasion_skill, "навык уклонения" },
	{ interaction_e::weapon_range,  "дальность" },
	{ interaction_e::vision,  "зрение" },
	{ interaction_e::vision_component,  "поле зрения" },
	{ interaction_e::skill_unarmed_combat,  "владение безоружным боем" },
	{ interaction_e::equip,  "экипировка" }
};

std::unordered_map<object_tag_e, std::string>  Parser::m_string_object_tag_e = {
	{ object_tag_e::poison_resist , "сопротивление к яду" },
	{ object_tag_e::purification_from_poison , "очищение от яда" },
	{ object_tag_e::mortal , "смертное существо" },
	{ object_tag_e::pass_able , "не является преградой" },
	{ object_tag_e::pick_able , "можно взять" },
	{ object_tag_e::seethrough_able , "не загораживает обзор" },
	{ object_tag_e::activator , "активирует/деактивирует механизмы" },
	{ object_tag_e::fast_move , "быстрое передвижение" },
	{ object_tag_e::equippable , "можно одеть" },
	{ object_tag_e::ring , "кольцо" },
	{ object_tag_e::requirements_to_object , "требования к предмету" },
	{ object_tag_e::cursed , "наложено проклятье" },
	{ object_tag_e::can_transfer_object , "может перекладывать предметы" },
	{ object_tag_e::footwear , "обувь" }
};

std::wstring_convert<std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>, wchar_t> Parser::m_convert;

std::size_t Parser::m_object_index = 0;

Parser::Parser()
{
	/*m_classes[u"A_class"] = A::create_instance;
	m_classes[u"B_class"] = B::create_instance;*/
}

Parser::~Parser()
{
}

std::u16string Parser::serialize_object(iSerializable* value)
{
	if (value)
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
		return u"null";
	}
}

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
					std::u16string value(opening_symbol + 1, &value[i] - opening_symbol - 1);
					return parse_object(value);
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
					std::u16string value(opening_symbol + 1, &value[i] - opening_symbol - 1);
					return parse_array(value);
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
					std::u16string value(opening_symbol + 1, &value[i] - opening_symbol - 1);
					return parse_pair(value);
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
						i = pos + 1;
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
						i = pos + 1;
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
			std::size_t j;
			for (j = i + 1; j < value.size(); ++j)
			{
				if (value[j] == u'"')
				{
					fname = new std::u16string(value.substr(i + 1, j - i - 1));
					break;
				}
			}
			i = j + 1;
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
					i = sep_pos;
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

//void Parser::print_u16string(std::u16string value)
//{
//	std::wstring ws = m_convert.from_bytes(
//		reinterpret_cast<const char*> (value.data()),
//		reinterpret_cast<const char*> (value.data() + value.size()));
//	std::wcout << ws << std::endl;
//}

void Parser::print_u16string(std::u16string value)
{

	/*std::wstring ws = m_convert.from_bytes(
	reinterpret_cast<const char*> (value.data()),
	reinterpret_cast<const char*> (value.data() + value.size()));
	std::wcout << ws << std::endl;*/
}



std::u16string Parser::to_u16string(int const &value) {
	std::string temp = std::to_string(value);
	std::u16string result(temp.size(), 0);
	for (int i = 0; i < temp.size(); ++i)
	{
		result[i] = temp[i];
	}
	return result;
}

std::u16string Parser::float_to_u16string(float const& i)
{
	return CP866_to_UTF16(std::to_string(i));
}

int Parser::to_int(const std::u16string& value) {
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
		return std::stoi(ws);
	}
	return 0;
}

float Parser::to_float(const std::u16string& value) {
	const char16_t* opening_symbol = nullptr;
	for (std::size_t i = 0; i < value.size(); ++i)
	{
		switch (value[i])
		{
		case u'0':case u'1':case u'2':case u'3':case u'4':case u'5':case u'6':case u'7':case u'8':case u'9':case u'.':
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
		return std::stof(ws);
	}
	return 0.0;
}

std::string Parser::UTF16_to_CP866(std::u16string const& value)
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

std::u16string Parser::CP866_to_UTF16(std::string const& value)
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

void Parser::register_class(std::u16string class_name, instance_function_t function)
{
	m_classes[class_name] = function;
}

template<> void Parser::from_json<int>(const std::u16string value, int& prop) {
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

template<> void Parser::from_json<std::u16string>(const std::u16string value, std::u16string& prop)
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

template<> void Parser::from_json<std::string>(const std::u16string value, std::string& prop) {
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

template<> std::u16string Parser::to_json<int>(int value)
{
	return to_u16string(value);
}

template<> std::u16string Parser::to_json<std::u16string>(std::u16string value)
{
	return u"\"" + value + u"\"";
}

template<> std::u16string Parser::to_json<std::string>(std::string value)
{
	return u"\"" + CP866_to_UTF16(value) + u"\"";
}

template<> std::u16string Parser::to_json<game_object_size_t>(game_object_size_t value)
{
	std::u16string result = u"[" + to_json<int>(value.x) + u"," + to_json<int>(value.y) + u"," + to_json<int>(value.z) + u"]";
	return result;
};


template<> void Parser::from_json<predicat_t*>(const std::u16string value, predicat_t*& prop)
{
	LOG(INFO) << Parser::UTF16_to_CP866(get_value(value)) << Parser::UTF16_to_CP866(value);
	if (get_value(value) == u"null")
	{
		prop = nullptr;
	}
	else
	{
		int result = to_int(value);
		prop = Application::instance().m_ai_manager->m_fov_qualifiers[result];
	}

}

template <> std::u16string Parser::to_json<predicat_t&>(predicat_t& value)
{
	std::u16string out = to_u16string(value.index);
	return out;
}

template<> void Parser::from_json<std::size_t>(const std::u16string value, std::size_t& prop)
{
	std::size_t result = to_int(value);
	prop=result;
}

template <> std::u16string Parser::to_json<std::size_t>(std::size_t value)
{
	std::u16string out = to_u16string(value);
	return out;
}

template<> void Parser::from_json<light_t*>(const std::u16string value, light_t*& prop)
{
	LOG(INFO) << Parser::UTF16_to_CP866(get_value(value)) << Parser::UTF16_to_CP866(value);
	if (get_value(value) == u"null")
	{
		prop = nullptr;
	}
	else
	{
		prop = new light_t();
		std::u16string temp = value;
		scheme_vector_t* s = read_pair(temp);
		from_json<int>((*s)[0], prop->R);
		from_json<int>((*s)[1], prop->G);
		from_json<int>((*s)[2], prop->B);
	}
}

template<> std::u16string Parser::to_json<light_t&>(light_t& value)
{
	std::u16string result = u"[" + to_json<int>(value.R) + u"," + to_json<int>(value.G) + u"," + to_json<int>(value.B) + u"]";
	return result;
}

template<> void Parser::from_json<float>(const std::u16string value,float& prop)
{
	float result = to_float(value);
	prop = result;
}

template <> std::u16string Parser::to_json<float>(float value)
{
	std::u16string out = float_to_u16string(value);
	return out;
}

template<> void Parser::from_json<optical_properties_t*>(const std::u16string value, optical_properties_t*& prop) 
{
	LOG(INFO) << Parser::UTF16_to_CP866(get_value(value)) << Parser::UTF16_to_CP866(value);
	if (get_value(value) == u"null")
	{
		prop = nullptr;
	}
	else
	{
		prop = new optical_properties_t();
		std::u16string temp = value;
		scheme_vector_t* s = read_pair(temp);
		from_json<float>((*s)[0], prop->attenuation.R);
		from_json<float>((*s)[1], prop->attenuation.G);
		from_json<float>((*s)[2], prop->attenuation.B);
	}

}

template<> std::u16string Parser::to_json<optical_properties_t&>(optical_properties_t& value)
{
	std::u16string result = u"[" + to_json<float>(value.attenuation.R) + u"," + to_json<float>(value.attenuation.G) + u"," + to_json<float>(value.attenuation.B) + u"]";
	return result;
}

template<> void Parser::from_json<AI_FOV>(const std::u16string value, AI_FOV& prop) {
	std::u16string temp = value;
	scheme_vector_t* s = read_pair(temp);
	from_json<int>((*s)[0],prop.radius);
	int x;
	from_json<int>((*s)[1], x);
	prop.qualifier= Application::instance().m_ai_manager->m_fov_qualifiers[x];
	from_json<int>((*s)[2],prop.start_angle);
	from_json<int>((*s)[3],prop.end_angle);
};

template<> std::u16string Parser::to_json<AI_FOV>(AI_FOV value)
{
	std::u16string result = u"[" + to_json<int>(value.radius) + u"," + to_json<int>(value.qualifier->index) + u"," + to_json<int>(value.start_angle) + u"," + to_json<int>(value.end_angle) + u"]";
	return result;
}

template<> void Parser::from_json<game_object_size_t>(const std::u16string value, game_object_size_t& prop) {
	std::u16string temp = value;
	scheme_vector_t* s = read_pair(temp);
	from_json<int>((*s)[0], prop.x);
	from_json<int>((*s)[1], prop.y);
	from_json<int>((*s)[2], prop.z);
};

template<> void Parser::from_json<GLuint, Parser::icon_t>(const std::u16string value, GLuint& prop)
{
	std::string&& name = UTF16_to_CP866(get_value(value));
	prop = GameObjectManager::m_config.m_icons[name];
}

template <> std::u16string Parser::to_json<GLuint, Parser::icon_t>(GLuint& value)
{
	return u"";
}

template<> void Parser::from_json< std::map<std::string, GLuint>, Parser::icon_map_t>(const std::u16string value, std::map<std::string, GLuint>& prop)
{
	std::u16string temp = value;
	scheme_list_t* s = read_array(temp);
	if (s)
	{
		for (auto element : (*s))
		{
			
			std::string&& name = UTF16_to_CP866(get_value(element));
			prop[name] = Application::instance().m_graph->load_texture(FileSystem::instance().m_resource_path + "Tiles\\" + name + ".bmp");
		}
		delete s;
	}
}

template <> std::u16string Parser::to_json<std::map<std::string, GLuint>, Parser::icon_map_t>(std::map<std::string, GLuint>& value)
{
	return u"";
}

template<> void Parser::from_json<TileManager*,Parser::tilemanager_ref_t>(const std::u16string value, TileManager*& prop)
{
	{
		LOG(INFO) << Parser::UTF16_to_CP866(get_value(value)) << Parser::UTF16_to_CP866(value);
		if (get_value(value) == u"null")
		{
			prop = nullptr;
		}
		else
		{
			int result = to_int(value);
			prop = Application::instance().m_graph->m_tile_managers[result];
		}
	}

}

template <> std::u16string Parser::to_json<TileManager*,Parser::tilemanager_ref_t>(TileManager*& value)
{
	std::u16string out = to_u16string(value->m_index);
	return out;
}

template<> void Parser::from_json< std::vector<GLuint>, Parser::icon_t>(const std::u16string value, std::vector<GLuint>& prop)
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

template <> std::u16string Parser::to_json<std::vector<GLuint>, Parser::icon_t>(std::vector<GLuint>& value)
{
	return u"";
}

#endif //DEFINITION_CPP