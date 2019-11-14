#ifndef GRAPHICALCONTROLLER_H
#define GRAPHICALCONTROLLER_H

#include "Definiton.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#include "gui_style.h"
#include "reader.h"
#include "vao.h"
#include <list>
#include <string>
#include <vector>

enum class tile_options_e : unsigned char {
    NONE = 0,
    FLIP_X = 1,
    FLIP_Y = 2
};

tile_options_e operator&(const tile_options_e& lhs, const tile_options_e& rhs);

tile_options_e operator|(const tile_options_e& lhs, const tile_options_e& rhs);

class TileManager;
const int font_size_c = 26;

class GraphicalController {
public:
    GLuint m_actions[18];

    GLuint m_horizontal_shader;
    GLuint m_vertical_shader;
    GLuint m_tile_shader;
    GLuint m_tile_shader_hide;
    GLuint m_mask_shader;
    GLuint m_mask_shader2;
    GLuint m_atlas_shader;
    GLuint m_ui_shader;

    GLuint m_empty_01;
    GLuint m_empty_02;
    GLuint m_empty_03;
    GLuint m_dir;
    GLuint m_cursor;
    GLuint m_close;
    GLuint m_preselect;
    GLuint m_select;
    GLuint m_logo;
    GLuint m_no_image;
    GLuint m_visible;

    GLuint m_atlas;

    GLuint m_gui_vao;
    GLuint m_gui_vertex_buffer;
    GLuint m_gui_atlas;

    GLsizei m_font_atlas_size = 1024;
    GLint m_font_atlas_x_offset;
    GLint m_font_atlas_y_offset;
    GLint m_font_atlas_row_symbol_max_height;

    dimension_t<int> m_size;

    std::list<rectangle_t<int>> m_scissors;
    std::unordered_map<char16_t, atlas_symbol_t> m_unicode_symbols;

    std::vector<vao_quad_t<gui_vertex_t>> m_gui_quads;
    int quads_count_to_render;
    std::unordered_map<std::u16string, atlas_tile_t> atlas_tiles;
    std::unordered_map<std::u16string, gui_style_t> gui_styles;

    GLuint m_FBO;

    std::thread::id m_id;

    explicit GraphicalController(dimension_t<int> size);

    GLuint load_shader(const std::string& vPath, const std::string& fPath);
    std::string load_shader_source(const std::string& path);
    bool set_uniform_vector(GLuint program, const char* name, const float* value);
    bool set_uniform_sampler(GLuint object, const char* name, int index);
    bool set_uniform_ptr(GLuint program, const char* name, const int value);
    position_t<int> get_open_gl_position(float x, float y);
    GLuint load_texture(const std::string& path);
    void output_text(int x, int y, const std::u16string& text, int sizex, int sizey);
    void center_text(int x, int y, const std::u16string& text, int sizex, int sizey);
    bool add_scissor(int x, int y, int w, int h);
    void remove_scissor();
    void render_gui();

    void set_VSync(bool sync);
    bool set_uniform_float(GLuint program, const char* name, const float value);
    GLint create_empty_texture(dimension_t<int> size);
    void load_font(const std::string& font_filename);

    void generate_symbol(atlas_symbol_t* symbol, GLint x_offset, GLint y_offset, unsigned char* buffer);

    atlas_symbol_t& get_symbol(char16_t value);

    GLuint png_texture_load(const std::string& path);
    GLuint texture_array_load(const std::vector<std::string>& path);

    std::size_t measure_text_width(const std::u16string& text);

    position_t<int> center_align_to_point(int x, int y, std::u16string text);

    void draw_sprite(const rectangle_t<int>& rect);
    void draw_rectangle(const rectangle_t<int>& rect);
    void stroke_cell(int x, int y, int xs, int ys);
    void stroke_cube(int x, int y, int z, int xs, int ys);
    void selection_cell(int x, int y, int z, int xs, int ys);
    void draw_tile(tile_t& tile, const rectangle_t<int>& rect);
    void draw_sprite_fbo(double tex_width, double tex_height, const rectangle_t<int>& rect);
    void draw_tile_fbo(double tx1, double ty1, double tx2, double ty2, const rectangle_t<int>& rect);
    void check_gl_error(const std::string& text);

    vao_quad_t<gui_vertex_t>& get_gui_quad(int x, int y, int w, int h);
    vao_quad_t<gui_vertex_t>& get_gui_quad(int x, int y, int w, int h, const atlas_tile_t& tile, tile_options_e options = tile_options_e::NONE);

    void render_background(int x, int y, int w, int h, const gui_style_t& style);
    void render_border(int x, int y, int w, int h, const gui_style_t& style);

    FT_Pos max_symbol_height_for_current_font() const;
    FT_Pos ascender_for_current_font() const;

private:
    FT_Library m_library;
    FT_Error m_error;
    FT_Face m_face;
    FT_GlyphSlot m_slot;

    bool compile_successful(int obj);
    bool link_successful(int obj);
    bool validate_successful(int obj);
};

class GuiAtlasReader : public JsonReader {
public:
    using JsonReader::read;
    explicit GuiAtlasReader(GraphicalController& graph);
    void load();
    void read(const std::u16string_view& json, std::unordered_map<std::u16string, atlas_tile_t>& ref);
    void read(const std::u16string_view& json, gui_style_t& ref) override;

private:
    GraphicalController& graph_;
};

inline GuiAtlasReader::GuiAtlasReader(GraphicalController& graph)
    : graph_(graph)
{
}

#endif //GRAPHICALCONTROLLER_H
