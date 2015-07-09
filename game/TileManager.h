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

	TileManager();
	~TileManager();

	virtual bool load_from_file(const std::string& filename, object_direction_e direction, int frame);
	virtual void set_tile(tile_t& tile, GameObject* obj, int frame, int shift);
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

	TileManager_Single_animate();
	virtual int get_tile_index(const object_direction_e& direction, const int& frame);

};

#endif //TILEMANAGER_H