#include "game/TileManager.h"
#include "game/GameObject.h"


TileManager::TileManager()
{
}


TileManager::~TileManager()
{
}


bool TileManager::load_from_file(const std::string& filename, object_direction_e direction, int frame)
{
	m_tiles[get_tile_index(direction,frame)] = Application::instance().m_graph->load_texture(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".bmp");
	return true;
}


bool TileManager::load_from_file(const std::string& filename, object_direction_e direction, int frame, std::string& ext)
{
	LOG(FATAL) << "Метод не описан";
	return false;
}

void TileManager::set_tile(tile_t& tile, GameObject* obj, int frame, int shift)
{
	tile.unit = m_tiles[get_tile_index(obj->m_direction, frame)];
	double sx = 1.0 / obj->m_active_state->m_tile_size.w;
	tile.coordinat[0] = 0.0;
	tile.coordinat[1] = 1.0;
	tile.coordinat[2] = 1.0;
	tile.coordinat[3] = 0.0;
}

void TileManager::set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction)
{
	tile.unit = m_tiles[get_tile_index(direction, frame)];
	double sx = 1.0 / obj->m_active_state->m_tile_size.w;
	tile.coordinat[0] = 0.0;
	tile.coordinat[1] = 1.0;
	tile.coordinat[2] = 1.0;
	tile.coordinat[3] = 0.0;
}

TileManager_Single::TileManager_Single()
{
	m_tiles = new GLuint[1];
}

int TileManager_Single::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return 0;
}


TileManager_Single_animate::TileManager_Single_animate(int frame):m_frame(frame)
{
	m_tiles = new GLuint[frame];
}

bool TileManager_Single_animate::load_from_file(const std::string& filename, object_direction_e direction, int frame)
{
	m_tiles[frame] = Application::instance().m_graph->load_texture(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".bmp");
	return true;
}

bool TileManager_Single_animate::load_from_file(const std::string& filename, object_direction_e direction, int frame, std::string& ext)
{
	m_tiles[frame] = Application::instance().m_graph->png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".png");
	return true;
}

int TileManager_Single_animate::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return frame * (m_frame/8.0);
}


TileManager_Map::TileManager_Map()
{
	m_tiles = new GLuint[32];
}

int TileManager_Map::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return static_cast<int>(direction) * 4 + frame / 7.0*3.0;
}


TileManager_rotating::TileManager_rotating()
{
	m_tiles = new GLuint[4];
}

int TileManager_rotating::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return static_cast<int>(direction)/2;
}

TileManager_rotating8::TileManager_rotating8()
{
	m_tiles = new GLuint[8];
}

int TileManager_rotating8::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return static_cast<int>(direction);
}