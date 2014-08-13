#include "game/TileManager.h"
#include "game/GameObject.h"


TileManager::TileManager()
{
}


TileManager::~TileManager()
{
}


bool TileManager::load_from_file(const std::string& filename, ObjectDirection direction, int frame)
{
	m_tiles[get_tile_index(direction,frame)] = Application::instance().m_graph->load_texture(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".bmp");
	return true;
}


void  TileManager::set_tile(tile_t& tile, GameObject* obj, int frame, int shift)
{
	tile.unit = m_tiles[get_tile_index(obj->m_direction, frame)];
	double sx = 1.0 / obj->m_tile_size.w;
	tile.coordinat[0] = (shift - 1)*sx;
	tile.coordinat[1] = 1.0;
	tile.coordinat[2] = shift*sx;
	tile.coordinat[3] = 0.0;
}


TileManager_Single::TileManager_Single()
{
	m_tiles = new GLuint[1];
}

int TileManager_Single::get_tile_index(const ObjectDirection& direction, const int& frame)
{
	return 0;
}


TileManager_Map::TileManager_Map()
{
	m_tiles = new GLuint[16];
}

int TileManager_Map::get_tile_index(const ObjectDirection& direction, const int& frame)
{
	return direction*4+frame;
}


TileManager_rotating::TileManager_rotating()
{
	m_tiles = new GLuint[4];
}

int TileManager_rotating::get_tile_index(const ObjectDirection& direction, const int& frame)
{
	return direction;
}
