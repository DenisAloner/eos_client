#include "game/TileManager.h"
#include "game/GameObject.h"


void TileManager::vector_GLuint_from_json(std::u16string value, std::vector<GLuint>& prop)
{
	std::u16string temp = value;
	scheme_list_t* s = Parser::read_array(temp);
	if (s)
	{
		prop.resize(s->size());
		int i = 0;
		for (auto element : (*s))
		{
			std::string&& name = Parser::UTF16_to_CP866(Parser::get_value(element));
			prop[i] = Application::instance().m_graph->png_texture_load(
				FileSystem::instance().m_resource_path + "Tiles\\" + name + ".png");
			i += 1;
		}
		delete s;
	}
}

void TileManager::vector_GLuint_from_binary(const std::string& value, std::vector<GLuint>& prop, std::size_t& pos)
{
}

TileManager::TileManager():m_animation(animation_e::idle)
{
}


TileManager::~TileManager()
{
}


bool TileManager::init(const std::string& filename, object_direction_e direction, int frame)
{
	m_tiles.resize(1);
	m_tiles[0] = Application::instance().m_graph->png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".png");
	return true;
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
}

int TileManager_Single::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return 0;

}

bool TileManager_Single::init(const std::string& filename, object_direction_e direction, int frame)
{
	m_tiles.resize(1);
	m_tiles[0] = Application::instance().m_graph->png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".png");
	return true;
}

TileManager_rotate8_animate::TileManager_rotate8_animate()
{
}

int TileManager_rotate8_animate::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return 0;

}

bool TileManager_rotate8_animate::init(const std::string& filename, object_direction_e direction, int frame)
{
	m_frame = frame;
	m_tiles.resize(2);
	for(int i=0;i<2;++i)
	{
		m_tiles[i] = Application::instance().m_graph->png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\" + filename + std::to_string(i + 1) + ".png");
	}
	return true;
}

void TileManager_rotate8_animate::set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction)
{
	tile.unit = m_tiles[static_cast<int>(m_animation)];
	int ix = frame * (m_frame / 24.0);
	int iy = int(direction);
	double dx = 1.0 / m_frame;
	double dy = 1.0 / 8.0;
	tile.coordinat[0] = ix*dx;
	tile.coordinat[1] = 1.0 - ((iy)*dy);
	tile.coordinat[2] = (ix + 1)*dx;
	tile.coordinat[3] = 1.0 - ((iy + 1)*dy);
}

TileManager_equilateral_animate::TileManager_equilateral_animate()
{
}

int TileManager_equilateral_animate::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return 0;

}

bool TileManager_equilateral_animate::init(const std::string& filename, object_direction_e direction, int frame)
{
	m_frame = frame;
	m_tiles.resize(1);
	m_tiles[0] = Application::instance().m_graph->png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".png");
	return true;
}

void TileManager_equilateral_animate::set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction)
{
	tile.unit = m_tiles[static_cast<int>(m_animation)];
	int ix = frame * (m_frame / 24.0);
	double dx = 1.0 / m_frame;
	tile.coordinat[0] = ix*dx;
	tile.coordinat[1] = 1.0;
	tile.coordinat[2] = (ix + 1)*dx;
	tile.coordinat[3] = 0.0;
}