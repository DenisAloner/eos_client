#include "game/TileManager.h"
#include "game/GameObject.h"


TileManager::TileManager()
{
}


TileManager::~TileManager()
{
}


bool TileManager::load_from_file(const std::string& filename)
{
	m_unit = Application::instance().m_graph->load_texture(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".bmp");
	return true;
}


void  TileManager::set_tile(tile_t& tile, GameObject* obj, unsigned int num)
{
}


TileManager_Single::TileManager_Single()
{
}


void  TileManager_Single::set_tile(tile_t& tile, GameObject* obj, unsigned int num)
{
	tile.unit = m_unit;
	tile.coordinat[0] = 0.0;
	tile.coordinat[1] = 1.0;
	tile.coordinat[2] = 0.0;
	tile.coordinat[3] = 0.0;
	tile.coordinat[4] = 1.0;
	tile.coordinat[5] = 0.0;
	tile.coordinat[6] = 1.0;
	tile.coordinat[7] = 1.0;
}


TileManager_Map::TileManager_Map()
{
}


void  TileManager_Map::set_tile(tile_t& tile, GameObject* obj, unsigned int num)
{
	tile.unit = m_unit;
	tile.coordinat[0] = (num*obj->m_size.x * 32) / 256.0;
	tile.coordinat[1] = ((obj->m_direction + 1)*obj->m_size.z * 16) / 384.0;
	tile.coordinat[2] = (num*obj->m_size.x * 32)/256.0;
	tile.coordinat[3] = (obj->m_direction*obj->m_size.z * 16)/384.0;
	tile.coordinat[4] = ((num + 1)*obj->m_size.x * 32) / 256.0;
	tile.coordinat[5] = (obj->m_direction*obj->m_size.z * 16)/384.0;
	tile.coordinat[6] = ((num+1)*obj->m_size.x * 32) / 256.0;
	tile.coordinat[7] = ((obj->m_direction + 1)*obj->m_size.z * 16) / 384.0;
	//tile.coordinat[0] = 0.0;
	//tile.coordinat[1] = 0.5;
	//tile.coordinat[2] = 0.0;
	//tile.coordinat[3] = 0.0;
	//tile.coordinat[4] = 0.5;
	//tile.coordinat[5] = 0.0;
	//tile.coordinat[6] = 0.5;
	//tile.coordinat[7] = 0.5;
}
