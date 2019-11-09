#include <reader.h>

float json_to_float(const std::u16string_view& value)
{
    float result = 0.0;
    auto opening = std::string::npos;
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
            if (opening == std::string::npos) {
                opening = i;
            }
            break;
        }
        default:
            if (opening != std::string::npos)
                goto parse;
        }
    }
parse:
    if (opening != std::string::npos) {
        auto v = utf16_to_cp1251(std::u16string(value.substr(opening, i - opening)));
        if (v != "-") {
            std::replace(v.begin(), v.end(), '.', *localeconv()->decimal_point);
            result = std::stof(v);
        }
    }
    return result;
}

std::u16string_view json_token(const std::u16string_view& value, std::size_t& i)
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
                    const auto result = value.substr(opening_pos, i - opening_pos + 1);
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
                    const auto result = value.substr(opening_pos, i - opening_pos + 1);
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
                return {};
            }
            i = pos;
            break;
        }
        case u',': {
            if (!expr) {
                return value.substr(opening_pos, i - opening_pos);
            }
            break;
        }
        }
    }
    if (!expr) {
        return value.substr(opening_pos, i - opening_pos + 1);
    }
    return {};
}

json_vector_t* json_pair(const std::u16string_view& value)
{
    json_vector_t* result = nullptr;
    for (std::size_t i = 0; i < value.size(); ++i) {
        const auto token = json_token(value, i);
        if (token.empty()) {
            return result;
        }
        if (result == nullptr) {
            result = new json_vector_t;
        }
        result->push_back(token);
    }
    return result;
}

json_vector_t* read_json_pair(const std::u16string_view& value)
{
    auto count = 0;
    auto position_opening_symbol = std::u16string::npos;
    for (long index = 0; index < value.size(); ++index) {
        switch (value[index]) {
        case u'[': {
            if (position_opening_symbol == std::u16string::npos) {
                position_opening_symbol = index;
            }
            count += 1;
            break;
        }
        case u']': {
            if (position_opening_symbol != std::u16string::npos) {
                count -= 1;
                if (count == 0) {
                    return json_pair(value.substr(position_opening_symbol + 1, index - position_opening_symbol - 1));
                }
            }
            break;
        }
        default:;
        }
    }
    return nullptr;
}

void JsonReader::read(const std::u16string_view& json, float& ref)
{
    ref = json_to_float(json);
}

void JsonReader::read(const std::u16string_view& json, atlas_tile_t& ref)
{
    const auto s = read_json_pair(json);
    auto& texture = ref.texture;
    Logger::instance().info("pair size: {} {}", s->size(),utf16_to_cp1251(std::u16string(json)));
    read((*s)[0], texture.x);
    read((*s)[1], texture.y);
    read((*s)[2], texture.w);
    read((*s)[3], texture.h);
    Logger::instance().info("floooooaaat: {}", texture.h);
    delete s;
}

void JsonReader::read(const std::u16string_view& json, std::u16string& ref)
{
    const auto start_pos = json.find(u'"');
    if (start_pos != std::string::npos) {
        const auto end_pos = json.find(u'"', start_pos + 1);
        if (end_pos != std::string::npos) {
            ref = std::u16string(json.substr(start_pos + 1, end_pos - start_pos - 1));
        }
    }
}
