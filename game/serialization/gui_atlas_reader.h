#ifndef GUI_ATLAS_READER_H
#define GUI_ATLAS_READER_H

#include "member_map.h"
#include "reader.h"

class GraphicalController;

using namespace std::literals::string_view_literals;

class GuiAtlasReader : public JsonReader {
public:
    using JsonReader::read;

    template <typename T>
    constexpr void
    read_object(T& object, const std::u16string_view& json)
    {
        const auto properties = read_json_object(json);
        if (properties) {
            static_for<object_properties<T>>([&](auto i) {
                constexpr auto item = std::get<i>(object_properties<T>());
                using property_type = decltype(item);
                if constexpr (std::is_base_of_v<iCustomHandler, property_type>) {
                    (*this.*(item.custom_function))(object.*(item.property.member), (*properties)[item.property.name]);
                } else if constexpr (std::is_pointer_v<typename property_type::type>) {
                    if constexpr (std::is_base_of_v<iJsonSerializable, std::remove_pointer_t<typename property_type::type>>) {
                        auto value = object.*(item.member);
                        if ((*properties)[item.name] != u"null") {
                            value->deserialize_to_json_pointer(*this, (*properties)[item.name]);
                        } else {
                            value = nullptr;
                        }
                    } else
                        read(object.*(item.member), (*properties)[item.name]);
                } else if constexpr (std::is_reference_v<typename property_type::type>) {
                    if constexpr (std::is_base_of_v<iJsonSerializable, std::remove_reference_t<typename property_type::type>>) {
                        auto value = object.*(item.member);
                        value.deserialize_to_json_reference(*this, (*properties)[item.name]);
                    } else
                        read(object.*(item.member), (*properties)[item.name]);
                } else
                    read(object.*(item.member), (*properties)[item.name]);
            });
            delete properties;
        }
    }

    explicit GuiAtlasReader(GraphicalController& graph);
    void load();
	
    void read(gui_style_t& ref, const std::u16string_view& json) override;
    void read(GraphicalController& ref, const std::u16string_view& json) override;
    void read(GraphicalController* ref, const std::u16string_view& json) override;

    void load_tiles(std::unordered_map<std::u16string, atlas_tile_t>& ref, const std::u16string_view& json);
    void load_tile_ref(atlas_tile_t*& ref, const std::u16string_view& json);
	
    template <typename T>
    constexpr static auto object_properties();

private:
    GraphicalController& graph_;
};

#endif
