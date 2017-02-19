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

Parser::Parser()
{
	/*m_classes[u"A_class"] = A::create_instance;
	m_classes[u"B_class"] = B::create_instance;*/
}

Parser::~Parser()
{
}

iSerializable* Parser::deserialize_object(std::u16string& value)
{
	scheme_map_t* s = read_object(value);
	if (s)
	{
		/*for (auto element : (*s))
		{
		print_u16string(u"Элемент");
		print_u16string(element.first);
		print_u16string(u"Значение");
		print_u16string(element.second);
		}*/
		iSerializable* result = m_classes[get_value((*s)[u"$type"])]();
		result->get_packer().from_json(result, s);
		delete s;
		return result;
	}
	return nullptr;
}

std::u16string Parser::serialize_object(iSerializable* value)
{
	if (value)
	{
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

std::u16string Parser::get_value(const std::u16string& value)
{
	std::size_t start_pos = value.find(u'"');
	if (start_pos != std::string::npos)
	{
		std::size_t end_pos = value.find(u'"', start_pos + 1);
		if (end_pos != std::string::npos)
		{
			return value.substr(start_pos + 1, end_pos - start_pos - 1);
		}
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


std::string Parser::to_utf8(const std::u16string& value)
{
	std::wstring_convert<std::codecvt_utf8<int16_t>, int16_t> convert;
	auto p = reinterpret_cast<const int16_t *>(value.data());
	return convert.to_bytes(p, p + value.size());
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
	return to_u16string(std::to_string(i));
}

std::u16string Parser::to_u16string(const std::string& value)
{
	std::wstring_convert<std::codecvt_utf8<int16_t>, int16_t> convert;
	auto asInt = convert.from_bytes(value);
	return std::u16string(reinterpret_cast<char16_t const *>(asInt.data()), asInt.length());
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

template<> TileManager& Parser::from_json<TileManager&>(const std::u16string value)
{
	int result = to_int(value);
	return *Application::instance().m_graph->m_tile_managers[result];
}

template <> std::u16string Parser::to_json<TileManager&>(TileManager& value)
{
	std::u16string out = to_u16string(value.m_index);
	return out;
}

template<> std::size_t Parser::from_json<std::size_t>(const std::u16string value)
{
	std::size_t result = to_int(value);
	return result;
}

template <> std::u16string Parser::to_json<std::size_t>(std::size_t value)
{
	std::u16string out = to_u16string(value);
	return out;
}

template<> light_t& Parser::from_json<light_t&>(const std::u16string value) {
	std::u16string temp = value;
	scheme_vector_t* s = read_pair(temp);
	return light_t(from_json<int>((*s)[0]), from_json<int>((*s)[1]), from_json<int>((*s)[2]));
}

template<> std::u16string Parser::to_json<light_t&>(light_t& value)
{
	std::u16string result = u"[" + to_json<int>(value.R) + u"," + to_json<int>(value.G) + u"," + to_json<int>(value.B) + u"]";
	return result;
}

template<> float Parser::from_json<float>(const std::u16string value)
{
	float result = to_float(value);
	return result;
}

template <> std::u16string Parser::to_json<float>(float value)
{
	std::u16string out = float_to_u16string(value);
	return out;
}

template<> optical_properties_t& Parser::from_json<optical_properties_t&>(const std::u16string value) {
	std::u16string temp = value;
	scheme_vector_t* s = read_pair(temp);
	return optical_properties_t(RGB_t(from_json<float>((*s)[0]), from_json<float>((*s)[1]), from_json<float>((*s)[2])));
}

template<> std::u16string Parser::to_json<optical_properties_t&>(optical_properties_t& value)
{
	std::u16string result = u"[" + to_json<float>(value.attenuation.R) + u"," + to_json<float>(value.attenuation.G) + u"," + to_json<float>(value.attenuation.B) + u"]";
	return result;
}

