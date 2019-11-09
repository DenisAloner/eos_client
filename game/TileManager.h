#ifndef TILEMANAGER_H
#define TILEMANAGER_H

#include "graphics/gl.hpp"
#include <string>
#include "Definiton.h"

class TileManager : public iSerializable, public InstanceDictionaryElement {
public:
    std::vector<GLuint> m_tiles;
    animation_e m_animation;
    int m_layer;

    TileManager();
    ~TileManager();

    //virtual bool init(const std::string& filename, object_direction_e direction, int frame);

    virtual void set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction);
    virtual int get_tile_index(const object_direction_e& direction, const int& frame) = 0;
    virtual void set_tile(quad_t& quad, GameObject* obj, int pos, const object_direction_e& direction);

    //Packer_generic& get_packer() override
    //{
    //	return Packer<Object_interaction>::Instance();
    //}

    void vector_gl_uint_from_json(const std::u16string& value, std::vector<GLuint>& prop, SerializationContext& context);
    void vector_gl_uint_from_binary(const std::string& value, std::vector<GLuint>& prop, std::size_t& pos, SerializationContext& context);

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&TileManager::m_tiles, u"tiles").from_json(&TileManager::vector_gl_uint_from_json).from_binary(&TileManager::vector_gl_uint_from_binary));
    }
};

class TileManager_Single_Atlas : public TileManager {
public:
    rectangle_t<int> m_frame;

    TileManager_Single_Atlas();
    //bool init(const std::string& filename, object_direction_e direction, int frame) override;
    int get_tile_index(const object_direction_e& direction, const int& frame) override;
    void set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction) override;
    void set_tile(quad_t& quad, GameObject* obj, int pos, const object_direction_e& direction) override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&TileManager_Single_Atlas::m_frame, u"frame"));
    }
};

class TileManager_Atlas_Rotatable : public TileManager {
public:
    rectangle_t<int> m_frame;

    TileManager_Atlas_Rotatable();
    //bool init(const std::string& filename, object_direction_e direction, int frame) override;
    int get_tile_index(const object_direction_e& direction, const int& frame) override;
    void set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction) override;
    void set_tile(quad_t& quad, GameObject* obj, int pos, const object_direction_e& direction) override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&TileManager_Atlas_Rotatable::m_frame, u"frame"));
    }
};

class TileManager_Single : public TileManager {
public:
    TileManager_Single();
    //bool init(const std::string& filename, object_direction_e direction, int frame) override;
    int get_tile_index(const object_direction_e& direction, const int& frame) override;

    iPacker& get_packer() override;

    constexpr static auto properties() { return TileManager::properties(); }
};

class TileManager_rotate8_animate : public TileManager {
public:
    int m_frame;

    TileManager_rotate8_animate();
    //bool init(const std::string& filename, object_direction_e direction, int frame) override;
    int get_tile_index(const object_direction_e& direction, const int& frame) override;
    void set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction) override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return make_union(
            TileManager::properties(),
            std::make_tuple(
                make_property(&TileManager_rotate8_animate::m_frame, u"frame")));
    }
};

class TileManager_equilateral_animate : public TileManager {
public:
    int m_frame;

    TileManager_equilateral_animate();
    //bool init(const std::string& filename, object_direction_e direction, int frame) override;
    int get_tile_index(const object_direction_e& direction, const int& frame) override;
    void set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction) override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return make_union(
            TileManager::properties(),
            std::make_tuple(
                make_property(&TileManager_equilateral_animate::m_frame, u"frame")));
    }
};

#endif //TILEMANAGER_H