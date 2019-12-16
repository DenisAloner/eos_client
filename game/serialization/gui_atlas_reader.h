#ifndef GUI_ATLAS_READER_H
#define GUI_ATLAS_READER_H

#include "member_map.h"
#include "reader.h"

class GraphicalController;

using namespace std::literals::string_view_literals;

class GuiAtlasReader : public JsonReader {
public:
	
    using JsonReader::visit;

    template <typename T>
    constexpr void
    read_object(T& object)
    {
        const auto properties = read_json_object(m_json);
        if (properties) {
            static_for<object_properties<T>>([&](auto i) {
                constexpr auto item = std::get<i>(object_properties<T>());
                using property_type = decltype(item);
                if constexpr (std::is_base_of_v<iCustomHandler, property_type>) {
                    m_json = (*properties)[item.property.name];
                    (*this.*(item.custom_function))(object.*(item.property.member));
                } else if constexpr (std::is_pointer_v<typename property_type::type>) {
                    m_json = (*properties)[item.name];
                    if constexpr (std::is_base_of_v<IVisitable, std::remove_pointer_t<typename property_type::type>>) {
                        auto value = object.*(item.member);
                        if (m_json != u"null") {
                            value->accept_pointer(*this);
                        } else {
                            value = nullptr;
                        }
                    } else
                        visit(object.*(item.member));
                } else if constexpr (std::is_reference_v<typename property_type::type>) {
                    m_json = (*properties)[item.property.name];
                    if constexpr (std::is_base_of_v<IVisitable, std::remove_reference_t<typename property_type::type>>) {
                        auto value = object.*(item.member);
                        value.accept_reference(*this);
                    } else
                        visit(object.*(item.member));
                } else {
                    m_json = (*properties)[item.name];
                    visit(object.*(item.member));
                }
            });
            delete properties;
        }
    }

    explicit GuiAtlasReader(GraphicalController& graph);
    void load();

    void visit(gui_style_t& ref) override;
    void visit(GraphicalController& ref) override;
    virtual void visit(GraphicalController*& ref);

    void load_tiles(std::unordered_map<std::u16string, atlas_tile_t>& ref);
    void load_tile_ref(atlas_tile_t*& ref);

    template <typename T>
    constexpr static auto object_properties();

private:
    GraphicalController& graph_;
};

#endif
