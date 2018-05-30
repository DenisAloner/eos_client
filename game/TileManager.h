#ifndef TILEMANAGER_H
#define	TILEMANAGER_H

#include <string>
#include "graphics/gl.hpp"
#include "Application.h"

class TileManager : public iSerializable, public InstanceDictonaryElement
{
public:

	std::vector<GLuint> m_tiles;
	animation_e m_animation;

	TileManager();
	~TileManager();

	virtual bool init(const std::string& filename, object_direction_e direction, int frame);
	virtual void set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction);
	virtual int get_tile_index(const object_direction_e& direction, const int& frame) = 0;

	void save() override {};
	void load() override {};

	//Packer_generic& get_packer() override
	//{
	//	return Packer<Object_interaction>::Instance();
	//}

	void vector_GLuint_from_json(std::u16string value, std::vector<GLuint>& prop, Parser_context& context);
	void vector_GLuint_from_binary(const std::string& value, std::vector<GLuint>& prop, std::size_t& pos, Parser_context& context);

	constexpr static auto properties() 
	{
		return std::make_tuple(
			make_property(&TileManager::m_tiles, u"tiles").from_json(&TileManager::vector_GLuint_from_json).from_binary(&TileManager::vector_GLuint_from_binary)
		);
	}

};

class TileManager_Single :
	public TileManager
{
public:

	TileManager_Single();
	bool init(const std::string& filename, object_direction_e direction, int frame) override;
	int get_tile_index(const object_direction_e& direction, const int& frame) override;

	Packer_generic& get_packer() override;

	constexpr static auto properties() { return TileManager::properties(); }

};

class TileManager_rotate8_animate :
	public TileManager
{
public:

	int m_frame;

	TileManager_rotate8_animate();
	bool init(const std::string& filename, object_direction_e direction, int frame) override;
	int get_tile_index(const object_direction_e& direction, const int& frame) override;
	void set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction) override;

	Packer_generic& get_packer() override;

	constexpr static auto properties() {
		return make_union(
			TileManager::properties(),
			std::make_tuple(
				make_property(&TileManager_rotate8_animate::m_frame, u"frame")
			));
	}
};

class TileManager_equilateral_animate :
	public TileManager
{
public:

	int m_frame;

	TileManager_equilateral_animate();
	bool init(const std::string& filename, object_direction_e direction, int frame) override;
	int get_tile_index(const object_direction_e& direction, const int& frame) override;
	void set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction) override;

	Packer_generic& get_packer() override;

	constexpr static auto properties() 
	{
		return make_union(
			TileManager::properties(),
			std::make_tuple(
				make_property(&TileManager_equilateral_animate::m_frame, u"frame")
			));
	}

};

#endif //TILEMANAGER_H