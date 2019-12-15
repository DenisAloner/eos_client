#ifndef READER_H
#define READER_H

#include "game_world.h"
#include "gui_style.h"
#include <string>

typedef std::vector<std::u16string_view> json_vector_t;
typedef std::unordered_map<std::u16string_view, std::u16string_view> json_map_t;

int json_to_int(const std::u16string_view& value);
float json_to_float(const std::u16string_view& value);
std::u16string_view json_token(const std::u16string_view& value, std::size_t& i);
json_vector_t* parse_json_array(const std::u16string_view& value);
json_vector_t* read_json_array(const std::u16string_view& value);
json_map_t* parse_json_object(const std::u16string_view& value);
json_map_t* read_json_object(const std::u16string_view& value);

class JsonReader:public iVisitor {
public:

    using iVisitor::visit;
    std::u16string_view m_json;
	
    //template <typename T>
    //void visit(std::list<T>& ref)
    //{
    //    const auto array = read_json_array(m_json);
    //    if (array) {
    //        if constexpr (std::is_pointer_v<T> && std::is_base_of_v<iJsonSerializable, std::remove_pointer_t<T>>) {
    //            for (auto& current : ref) {
    //                result += (current ? current->deserialize_to_json_pointer(*this) : u"null") + u",";
    //            }
    //        } else
    //            for (auto& current : ref) {
    //                result += write(current) + u",";
    //            }
    //        delete array;
    //    }
    //}

    template <typename Key, typename Value>
    void visit(std::unordered_map<Key, Value>& ref)
    {
        const auto array = read_json_array(m_json);
        if (array) {
            if constexpr (std::is_pointer_v<Value> && std::is_base_of_v<iJsonSerializable, std::remove_pointer_t<Value>>) {
                for (auto& current : ref) {
                    for (std::size_t index = 0; index < array->size(); index += 2) {
                        Key key;
                        m_json = (*array)[index];
                        visit(key);
                        Value& value = ref[key];
                        m_json = (*array)[index + 1];
                        value->accept_pointer(*this);
                    }
                }
            } else
                for (std::size_t index = 0; index < array->size(); index += 2) {
                    Key key;
                    m_json = (*array)[index];
                    visit(key);
                    Value& value = ref[key];
                    m_json = (*array)[index + 1];
                    visit(value);
                }
            delete array;
        }
    }

    void visit(int& ref) override;
    virtual void visit(int*& ref);
    void visit(float& ref) override;
    void visit(std::u16string& ref) override;
    virtual void visit(atlas_tile_t& ref);

    void visit(gui_style_t& ref) override;
    void visit(GraphicalController& ref) override;
    virtual void visit(GraphicalController*& ref);
    /*virtual void read(GameWorld& ref, const std::u16string_view& json);
    virtual void read(GameWorld*& ref, const std::u16string_view& json);*/
    /*virtual void read(GameMap& ref, const std::u16string_view& json);
    virtual void read(GameMap*& ref, const std::u16string_view& json);*/
    void visit(GameObject& ref) override;
    virtual void visit(GameObject*& ref);
    void visit(Object_state& ref) override;
    virtual void visit(Object_state*& ref);
};

#endif