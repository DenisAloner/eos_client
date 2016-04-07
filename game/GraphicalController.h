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

#define WIDTH   640
#define HEIGHT  480

class TileManager;

class GraphicalController
{
public:

	GLuint m_actions[16];

	GLuint m_horizontal_shader;
	GLuint m_vertical_shader;
	GLuint m_tile_shader;
	GLuint m_tile_shader_hide;
	GLuint m_mask_shader;
	GLuint m_mask_shader2;

	GLuint m_empty_01;
	GLuint m_empty_02;
	GLuint m_empty_03;
	GLuint m_dir;
	GLuint m_cursor;
	GLuint m_close;
	GLuint m_preselect;
	GLuint m_select;
	GLuint m_font;
	GLuint m_logo;
	GLuint m_no_image;
	GLuint m_visible;
	GLuint m_blur;

	enum class command_e
	{
		single,
		single_animate,
		rotating,
		rotating8,
		icon

	};

	std::list<frectangle_t> m_scissors;
	std::vector<TileManager*> m_tile_managers;
	std::vector<GLuint> m_icons;
	std::map<std::string, command_e> m_commands;

	GLuint m_FBO;

	GraphicalController();

	GLuint load_shader(const std::string& vPath,const std::string& fPath);
	std::string load_shader_source(const std::string& path);
	bool set_uniform_vector(GLuint program, const char * name, const float * value);
	bool set_uniform_sampler(GLuint object, const char * name, int index);
	bool set_uniform_ptr(GLuint program, const char * name, const int value);
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
	void set_VSync(bool sync);
	bool set_uniform_float(GLuint program, const char * name, const float value);
	void draw_tile(tile_t& tile, double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3);
	void draw_tile_FBO(double tx1, double ty1, double tx2, double ty2, double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3);
	GLint create_empty_texture(dimension_t size);
	void load_configuration();
	void parser(const std::string& command);

private:
	bool CompileSuccessful(int obj);
	bool LinkSuccessful(int obj);
	bool ValidateSuccessful(int obj);


	
};

#endif //GRAPHICALCONTROLLER_H
