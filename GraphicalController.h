#pragma once
#include "glew.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "Definiton.h"
#include <string>
#include <list>
#include <ft2build.h>
#include FT_FREETYPE_H

#define WIDTH   640
#define HEIGHT  480

struct TPoint
{
	GLdouble x,y,z;
};


class GraphicalController
{
public:
	/* origin is the upper left corner */

	FT_Library library;
	FT_Face face;
	FT_GlyphSlot slot;
	FT_Matrix     matrix;                 /* transformation matrix */
	FT_Vector     pen;                    /* untransformed origin  */

	GLuint Sprites[22];
	GLuint Actions[4];

	GLhandleARB RenderShader;
	GLhandleARB LightShader;
	GLhandleARB HorizontalShader;
	GLhandleARB VerticalShader;
	GLuint ButtonTexture;
	GLuint PaperTexture;

	GLuint Font;

	std::list<GLint*> Scissors;

	GLuint FBO;

	GraphicalController();

	void UnloadShaderSource(GLcharARB* ShaderSource);
	bool checkOpenGLError();
	void ShowError(GLhandleARB object);
	GLhandleARB LoadShader(char* vPath, char* fPath);
	bool setUniformVector(GLhandleARB program, const char * name, const float * value);
	bool setUniformSampler(GLhandleARB object, const char * name);
	bool setUniformPtr(GLhandleARB program, const char * name, const unsigned int value);
	Point GetOGLPos(float x, float y);
	GLuint LoadTexture(const char * filename);
	void OutputText(int x, int y, std::string& Text, int sizex, int sizey);
	void DrawSprite(double x0,double y0,double x1,double y1,double x2,double y2,double x3,double y3);
	void CenterText(int x, int y, std::string Text, int sizex, int sizey);
	void RenderText(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3);
	GLuint BindTexture(FT_Bitmap* data);
	bool AddScissor(GLint* rect);
	void RemoveScissor();
	void DrawSpriteFBO(double TexWidth, double TexHeight, double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3);
	void BlurRect(int x, int y, int width, int height);
	GLcharARB* LoadShaderSource(char* filename);
	void setVSync(bool sync);

};

