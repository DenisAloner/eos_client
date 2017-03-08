#ifndef TILEMANAGER_H
#define	TILEMANAGER_H

#include <string>
#include "graphics/gl.hpp"
#include "Application.h"


class TileManager: public iSerializable
{
public:

	std::vector<GLuint> m_tiles;
	size_t m_index;
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

	constexpr static auto properties() {
		return std::make_tuple(
			make_property<Parser::icon_t>(&TileManager::m_tiles, u"tiles")
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

	Packer_generic& get_packer() override
	{
		return Packer<TileManager_Single>::Instance();
	}

	constexpr static auto properties() {
		return TileManager::properties();
	}

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

	Packer_generic& get_packer() override
	{
		return Packer<TileManager_rotate8_animate>::Instance();
	}

	constexpr static auto properties() {
		return make_union(std::make_tuple(
			make_property(&TileManager_rotate8_animate::m_frame, u"frame")
		), TileManager::properties());
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

	Packer_generic& get_packer() override
	{
		return Packer<TileManager_equilateral_animate>::Instance();
	}

	constexpr static auto properties() {
		return make_union(std::make_tuple(
			make_property(&TileManager_equilateral_animate::m_frame, u"frame")
		), TileManager::properties());
	}

};

#endif //TILEMANAGER_H