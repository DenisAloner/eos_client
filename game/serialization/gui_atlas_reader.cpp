#include "gui_atlas_reader.h"
#include <stb_image.h>

using namespace std::literals::string_view_literals;

template <typename T>
constexpr auto GuiAtlasReader::object_properties()
{
    if constexpr (std::is_same_v<T, GraphicalController>) {
        return std::tuple(
            CustomHandler(MemberMap::get<&GraphicalController::atlas_tiles>, &GuiAtlasReader::load_tiles),
            MemberMap::get<&GraphicalController::gui_styles>);
    }
    if constexpr (std::is_same_v<T, gui_style_t>) {
        return std::tuple(
            CustomHandler(MemberMap::get<&gui_style_t::background_tile>, &GuiAtlasReader::load_tile_ref),
            CustomHandler(MemberMap::get<&gui_style_t::border_x_tile>, &GuiAtlasReader::load_tile_ref),
            CustomHandler(MemberMap::get<&gui_style_t::border_y_tile>, &GuiAtlasReader::load_tile_ref),
            CustomHandler(MemberMap::get<&gui_style_t::corner_tile>, &GuiAtlasReader::load_tile_ref),
            CustomHandler(MemberMap::get<&gui_style_t::scroll_y_head_tile>, &GuiAtlasReader::load_tile_ref),
            CustomHandler(MemberMap::get<&gui_style_t::scroll_y_body_tile>, &GuiAtlasReader::load_tile_ref));
    }
}

GuiAtlasReader::GuiAtlasReader(GraphicalController& graph)
    : graph_(graph)
{
}

void GuiAtlasReader::load()
{
    bytearray json;
    FileSystem::instance().load_from_file(FileSystem::instance().m_resource_path + "Configs\\gui.json", json);
    const std::u8string json_u8(json);
    const auto json_config(utf8_to_utf16(json_u8));
    Logger::instance().info("load()");
    m_json = json_config;
    graph_.accept_reference(*this);
}

void GuiAtlasReader::visit(gui_style_t& ref)
{
    read_object(ref);
}

void GuiAtlasReader::load_tiles(std::unordered_map<std::u16string, atlas_tile_t>& ref)
{
    Logger::instance().info("GuiAtlasReader::load_tiles");
    const auto property = read_json_array(m_json);
    if (property) {
        std::u16string k;
        glGenTextures(1, &graph_.m_gui_atlas);
        glBindTexture(GL_TEXTURE_2D_ARRAY, graph_.m_gui_atlas);
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, graph_.m_font_atlas_size, graph_.m_font_atlas_size, property->size() / 2 + 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        for (std::size_t image_index = 0; image_index < property->size(); image_index += 2) {
            std::u16string name;
            m_json = (*property)[image_index];
            visit(name);
            const auto file_name = FileSystem::instance().m_resource_path + "Tiles/gui/" + utf16_to_cp1251(name) + ".png";
            int width, height, channels;
            const auto image_data = stbi_load(file_name.c_str(), &width, &height, &channels, STBI_rgb_alpha);
            Logger::instance().info("texture name: {} {} {}", file_name, width, height);
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, image_index / 2 + 2, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
            stbi_image_free(image_data);
            const auto p = read_json_array((*property)[image_index + 1]);
            for (std::size_t j = 0; j < p->size(); j += 2) {
                m_json = (*p)[j];
                visit(k);
                auto& v = ref[k];
                m_json = (*p)[j + 1];
                visit(v);
                v.texture.y = height - v.texture.bottom();
                v.layer = image_index / 2 + 2;
            }
            delete p;
        }
        delete property;
    }
}

void GuiAtlasReader::load_tile_ref(atlas_tile_t*& ref)
{
    std::u16string value;
    visit(value);
    ref = &graph_.atlas_tiles[value];
}

void GuiAtlasReader::visit(GraphicalController& ref)
{
    read_object(ref);
}

void GuiAtlasReader::visit(GraphicalController*& ref)
{
    read_object(*ref);
}
