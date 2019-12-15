#include "json_game_loader.h"
//#include "member_map.h"

//iSerializable* JsonGameLoader::fabric(const std::u16string& type_name)
//{
//    if (MemberMap::type<GameObject> == type_name) {
//        return nullptr;
//    }
//    return nullptr;
//}
//
//JsonGameLoader::JsonGameLoader(GameWorld& game_world)
//    : game_world_(game_world)
//{
//}
//
//template <typename T>
//constexpr auto JsonGameLoader::object_properties()
//{
//    if constexpr (std::is_same_v<T, GameWorld>) {
//        return std::tuple(
//            MemberMap::get<&GameWorld::m_maps>,
//            MemberMap::get<&GameWorld::m_objects>);
//    }
//    if constexpr (std::is_same_v<T, GameMap>) {
//        return std::tuple(
//            MemberMap::get<&GameMap::m_size>);
//    }
//    if constexpr (std::is_same_v<T, GameObject>) {
//        return std::tuple(
//            MemberMap::get<&GameObject::m_name>,
//            MemberMap::get<&GameObject::m_active_state>);
//    }
//}
//
////void JsonGameLoader::read(GameWorld& ref, const std::u16string_view& json)
////{
////    read_object(ref, json);
////}
////
////void JsonGameLoader::read(GameWorld*& ref, const std::u16string_view& json)
////{
////    if (json == u"null") {
////        ref = nullptr;
////    } else {
////        read(*ref, json);
////    }
////}
//
////void JsonGameLoader::read(GameMap& ref, const std::u16string_view& json)
////{
////    read_object(ref, json);
////}
////
////void JsonGameLoader::read(GameMap*& ref, const std::u16string_view& json)
////{
////    if (json == u"null") {
////        ref = nullptr;
////    } else {
////        read(*ref, json);
////    }
////}
//
//void JsonGameLoader::read(GameObject& ref, const std::u16string_view& json)
//{
//    read_object(ref, json);
//}
//
//void JsonGameLoader::read(GameObject*& ref, const std::u16string_view& json)
//{
//    if (json == u"null") {
//        ref = nullptr;
//    } else {
//        read(*ref, json);
//    }
//}
//
//void JsonGameLoader::read(Object_state& ref, const std::u16string_view& json)
//{
//    Logger::instance().info("Object_state&");
//}
//
//void JsonGameLoader::read(Object_state*& ref, const std::u16string_view& json)
//{
//    Logger::instance().info("Object_state*&");
//}
