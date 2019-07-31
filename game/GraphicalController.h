#ifndef GRAPHICALCONTROLLER_H
#define	GRAPHICALCONTROLLER_H

#include <iostream>
#include <fstream>
#include <Windows.h>
#include "Definiton.h"
#include <string>
#include <list>
#include <vector>
#include "FileSystem.h"
#include <map>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <png.h>

class TileManager;

const int font_size_c = 26;

class GraphicalController
{
public:

	struct rectangle_t
	{
		position_t a;
		position_t b;
		rectangle_t() : a(0, 0), b(0, 0) {};
		rectangle_t(int x, int y, int w, int h) : a(x, y), b(x + w, y + h) {};
		void set(int x, int y, int w, int h)
		{
			a.x = x;
			a.y = y;
			b.x = x + w;
			b.y = y + h;
		};
	};

	GLuint m_actions[17];

	GLuint m_horizontal_shader;
	GLuint m_vertical_shader;
	GLuint m_tile_shader;
	GLuint m_tile_shader_hide;
	GLuint m_mask_shader;
	GLuint m_mask_shader2;
	GLuint m_atlas_shader;

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
	GLuint m_blur;

	GLuint m_atlas;

	FT_Library    m_library;
	FT_Error      m_error;
	FT_Face       m_face;
	FT_GlyphSlot  m_slot;


	dimension_t m_size;

	
	std::list<frectangle_t> m_scissors;
	std::unordered_map<char16_t, font_symbol_t> m_unicode_symbols;

	GLuint m_FBO;

	GraphicalController(dimension_t size);

	GLuint load_shader(const std::string& vPath,const std::string& fPath);
	std::string load_shader_source(const std::string& path);
	bool set_uniform_vector(GLuint program, const char * name, const float * value);
	bool set_uniform_sampler(GLuint object, const char * name, int index);
	bool set_uniform_ptr(GLuint program, const char * name, const int value);
	position_t get_OpenGL_position(float x, float y);
	GLuint load_texture(const std::string& path);
	void output_text(int x, int y, std::u16string& Text, int sizex, int sizey);
	void center_text(int x, int y, std::u16string Text, int sizex, int sizey);
	void render_text(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3);
	bool add_scissor(const frectangle_t& rect);
	void remove_scissor();
	void blur_rect(int x, int y, int width, int height);
	void set_VSync(bool sync);
	bool set_uniform_float(GLuint program, const char * name, const float value);
	GLint create_empty_texture(dimension_t size);
	void load_font(const std::string& font_filename);
	font_symbol_t& get_symbol(char16_t value);

	GLuint png_texture_load(const std::string& path);
	GLuint texture_array_load(const std::vector<std::string>& path);

	std::size_t measure_text_width(std::u16string& Text);

	position_t center_aling_to_point(int x, int y, std::u16string text);

	void draw_sprite(const rectangle_t &rect);
	void draw_rectangle(rectangle_t rect);
	void stroke_cell(int x, int y, int xs, int ys);
	void draw_tile(tile_t& tile, rectangle_t rect);
	void draw_sprite_FBO(double TexWidth, double TexHeight, rectangle_t rect);
	void draw_tile_FBO(double tx1, double ty1, double tx2, double ty2, rectangle_t rect);
	void check_gl_error(const std::string& text);
	

private:
	bool CompileSuccessful(int obj);
	bool LinkSuccessful(int obj);
	bool ValidateSuccessful(int obj);
};

#endif //GRAPHICALCONTROLLER_H
