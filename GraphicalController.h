#pragma once
#include "glew.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "Definiton.h"
#include <string>
#include <list>
#include <vector>
#include "FileSystem.h"

#define WIDTH   640
#define HEIGHT  480

struct TPoint
{
	GLdouble x,y,z;
};


class GraphicalController
{
public:

	GLuint m_actions[4];

	GLhandleARB m_render_shader;
	GLhandleARB m_light_shader;
	GLhandleARB m_horizontal_shader;
	GLhandleARB m_vertical_shader;
	GLhandleARB m_tile_shader;

	GLuint m_empty_01;
	GLuint m_empty_02;
	GLuint m_cursor;
	GLuint m_close;
	GLuint m_select;
	GLuint m_font;

	std::list<frectangle_t> m_scissors;

	GLuint m_FBO;

	GraphicalController();

	void unload_shader_source(GLcharARB* ShaderSource);
	bool check_OpenGL_error();
	void show_error(GLhandleARB object);
	bool set_uniform_vector(GLhandleARB program, const char * name, const float * value);
	bool set_uniform_sampler(GLhandleARB object, const char * name);
	bool set_uniform_ptr(GLhandleARB program, const char * name, const unsigned int value);
	position_t get_OpenGL_position(float x, float y);
	GLuint load_texture(const std::string& path);
	void output_text(int x, int y, std::string& Text, int sizex, int sizey);
	void draw_sprite(double x0,double y0,double x1,double y1,double x2,double y2,double x3,double y3);
	void center_text(int x, int y, std::string Text, int sizex, int sizey);
	void render_text(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3);
	bool add_scissor(const frectangle_t& rect);
	void remove_scissor();
	void draw_sprite_FBO(double TexWidth, double TexHeight, double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3);
	void blur_rect(int x, int y, int width, int height);
	GLcharARB* load_shader_source(const std::string& path);
	GLhandleARB load_shader(const std::string& vPath,const std::string& fPath);
	void set_VSync(bool sync);
	bool set_uniform_float(GLhandleARB program, const char * name, const float value);
	void draw_tile(tile_t& tile, double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3);

};

