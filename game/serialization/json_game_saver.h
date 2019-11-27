#ifndef JSON_GAME_SAVER_H
#define JSON_GAME_SAVER_H

#include "writer.h"
#include <string>
#include "member_map.h"

class JsonGameSaver : public JsonWriter {
public:
    using JsonWriter::write;

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
        static_for<object_properties<T>>([&](auto i) {
            constexpr auto item = std::get<i>(object_properties<T>());
            if (i != 0) {
                result += u",";
            }
            if constexpr (std::is_base_of_v<iCustomHandler, decltype(item)>) {
                result += u"\"" + std::u16string { item.property.name } + u"\":" + (*this.*(item.custom_function))(object.*(item.property.member));
            } else
                result += u"\"" + std::u16string { item.name } + u"\":" + write(object.*(item.member));
        });
        return result + u'}';
    }

    std::u16string write(GameWorld& value) override;
    std::u16string write(GameWorld* value) override;
  
    std::u16string write(GameMap& value) override;
    std::u16string write(GameMap* value) override;
    std::u16string write(GameObject& value) override;
    std::u16string write(GameObject* value) override;
    std::u16string write(Attribute_map& value) override;
    std::u16string write(Attribute_map* value) override;
    std::u16string write(Object_state& value) override;
    std::u16string write(Object_state* value) override;
    std::u16string write(Interaction_list& value) override;
    std::u16string write(Interaction_list* value) override;
    std::u16string write(MapCell& value) override;
    std::u16string write(MapCell* value) override;
    std::u16string write(InventoryCell& value) override;
    std::u16string write(InventoryCell* value) override;
    std::u16string write(ObjectPart& value) override;
    std::u16string write(ObjectPart* value) override;
    std::u16string write(AI& value) override;
    std::u16string write(AI* value) override;
    std::u16string write(AI_enemy& value) override;
    std::u16string write(AI_enemy* value) override;
    std::u16string write(predicate_t& value) override;
    std::u16string write(predicate_t* value) override;

	std::u16string write(Game_object_owner* value) override;
	
    std::u16string map_cell_owner_save(GameMap* value);

	template <typename T>
    constexpr static auto object_properties();
};

#endif
