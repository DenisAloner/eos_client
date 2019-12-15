#include "writer.h"

void JsonWriter::visit(int& value)
{
    m_result = cp1251_to_utf16(std::to_string(value));
}

void JsonWriter::visit(int* value)
{
    if (!value) {
        m_result = u"null";
        return;
    }
    visit(*value);
}

void JsonWriter::visit(const int& value)
{
    m_result = cp1251_to_utf16(std::to_string(value));
}

void JsonWriter::visit(const int* value)
{
    if (!value) {
        m_result = u"null";
        return;
    }
    visit(*value);
}

void JsonWriter::visit(float& value)
{
    m_result = cp1251_to_utf16(std::to_string(value));
}

void JsonWriter::visit(float* value)
{
    if (!value) {
        m_result = u"null";
        return;
    }
    visit(*value);
}

void JsonWriter::visit(std::u16string& value)
{
    m_result = u"\"" + value + u"\"";
}

void JsonWriter::visit(std::u16string* value)
{
    if (!value) {
        m_result = u"null";
        return;
    }
    visit(*value);
}

void JsonWriter::visit(const std::u16string& value)
{
    m_result = u"\"" + value + u"\"";
}

void JsonWriter::visit(const std::u16string* value)
{
    if (!value) {
        m_result = u"null";
        return;
    }
    visit(*value);
}

void JsonWriter::visit(dimension3_t& value)
{
    auto result = u"["s;
    visit(value.dx);
    result += m_result;
    visit(value.dy);
    result += m_result;
    visit(value.dz);
    result += m_result;
    m_result = result + u"]"s;
}

void JsonWriter::visit(dimension3_t* value)
{
    if (!value) {
        m_result = u"null";
        return;
    }
    visit(*value);
}

void JsonWriter::visit(optical_properties_t& value)
{
	visit(value.attenuation);
}

void JsonWriter::visit(optical_properties_t* value)
{
    if (!value) {
        m_result = u"null";
        return;
    }
    visit(*value);
}
