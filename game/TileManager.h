#ifndef TILEMANAGER_H
#define	TILEMANAGER_H

#include <string>
#include "graphics/gl.hpp"
#include "Application.h"


class GameObject;

class TileManager
{
public:
	GLuint m_unit;

	TileManager();
	~TileManager();

	virtual bool load_from_file(const std::string& filename);
	virtual void set_tile(tile_t& tile, GameObject* obj, unsigned int num);
};

class TileManager_Single :
	public TileManager
{
public:

	TileManager_Single();

	virtual void set_tile(tile_t& tile, GameObject* obj, unsigned int num);
};

class TileManager_Map :
	public TileManager
{
public:

	TileManager_Map();

	virtual void set_tile(tile_t& tile, GameObject* obj, unsigned int num);
};

#endif //TILEMANAGER_H