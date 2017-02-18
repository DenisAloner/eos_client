#include "Definiton.h"

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
		print_u16string(u"Ёлемент");
		print_u16string(element.first);
		print_u16string(u"«начение");
		print_u16string(element.second);
		}*/

		print_u16string(m_classes.begin()->first);
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
			return u"{\"$type\":\"" + value->get_packer().get_type() + u"\"}";
		}
		else
		{
			return u"{\"$type\":\"" + value->get_packer().get_type() + u"\"," + result + u"}";
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
			if (value[opening_pos] != u'{')
			{
				if (!expr)
				{
					opening_pos = i;
					count += 1;
					expr = true;
				}
			}
			else
			{
				count += 1;
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
			if (value[opening_pos] != u'[')
			{
				if (!expr)
				{
					opening_pos = i;
					count += 1;
					expr = true;
				}
			}
			else
			{
				count += 1;
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
	for (int i = 0; i<temp.size(); ++i)
	{
		result[i] = temp[i];
	}
	return result;
}

std::u16string Parser::to_u16string(const std::string& value)
{
	std::wstring_convert<std::codecvt_utf8<int16_t>, int16_t> convert;
	auto asInt = convert.from_bytes(value);
	return std::u16string(reinterpret_cast<char16_t const *>(asInt.data()), asInt.length());
}
