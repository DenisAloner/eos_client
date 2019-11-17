#ifndef JSON_GAME_SAVER_H
#define JSON_GAME_SAVER_H

#include "writer.h"
#include <string>

class JsonGameSaver : public JsonWriter {
public:
    using JsonWriter::write;

    template <typename T>
    constexpr static auto object_properties();

    template <typename T>
    constexpr std::u16string
    write_object(T& object)
    {
        std::u16string result;
        auto ref = objects.find(&object);
        if (ref == objects.end()) {
            result = u"{";
        } else
            result = u"{\"$link\":" + cp1251_to_utf16(std::to_string(ref->second)) + u",";
        constexpr auto props = std::tuple_size<decltype(object_properties<T>())>::value;
        for_sequence_json(std::make_index_sequence<props> {}, [&](auto i) {
            constexpr auto property = std::get<i>(object_properties<T>());
            if (i != 0) {
                result += u",";
            }
            if constexpr (property.custom_function) {
                result += u"\"" + std::u16string { property.name } + u"\":" + (*this.*(property.custom_function.value()))(object.*(property.member));
            } else
                result += u"\"" + std::u16string { property.name } + u"\":" + write(object.*(property.member));
        });
        return result + u'}';
    }

    std::u16string write(GameWorld& value) override;
    std::u16string write(GameMap& value) override;
    std::u16string write(GameObject& value) override;
    std::u16string write(Attribute_map& value) override;
    std::u16string write(Object_state& value) override;
    std::u16string write(Interaction_list& value) override;
    std::u16string write(MapCell& value) override;
    std::u16string write(InventoryCell& value) override;
    std::u16string write(ObjectPart& value) override;

    std::u16string write(Game_object_owner* value) override;
    std::u16string write(MapCell* value) override;
    std::u16string write(GameWorld* value) override;
    std::u16string write(GameMap* value) override;
    std::u16string write(GameObject* value) override;
    std::u16string write(Attribute_map* value) override;
    std::u16string write(Object_state* value) override;
    std::u16string write(Interaction_list* value) override;
    std::u16string write(InventoryCell* value) override;
    std::u16string write(ObjectPart* value) override;

	std::u16string map_cell_owner_save(GameMap* value);
};

#endif
