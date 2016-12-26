#ifndef TILEMANAGER_H
#define	TILEMANAGER_H

#include <string>
#include "graphics/gl.hpp"
#include "Application.h"


class GameObject;

class TileManager
{
public:

	GLuint* m_tiles;
	size_t m_index;
	animation_e m_animation;

	TileManager();
	~TileManager();

	virtual bool load_from_file(const std::string& filename, object_direction_e direction, int frame);
	virtual bool load_from_file(const std::string& filename, object_direction_e direction, int frame,std::string& ext);
	virtual void set_tile(tile_t& tile, GameObject* obj, int frame, int shift);
	virtual void set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction);
	virtual int get_tile_index(const object_direction_e& direction, const int& frame) = 0;
};

class TileManager_Single :
	public TileManager
{
public:

	TileManager_Single();

	virtual int get_tile_index(const object_direction_e& direction, const int& frame);
};

class TileManager_Map :
	public TileManager
{
public:

	TileManager_Map();

	virtual int get_tile_index(const object_direction_e& direction, const int& frame);
};

class TileManager_rotating :
	public TileManager
{
public:

	TileManager_rotating();

	virtual int get_tile_index(const object_direction_e& direction, const int& frame);
};

class TileManager_rotating8 :
	public TileManager
{
public:

	TileManager_rotating8();

	virtual int get_tile_index(const object_direction_e& direction, const int& frame);
};

class TileManager_Single_animate :
	public TileManager
{
public:

	int m_frame;

	TileManager_Single_animate(int frame);
	bool load_from_file(const std::string& filename, object_direction_e direction, int frame) override;
	bool load_from_file(const std::string& filename, object_direction_e direction, int frame, std::string& ext) override;
	virtual int get_tile_index(const object_direction_e& direction, const int& frame);

};

class TileManager_Single_png :
	public TileManager
{
public:

	int m_frame;

	TileManager_Single_png();
	bool load_from_file(const std::string& filename, object_direction_e direction, int frame) override;
	virtual int get_tile_index(const object_direction_e& direction, const int& frame);

};

class TileManager_rotate8_animate :
	public TileManager
{
public:

	int m_frame;

	TileManager_rotate8_animate(int frame);
	bool load_from_file(const std::string& filename, object_direction_e direction, int frame) override;
	int get_tile_index(const object_direction_e& direction, const int& frame) override;
	void set_tile(tile_t& tile, GameObject* obj, int frame, int shift) override;

};

class TileManager_equilateral_animate :
	public TileManager
{
public:

	int m_frame;

	TileManager_equilateral_animate(int frame);
	bool load_from_file(const std::string& filename, object_direction_e direction, int frame) override;
	int get_tile_index(const object_direction_e& direction, const int& frame) override;
	void set_tile(tile_t& tile, GameObject* obj, int frame, int shift) override;

};

#endif //TILEMANAGER_H