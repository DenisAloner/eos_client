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

std::unordered_map<interaction_e, std::string>  Parser::m_string_interaction_e = {
	{ interaction_e::total_damage, "����� �������������� ����" },
	{ interaction_e::damage, "����" },
	{ interaction_e::buff, "�����" },
	{ interaction_e::use, "����������" },
	{ interaction_e::health, "��������" },
	{ interaction_e::strength, "����" },
	{ interaction_e::intelligence, "���������" },
	{ interaction_e::dexterity, "��������" },
	{ interaction_e::hunger,  "�����" },
	{ interaction_e::thirst,  "�����" },
	{ interaction_e::poison,  "��" },
	{ interaction_e::action,  "��������" },
	{ interaction_e::tag,  "�����" },
	{ interaction_e::body,  "����" },
	{ interaction_e::weapon_damage,  "���� ������" },
	{ interaction_e::skill_sword,  "�������� �����" },
	{ interaction_e::skill_bow,  "�������� �����" },
	{ interaction_e::strength_bonus,  "����� ����" },
	{ interaction_e::demand_weapon_skill,  "���������� � �������� �������" },
	{ interaction_e::evasion_skill, "����� ���������" },
	{ interaction_e::weapon_range,  "���������" },
	{ interaction_e::vision,  "������" },
	{ interaction_e::vision_component,  "���� ������" },
	{ interaction_e::skill_unarmed_combat,  "�������� ���������� ����" },
	{ interaction_e::equip,  "����������" }
};

std::unordered_map<object_tag_e, std::string>  Parser::m_string_object_tag_e = {
	{ object_tag_e::poison_resist , "������������� � ���" },
	{ object_tag_e::purification_from_poison , "�������� �� ���" },
	{ object_tag_e::mortal , "�������� ��������" },
	{ object_tag_e::pass_able , "�� �������� ���������" },
	{ object_tag_e::pick_able , "����� �����" },
	{ object_tag_e::seethrough_able , "�� ������������ �����" },
	{ object_tag_e::activator , "����������/������������ ���������" },
	{ object_tag_e::fast_move , "������� ������������" },
	{ object_tag_e::equippable , "����� �����" },
	{ object_tag_e::ring , "������" },
	{ object_tag_e::requirements_to_object , "���������� � ��������" },
	{ object_tag_e::cursed , "�������� ���������" },
	{ object_tag_e::can_transfer_object , "����� ������������� ��������" },
	{ object_tag_e::footwear , "�����" }
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

iSerializable* Parser::deserialize_object(std::u16string& value)
{
	scheme_map_t* s = read_object(value);
	if (s)
	{
		/*for (auto element : (*s))
		{
		print_u16string(u"�������");
		print_u16string(element.first);
		print_u16string(u"��������");
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
		if (value[i]>0x409 && value[i]<0x44f)
		{
			if (value[i]<0x440)
			{
				out[i] = value[i] - 0x410 + 0x80;
			}
			else
			{
				out[i] = value[i] - 0x440 + 0xE0;
			}

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

//template<> predicat_t& Parser::from_json<predicat_t&>(const std::u16string value)
//{
//	int result = to_int(value);
//	return *Application::instance().m_ai_manager->m_fov_qualifiers[result];
//}
//
//template <> std::u16string Parser::to_json<predicat_t&>(predicat_t& value)
//{
//	std::u16string out = to_u16string(value.index);
//	return out;
//}



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

template<> AI_FOV Parser::from_json<AI_FOV>(const std::u16string value) {
	std::u16string temp = value;
	scheme_vector_t* s = read_pair(temp);
	AI_FOV out;
	out.radius = from_json<int>((*s)[0]);
	out.qualifier= Application::instance().m_ai_manager->m_fov_qualifiers[from_json<int>((*s)[1])];
	out.start_angle = from_json<int>((*s)[2]);
	out.end_angle = from_json<int>((*s)[3]);
	return out;
};

template<> std::u16string Parser::to_json<AI_FOV>(AI_FOV value)
{
	std::u16string result = u"[" + to_json<int>(value.radius) + u"," + to_json<int>(value.qualifier->index) + u"," + to_json<int>(value.start_angle) + u"," + to_json<int>(value.end_angle) + u"]";
	return result;
}