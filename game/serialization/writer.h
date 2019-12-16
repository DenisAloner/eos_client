#ifndef WRITER_H
#define WRITER_H

#include "visitor.h"
#include <unordered_map>
#include "Definiton.h"

class JsonWriter : public IVisitor {
public:
	
    using IVisitor::visit;
    unsigned long counter = 0;
    std::unordered_map<iSerializable*, long> objects;

    std::u16string m_result;

    VISIT_OVERLOAD(int);
    VISIT_OVERLOAD(const int);
    VISIT_OVERLOAD(float);
    VISIT_OVERLOAD(std::u16string);
    VISIT_OVERLOAD(const std::u16string);
    VISIT_OVERLOAD(dimension3_t);
    VISIT_OVERLOAD(optical_properties_t);

    template <typename T, class = typename std::enable_if<std::is_enum<T>::value, T>::type>
    void visit(const T& value)
    {
        m_result = u"\"" + cp1251_to_utf16(Dictionaries::get_dictionary<T>().get_string(value)) + u"\"";
    }

    template <typename T>
    void visit(std::vector<T>& ref)
    {
        if (ref.empty()) {
            m_result = u"[]"s;
            return;
        }
        auto result = u"["s;
        if constexpr (std::is_pointer_v<T> && std::is_base_of_v<IVisitable, std::remove_pointer_t<T>>) {
            for (auto& current : ref) {
                if (current) {
                    current->accept_pointer(*this);
                    result += m_result + u","s;
                } else {
                    result += u"null,"s;
                }
            }
        } else
            for (auto& current : ref) {
                visit(current);
                result += m_result + u","s;
            }
        result[result.length() - 1] = u']';
        m_result = result;
    }

    template <typename T>
    void visit(std::list<T>& ref)
    {
        if (ref.empty()) {
            m_result = u"[]"s;
            return;
        }
        auto result = u"["s;
        if constexpr (std::is_pointer_v<T> && std::is_base_of_v<IVisitable, std::remove_pointer_t<T>>) {
            for (auto& current : ref) {
                if (current) {
                    current->accept_pointer(*this);
                    result += m_result + u","s;
                } else {
                    result += u"null,"s;
                }
            }
        } else
            for (auto& current : ref) {
                visit(current);
                result += m_result + u","s;
            }
        result[result.length() - 1] = u']';
        m_result = result;
    }

    template <typename Key, typename Value>
    void visit(std::unordered_map<Key, Value>& ref)
    {
        if (ref.empty()) {
            m_result = u"[]"s;
            return;
        }
        auto result = u"["s;
        if constexpr (std::is_pointer_v<Value> && std::is_base_of_v<IVisitable, std::remove_pointer_t<Value>>) {
            for (auto& current : ref) {
                visit(current.first);
                result += m_result + u","s;
                if (current.second) {
                    current.second->accept_pointer(*this);
                    result += m_result + u","s;
                } else {
                    result += u"null,"s;
                }
            }
        } else
            for (auto& current : ref) {
                visit(current.first);
                result += m_result + u","s;
                visit(current.second);
                result += m_result + u","s;
            }
        result[result.length() - 1] = u']';
        m_result = result;
    }

    template <typename Key, typename Value>
    void visit(std::map<Key, Value>& ref)
    {
        if (ref.empty()) {
            m_result = u"[]"s;
            return;
        }
        auto result = u"["s;
        if constexpr (std::is_pointer_v<Value> && std::is_base_of_v<IVisitable, std::remove_pointer_t<Value>>) {
            for (auto& current : ref) {
                visit(current.first);
                result += m_result + u","s;
                if (current.second) {
                    current.second->accept_pointer(*this);
                    result += m_result + u","s;
                } else {
                    result += u"null,"s;
                }
            }
        } else
            for (auto& current : ref) {
                visit(current.first);
                result += m_result + u","s;
                visit(current.second);
                result += m_result + u","s;
            }
        result[result.length() - 1] = u']';
        m_result = result;
    }

    template <typename T>
    void visit(rgb_t<T>& ref)
    {
        auto result = u"["s;
        visit(ref.r);
        result += m_result + u',';
        visit(ref.g);
        result += m_result + u',';
        visit(ref.b);
        result += m_result;
        m_result = result + u"]"s;
    }

    template <typename T>
    void visit(rgb_t<T>* value)
    {
        if (!value) {
            m_result = u"null"s;
            return;
        }
        visit(*value);
    }

    template <typename T>
    void write_pointer(T* value)
    {
        if (!value) {
            m_result = u"null"s;
            return;
        }
        auto ref = objects.find(value);
        if (ref == objects.end()) {
            counter += 1;
            objects[value] = counter;
            visit(*value);
            return;
        }
        m_result = u"{\"$type\":\"link\",\"$link\":" + cp1251_to_utf16(std::to_string(ref->second)) + u"}";
    }
};

#endif //WRITER_H