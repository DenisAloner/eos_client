#include "writer.h"

std::u16string JsonWriter::write(int& value)
{
    return cp1251_to_utf16(std::to_string(value));
}

std::u16string JsonWriter::write(const int& value)
{
    return cp1251_to_utf16(std::to_string(value));
}

std::u16string JsonWriter::write(std::u16string& value)
{
    return u"\"" + value + u"\"";
}

std::u16string JsonWriter::write(const std::u16string& value)
{
    return u"\"" + value + u"\"";
}

std::u16string JsonWriter::write(dimension3_t& value)
{
    return u"[" + write(value.dx) + u"," + write(value.dy) + u"," + write(value.dz) + u"]";
}

std::u16string JsonWriter::write(const dimension3_t& value)
{
    return u"[" + write(value.dx) + u"," + write(value.dy) + u"," + write(value.dz) + u"]";
}
