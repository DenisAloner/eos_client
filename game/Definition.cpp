#ifndef DEFINITION_CPP
#define DEFINITION_CPP

#include "Application.h"
#include "Definiton.h"
#include "map_cell.h"
#include "inventory_cell.h"

void SerializationContext::reset()
{
    m_items.clear();
    m_object_index = 0;
}

Apply_info::Apply_info(GameObject* unit, Object_interaction* object)
    : m_unit(unit)
    , m_object(object)
{
}


void Object_interaction::apply_visitor(Visitor_generic& visitor) {}

//iPacker& Object_interaction::get_packer()
//{
//    return Packer<Object_interaction>::instance();
//}

iSerializable*
Packer<Object_interaction>::from_json(scheme_map_t* value,
    SerializationContext& context)
{
    // LOG(INFO) << Parser::UTF16_to_CP1251(Parser::get_value((*value)[u"value"]))
    // << " :: " <<
    // std::to_string(GameObjectManager::m_config.m_templates.size());
    return GameObjectManager::m_config
        ->m_templates[utf16_to_cp1251(get_value((*value)[u"value"]))]
        ->clone();
}

std::u16string Packer<MapCell>::to_json(iSerializable* value,
    SerializationContext& context)
{
    if (value) {
        auto& obj = *dynamic_cast<MapCell*>(value);
        auto out = u"{\"$type\":\"" + value->get_packer().get_type_name() + u"\",\"value\":[" + int_to_u16string(obj.x) + u"," + int_to_u16string(obj.y) + u"," + int_to_u16string(obj.z) + u"]}";
        return out;
    } else {
        std::u16string out = u"null";
        return out;
    }
}

std::string Packer<MapCell>::to_binary(iSerializable* value,
    SerializationContext& context)
{
    if (value) {
        auto& obj = *dynamic_cast<MapCell*>(value);
        auto id = value->get_packer().get_type_id() + 1;
        return std::string(reinterpret_cast<const char*>(&id),
                   sizeof(std::size_t))
            + parser_to_binary<int>(obj.x, context) + parser_to_binary<int>(obj.y, context) + parser_to_binary<int>(obj.z, context);
    } else {
        std::size_t id = 0;
        return std::string(reinterpret_cast<const char*>(&id),
            sizeof(std::size_t));
    }
}

iSerializable* Packer<MapCell>::from_json(scheme_map_t* value,
    SerializationContext& context)
{
    const auto s = read_pair((*value)[u"value"]);
    int x;
    int y;
    int z;
    parser_from_json<int>((*s)[0], x, context);
    parser_from_json<int>((*s)[1], y, context);
    parser_from_json<int>((*s)[2], z, context);
    return &context.m_game_world.m_maps.front()->get(z, y, x);
}

iSerializable* Packer<MapCell>::from_binary(const std::string& value,
    std::size_t& pos,
    SerializationContext& context)
{
    int x;
    int y;
    int z;
    parser_from_binary<int>(value, x, pos, context);
    parser_from_binary<int>(value, y, pos, context);
    parser_from_binary<int>(value, z, pos, context);
    return &context.m_game_world.m_maps.front()->get(z, y, x);
}

iSerializable*
Packer<SerializationLink>::from_json(scheme_map_t* value,
    SerializationContext& context)
{
    std::size_t id;
    parser_from_json<std::size_t>((*value)[u"value"], id, context);
    return context.m_items[id - 1];
}

iSerializable* Packer<SerializationLink>::from_binary(
    const std::string& value, std::size_t& pos, SerializationContext& context)
{
    std::size_t id;
    parser_from_binary<std::size_t>(value, id, pos, context);
    return context.m_items[id - 1];
}

std::u16string Packer<GameObject>::to_json(iSerializable* value,
    SerializationContext& context)
{
    std::u16string out = u"{\"$type\":\"" + value->get_packer().get_type_name() + u"\",\"value\":" + int_to_u16string(value->m_serialization_index) + u"}";
    return out;
}

std::string Packer<GameObject>::to_binary(iSerializable* value,
    SerializationContext& context)
{
	auto id = value->get_packer().get_type_id() + 1;
    auto index = value->m_serialization_index;
    return std::string(reinterpret_cast<const char*>(&id), sizeof(std::size_t)) + std::string(reinterpret_cast<const char*>(&index), sizeof(std::size_t));
}

iSerializable* Packer<GameObject>::from_json(scheme_map_t* value,
    SerializationContext& context)
{
    // LOG(INFO) << "iSerializable* Packer<GameObject>::from_json(scheme_map_t*
    // value)";
    const auto i = to_int32((*value)[u"value"]);
    const auto it = std::next(context.m_game_world.m_objects.begin(), i);
    return &(*it);
}

iSerializable* Packer<GameObject>::from_binary(const std::string& value,
    std::size_t& pos,
    SerializationContext& context)
{
    std::size_t i;
    parser_from_binary<std::size_t>(value, i, pos, context);
    const auto it = std::next(context.m_game_world.m_objects.begin(), i);
    return &(*it);
}

void Packer<GameObject>::from_binary(iSerializable& object,
    const std::string& value, std::size_t& pos,
    SerializationContext& context)
{
    auto& obj = dynamic_cast<GameObject&>(object);
    object_from_binary<GameObject>(obj, value, pos, context);
    obj.set_direction(obj.m_direction);
    obj.update_interaction();
}

Serialization::Serialization()
{
    Logger::instance().info("Start register packer");
    register_packer<Instruction_arg_extract>(u"instruction_arg_extract");
    register_packer<TileManager_Single>(u"tilemanager_single");
    register_packer<TileManager_rotate8_animate>(u"tilemanager_rotate8_animate");
    register_packer<TileManager_equilateral_animate>(
        u"tilemanager_equilateral_animate");
    register_packer<Instruction_get_owner>(u"instruction_get_owner");
    register_packer<Instruction_get_owner_top>(u"instruction_get_owner_top");
    register_packer<Effect>(u"effect");
    register_packer<Interaction_time>(u"interaction_time");
    register_packer<ObjectTag::Can_transfer_object>(u"can_transfer_object");
    register_packer<Instruction_check_owner_type>(
        u"instruction_check_owner_type");
    register_packer<GameObject>(u"game_object");
    register_packer<InventoryCell>(u"inventory_cell");
    register_packer<ObjectPart>(u"object_part");
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
    register_packer<GameWorld>(u"game_world");
    register_packer<SerializationLink>(u"link");
    register_packer<TileManager_Single_Atlas>(u"tilemanager_single_atlas");
    register_packer<TileManager_Atlas_Rotatable>(u"tilemanager_atlas_rotatable");
}

Serialization::~Serialization() {}

// std::u16string Parser::serialize_object(iSerializable* value)
//{
//	if (value)
//	{
//		LOG(INFO) <<
// UTF16_to_CP1251((*value).get_packer().get_type_name());
// std::u16string result = value->get_packer().parser_to_json(value); if
//(result.empty())
//		{
//			std::u16string out = u"{\"$type\":\"" +
// value->get_packer().get_type_name() + u"\"}"; 			return
// out;
//		}
//		else
//		{
//			std::u16string out = u"{\"$type\":\"" +
// value->get_packer().get_type_name() + u"\"," + result + u"}";
// return out;
//		}
//	}
//	else
//	{
//		return u"null";
//	}
//}

scheme_map_t* read_object(std::u16string& value)
{
    const char16_t* opening_symbol = nullptr;
    auto count = 0;
    for (auto& i : value) {
        switch (i) {
        case u'{': {
            if (opening_symbol == nullptr) {
                opening_symbol = &i;
            }
            count += 1;
            break;
        }
        case u'}': {
            if (opening_symbol != nullptr) {
                count = count - 1;
                if (count == 0) {
                    std::u16string sub_value(opening_symbol + 1, &i - opening_symbol - 1);
                    // LOG(INFO) << "read_object -> " << UTF16_to_CP1251(sub_value);
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

scheme_list_t* read_array(std::u16string& value)
{
    const char16_t* opening_symbol = nullptr;
    auto count = 0;
    for (auto& i : value) {
        switch (i) {
        case u'[': {
            if (opening_symbol == nullptr) {
                opening_symbol = &i;
            }
            count += 1;
            break;
        }
        case u']': {
            if (opening_symbol != nullptr) {
                count = count - 1;
                if (count == 0) {
                    std::u16string sub_value(opening_symbol + 1, &i - opening_symbol - 1);
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

scheme_vector_t* read_pair(std::u16string& value)
{
    const char16_t* opening_symbol = nullptr;
    auto count = 0;
    for (auto& i : value) {
        switch (i) {
        case u'[': {
            if (opening_symbol == nullptr) {
                opening_symbol = &i;
            }
            count += 1;
            break;
        }
        case u']': {
            if (opening_symbol != nullptr) {
                count = count - 1;
                if (count == 0) {
                    std::u16string sub_value(opening_symbol + 1, &i - opening_symbol - 1);
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
    auto opening_pos = i;
    std::size_t count = 0;
    auto expr = false;
    for (i; i < value.size(); ++i) {
        switch (value[i]) {
        case u'{': {
            if (expr) {
                if (value[opening_pos] == u'{') {
                    count += 1;
                }
            } else {
                opening_pos = i;
                count += 1;
                expr = true;
            }
            break;
        }
        case u'}': {
            if (value[opening_pos] == u'{') {
                count -= 1;
                if (count == 0) {
                    const auto pos = value.find(u',', i);
                    const auto result = new std::u16string(
                        value.substr(opening_pos, i - opening_pos + 1));
                    if (pos != std::string::npos) {
                        i = pos;
                    } else {
                        i = value.size() - 1;
                    }
                    return result;
                }
            }
            break;
        }
        case u'[': {
            if (expr) {
                if (value[opening_pos] == u'[') {
                    count += 1;
                }
            } else {
                opening_pos = i;
                count += 1;
                expr = true;
            }
            break;
        }
        case u']': {
            if (value[opening_pos] == u'[') {
                count -= 1;
                if (count == 0) {
                    const auto pos = value.find(u',', i);
                    const auto result = new std::u16string(
                        value.substr(opening_pos, i - opening_pos + 1));
                    if (pos != std::string::npos) {
                        i = pos;
                    } else {
                        i = value.size() - 1;
                    }
                    return result;
                }
            }
            break;
        }
        case u'"': {
            const auto pos = value.find(u'"', i + 1);
            if (pos == std::string::npos) {
                return nullptr;
            } else {
                i = pos;
            }
            break;
        }
        case u',': {
            if (!expr) {
                return new std::u16string(value.substr(opening_pos, i - opening_pos));
            }
            break;
        }
        }
    }
    if (!expr) {
        return new std::u16string(value.substr(opening_pos, i - opening_pos + 1));
    }
    return nullptr;
}

scheme_map_t* parse_object(std::u16string& value)
{
    std::u16string* property_name = nullptr;
    scheme_map_t* result = nullptr;
    for (std::size_t i = 0; i < value.size(); ++i) {
        switch (value[i]) {
        case u'"': {
            // LOG(INFO) << "parse_object i -> " << UTF16_to_CP1251(std::u16string{
            // value[i] }) << "   index: " << std::to_string(i);
            std::size_t j;
            for (j = i + 1; j < value.size(); ++j) {
                // LOG(INFO) << "parse_object j -> " << UTF16_to_CP1251(std::u16string{
                // value[j] }) << "   index: " << std::to_string(j);
                if (value[j] == u'"') {
                    property_name = new std::u16string(value.substr(i + 1, j - i - 1));
                    // LOG(INFO) << "parse_object -> " << UTF16_to_CP1251(*fname);
                    break;
                }
            }
            i = j + 1;
            // LOG(INFO) << "parse_object sep_pos -> "<< "   index: " <<
            // std::to_string(i);
            if (property_name) {
                auto sep_pos = value.find(u':', i);
                if (sep_pos != std::string::npos) {
                    sep_pos += 1;
                    const auto token = get_token(value, sep_pos);
                    if (!token) {
                        return result;
                    }
                    if (result == nullptr) {
                        result = new scheme_map_t;
                    }
                    (*result)[*property_name] = *token;
                    i = sep_pos - 1;
                } else {
                    return result;
                }
            } else {
                return result;
            }
            break;
        }
        }
    }
    return result;
}

scheme_list_t* parse_array(std::u16string& value)
{
    scheme_list_t* result = nullptr;
    for (std::size_t i = 0; i < value.size(); ++i) {
        const auto token = get_token(value, i);
        if (!token) {
            return result;
        }
        if (result == nullptr) {
            result = new scheme_list_t;
        }
        result->push_back(*token);
    }
    return result;
}

scheme_vector_t* parse_pair(std::u16string& value)
{
    scheme_vector_t* result = nullptr;
    for (std::size_t i = 0; i < value.size(); ++i) {
        const auto token = get_token(value, i);
        if (!token) {
            return result;
        }
        if (result == nullptr) {
            result = new scheme_vector_t;
        }
        result->push_back(*token);
    }
    return result;
}

bool is_alpha(const char16_t symbol)
{
    return (symbol > 0x3F && symbol < 0x5B) || (symbol > 0x61 && symbol < 0x7B);
}

std::u16string get_value(const std::u16string& value)
{
    const auto start_pos = value.find(u'"');
    if (start_pos != std::string::npos) {
        const auto end_pos = value.find(u'"', start_pos + 1);
        if (end_pos != std::string::npos) {
            const auto result = new std::u16string(
                value.substr(start_pos + 1, end_pos - start_pos - 1));
            return *result;
        }
    }
    const char16_t* opening_symbol = nullptr;
    for (const auto& i : value) {
        if (is_alpha(i)) {
            if (opening_symbol == nullptr) {
                opening_symbol = &i;
            }
        } else {
            if (opening_symbol != nullptr) {
                break;
            }
        }
    }
    if (opening_symbol != nullptr) {
        std::u16string out(opening_symbol,
            &value[value.size() - 1] - opening_symbol + 1);
        return out;
    }
    return u"";
}

std::u16string int_to_u16string(const int& value)
{
    auto temp = std::to_string(value);
    std::u16string result(temp.size(), 0);
    for (std::size_t i = 0; i < temp.size(); ++i) {
        result[i] = temp[i];
    }
    return result;
}

std::u16string float_to_u16string(const float& i)
{
    return cp1251_to_utf16(std::to_string(i));
}

int to_int32(const std::u16string& value)
{
    auto result = 0;
    const char16_t* opening_symbol = nullptr;
    std::size_t i;
    for (i = 0; i < value.size(); ++i) {
        switch (value[i]) {
        case u'-':
        case u'0':
        case u'1':
        case u'2':
        case u'3':
        case u'4':
        case u'5':
        case u'6':
        case u'7':
        case u'8':
        case u'9': {
            if (opening_symbol == nullptr) {
                opening_symbol = &value[i];
            }
            break;
        }
        default:
            if (opening_symbol != nullptr)
                goto transform;
        }
    }
transform:
    if (opening_symbol != nullptr) {
        const std::size_t len = &value[i - 1] - opening_symbol + 1;
        if (opening_symbol[0] == u'-') {
            switch (len - 1) {
            case 10:
                result -= (opening_symbol[len - 10] - u'0') * 1000000000;
            case 9:
                result -= (opening_symbol[len - 9] - u'0') * 100000000;
            case 8:
                result -= (opening_symbol[len - 8] - u'0') * 10000000;
            case 7:
                result -= (opening_symbol[len - 7] - u'0') * 1000000;
            case 6:
                result -= (opening_symbol[len - 6] - u'0') * 100000;
            case 5:
                result -= (opening_symbol[len - 5] - u'0') * 10000;
            case 4:
                result -= (opening_symbol[len - 4] - u'0') * 1000;
            case 3:
                result -= (opening_symbol[len - 3] - u'0') * 100;
            case 2:
                result -= (opening_symbol[len - 2] - u'0') * 10;
            case 1:
                result -= opening_symbol[len - 1] - u'0';
            }
        } else {
            switch (len) {
            case 10:
                result += (opening_symbol[len - 10] - u'0') * 1000000000;
            case 9:
                result += (opening_symbol[len - 9] - u'0') * 100000000;
            case 8:
                result += (opening_symbol[len - 8] - u'0') * 10000000;
            case 7:
                result += (opening_symbol[len - 7] - u'0') * 1000000;
            case 6:
                result += (opening_symbol[len - 6] - u'0') * 100000;
            case 5:
                result += (opening_symbol[len - 5] - u'0') * 10000;
            case 4:
                result += (opening_symbol[len - 4] - u'0') * 1000;
            case 3:
                result += (opening_symbol[len - 3] - u'0') * 100;
            case 2:
                result += (opening_symbol[len - 2] - u'0') * 10;
            case 1:
                result += opening_symbol[len - 1] - u'0';
            }
        }
    }
    return result;
}

float to_float(const std::u16string& value)
{
    float result = 0.0;
    const char16_t* opening_symbol = nullptr;
    std::size_t i;
    for (i = 0; i < value.size(); ++i) {
        switch (value[i]) {
        case u'0':
        case u'1':
        case u'2':
        case u'3':
        case u'4':
        case u'5':
        case u'6':
        case u'7':
        case u'8':
        case u'9':
        case u'.':
        case u'-': {
            if (opening_symbol == nullptr) {
                opening_symbol = &value[i];
            }
            break;
        }
        default:
            if (opening_symbol != nullptr)
                goto parse;
        }
    }
parse:
    if (opening_symbol != nullptr) {
        auto v = utf16_to_cp1251(
            std::u16string(opening_symbol, &value[i - 1] - opening_symbol + 1));
        if (v != "-") {
            std::replace(v.begin(), v.end(), '.', *localeconv()->decimal_point);
            result = std::stof(v);
        }
    }
    return result;
}

std::size_t get_u8_string_length(const std::u8string& value)
{
    std::size_t index, bytes_number, length;
    for (length = 0, index = 0, bytes_number = value.length();
         index < bytes_number; index++, length++) {
        const auto symbol = value[index];
        if (symbol >= 0 && symbol <= 127)
            continue;
        if ((symbol & 0xE0) == 0xC0)
            index += 1;
        else if ((symbol & 0xF0) == 0xE0)
            index += 2;
        else if ((symbol & 0xF8) == 0xF0)
            index += 3;
        else
            return 0;
    }
    return length;
}

std::u16string utf8_to_utf16(const std::u8string& value)
{
    std::u16string result(get_u8_string_length(value), '0');
    std::size_t pos = 0;
    for (std::size_t i = 0; i < value.size(); ++i) {
        const auto code = value[i];
        if ((code & 0x80) == 0) {
            result[pos] = code;
            pos += 1;
        } else if ((code & 0xC0) == 0xC0) {
            result[pos] = static_cast<unsigned short>(0x1F & code) << 6 | 0x3F & value[i + 1];
            pos += 1;
            i += 1;
        }
    }
    return result;
}

std::string utf16_to_cp1251(std::u16string const& value)
{
    std::string out(value.length(), '0');
    for (std::size_t i = 0; i < value.length(); ++i) {
        if (value[i] > 0x409 && value[i] < 0x450) {
            out[i] = value[i] - 0x410 + 0xC0;
        } else {
            out[i] = value[i];
        }
    }
    return out;
}

std::u16string cp1251_to_utf16(std::string const& value)
{
    std::u16string out(value.length(), '0');
    for (std::size_t i = 0; i < value.length(); ++i) {
        const auto symbol = static_cast<unsigned char>(value[i]);
        if (symbol > 0x7F && symbol < 0xB0) {
            out[i] = symbol - 0xE0 + 0x440;
        } else {
            out[i] = symbol;
        }
    }
    return out;
}

#endif // DEFINITION_CPP