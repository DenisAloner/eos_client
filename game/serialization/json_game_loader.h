#ifndef JSON_GAME_LOADER_H
#define JSON_GAME_LOADER_H

//#include "member_map.h"
//#include "reader.h"

//class JsonGameLoader : public JsonReader {
//public:
//    using JsonReader::visit;
//
//    iSerializable* fabric(const std::u16string& type_name);
//	
//    template <typename T>
//    constexpr void
//    read_object(T& object, const std::u16string_view& json)
//    {
//        const auto properties = read_json_object(json);
//        if (properties) {
//            static_for<object_properties<T>>([&](auto i) {
//                constexpr auto item = std::get<i>(object_properties<T>());
//                using property_type = decltype(item);
//                if constexpr (std::is_base_of_v<iCustomHandler, property_type>) {
//                    (*this.*(item.custom_function))(object.*(item.property.member), (*properties)[item.property.name]);
//                } else if constexpr (std::is_pointer_v<typename property_type::type>) {
//                    if constexpr (std::is_base_of_v<iJsonDeserializable, std::remove_pointer_t<typename property_type::type>>) {
//                        //auto nested_object = parse_json_object((*properties)[item.name]);
//                        //if (nested_object) {
//                        //	
//                        //    delete nested_object;
//                        //}
//                        read(object.*(item.member), (*properties)[item.name]);
//                    } else
//                        read(object.*(item.member), (*properties)[item.name]);
//                } else if constexpr (std::is_reference_v<typename property_type::type>) {
//                    if constexpr (std::is_base_of_v<iJsonDeserializable, std::remove_reference_t<typename property_type::type>>) {
//                        read(object.*(item.member), (*properties)[item.name]);
//                    } else
//                        read(object.*(item.member), (*properties)[item.name]);
//                } else
//                    read(object.*(item.member), (*properties)[item.name]);
//            });
//            delete properties;
//        }
//    }
//
//    explicit JsonGameLoader(GameWorld& game_world);
//
//    /*  void read(GameWorld& ref, const std::u16string_view& json) override;
//    void read(GameWorld*& ref, const std::u16string_view& json) override;*/
//    /* void read(GameMap& ref, const std::u16string_view& json) override;
//    void read(GameMap*& ref, const std::u16string_view& json) override;*/
//    void read(GameObject& ref, const std::u16string_view& json) override;
//    void read(GameObject*& ref, const std::u16string_view& json) override;
//    void read(Object_state& ref, const std::u16string_view& json) override;
//    void read(Object_state*& ref, const std::u16string_view& json) override;
//
//    template <typename T>
//    constexpr static auto object_properties();
//
//private:
//    GameWorld& game_world_;
//};

#endif