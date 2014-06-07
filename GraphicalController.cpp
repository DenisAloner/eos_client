#include "GraphicalController.h"

GraphicalController::GraphicalController()
{
	m_sprites[0] = load_texture("C:\\ExplorersOfSaarum\\plitka_na_pol.bmp");
	m_sprites[1] = load_texture("C:\\ExplorersOfSaarum\\Stena.bmp");
	m_sprites[2] = load_texture("C:\\ExplorersOfSaarum\\sprite_8.bmp");
	m_sprites[3] = load_texture("C:\\ExplorersOfSaarum\\ElfSprite-1.bmp");
	m_sprites[4] = load_texture("C:\\ExplorersOfSaarum\\ElfSprite-2.bmp");
	m_sprites[5] = load_texture("C:\\ExplorersOfSaarum\\ElfSprite-3.bmp");
	m_sprites[6] = load_texture("C:\\ExplorersOfSaarum\\ElfSprite-4.bmp");
	m_sprites[7] = load_texture("C:\\ExplorersOfSaarum\\orc.bmp");
	m_sprites[8] = load_texture("C:\\ExplorersOfSaarum\\select.bmp");
	m_sprites[9] = load_texture("C:\\ExplorersOfSaarum\\Sunduk.bmp");
	m_sprites[10] = load_texture("C:\\ExplorersOfSaarum\\ElfSprite-5.bmp");
	m_sprites[11] = load_texture("C:\\ExplorersOfSaarum\\ElfSprite-6.bmp");
	m_sprites[12] = load_texture("C:\\ExplorersOfSaarum\\ElfSprite-7.bmp");
	m_sprites[13] = load_texture("C:\\ExplorersOfSaarum\\ElfSprite-8.bmp");
	m_sprites[14] = load_texture("C:\\ExplorersOfSaarum\\Elf2.bmp");
	m_sprites[15] = load_texture("C:\\ExplorersOfSaarum\\Stena2.bmp");
	m_sprites[16] = load_texture("C:\\ExplorersOfSaarum\\plitka_na_pol2.bmp");
	m_sprites[18] = load_texture("C:\\ExplorersOfSaarum\\IB2.bmp");
	m_sprites[19] = load_texture("C:\\ExplorersOfSaarum\\Screen.bmp");
	m_sprites[20] = load_texture("C:\\ExplorersOfSaarum\\Screen.bmp");
	m_sprites[21] = load_texture("C:\\ExplorersOfSaarum\\EoS_Cursor.bmp");

	m_sprites[22] = load_texture("C:\\ExplorersOfSaarum\\Tiles\\EoS_Wall_01.bmp");
	m_sprites[23] = load_texture("C:\\ExplorersOfSaarum\\Tiles\\EoS_Wall_02.bmp");
	m_sprites[24] = load_texture("C:\\ExplorersOfSaarum\\Tiles\\EoS_Wall_03.bmp");
	m_sprites[25] = load_texture("C:\\ExplorersOfSaarum\\Tiles\\EoS_Wall_04.bmp");
	m_sprites[26] = load_texture("C:\\ExplorersOfSaarum\\Tiles\\EoS_Wall_05.bmp");
	m_sprites[27] = load_texture("C:\\ExplorersOfSaarum\\Tiles\\EoS_Floor_01.bmp");
	m_sprites[28] = load_texture("C:\\ExplorersOfSaarum\\Tiles\\EoS_Floor_02.bmp");
	m_sprites[29] = load_texture("C:\\ExplorersOfSaarum\\Tiles\\EoS_Floor_03.bmp");
	m_sprites[30] = load_texture("C:\\ExplorersOfSaarum\\Tiles\\EoS_Floor_04.bmp");

	m_actions[ActionKind_Move] = load_texture("C:\\ExplorersOfSaarum\\Action_0.bmp");
	m_actions[ActionKind_Push] = load_texture("C:\\ExplorersOfSaarum\\Action_1.bmp");
	m_actions[ActionKind_Turn] = load_texture("C:\\ExplorersOfSaarum\\Action_2.bmp");
	m_actions[ActionKind_OpenInventory] = load_texture("C:\\ExplorersOfSaarum\\Bag.bmp");

	m_render_shader = load_shader("C:\\ExplorersOfSaarum\\EoS_Render.vsh", "C:\\ExplorersOfSaarum\\EoS_Render.fsh");
	m_light_shader = load_shader("C:\\ExplorersOfSaarum\\EoS_Light.vsh", "C:\\ExplorersOfSaarum\\EoS_Light.fsh");
	m_horizontal_shader = load_shader("C:\\ExplorersOfSaarum\\EoS_Render.vsh", "C:\\ExplorersOfSaarum\\EoS_HorizontalBlur.fsh");
	m_vertical_shader = load_shader("C:\\ExplorersOfSaarum\\EoS_Render.vsh", "C:\\ExplorersOfSaarum\\EoS_VerticalBlur.fsh");
	m_font = load_texture("C:\\ExplorersOfSaarum\\FontRender4.bmp");

	m_scissors.push_front(frectangle_t(0.0f, 0.0f, 1024.0f, 1024.0f));
	glGenFramebuffersEXT(1, &m_FBO);
}

void GraphicalController::draw_sprite(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3)
	{
		glBegin(GL_QUADS);
		glTexCoord2d(0,1); glVertex2d(x0, y0);
		glTexCoord2d(0,0); glVertex2d(x1, y1);
		glTexCoord2d(1,0); glVertex2d(x2, y2);
		glTexCoord2d(1,1); glVertex2d(x3, y3);
		glEnd();
	}

void GraphicalController::draw_sprite_FBO(double TexWidth, double TexHeight, double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3)
{
	glBegin(GL_QUADS);
	glTexCoord2d(0, TexHeight); glVertex2d(x0, y0);
	glTexCoord2d(0, 0); glVertex2d(x1, y1);
	glTexCoord2d(TexWidth, 0); glVertex2d(x2, y2);
	glTexCoord2d(TexWidth, TexHeight); glVertex2d(x3, y3);
	glEnd();
}

void GraphicalController::center_text(int x,int y,std::string Text,int sizex,int sizey)
{
	int cx=x-(Text.length())*(sizex+1)/2;
	int cy=y-sizey/2;
	output_text(cx,cy,Text,sizex,sizey);
}

void GraphicalController::set_VSync(bool sync)
{
	// Function pointer for the wgl extention function we need to enable/disable
	// vsync
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;

	const char *extensions = (char*)glGetString(GL_EXTENSIONS);

	if (strstr(extensions, "WGL_EXT_swap_control") == 0)
	{
		return;
	}
	else
	{
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");

		if (wglSwapIntervalEXT)
			wglSwapIntervalEXT(sync);
	}
}

void GraphicalController::output_text(int x, int y, std::string& Text, int sizex, int sizey)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgramObjectARB(0);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, m_font);
	int x0, y0, x1, y1, x2, y2, x3, y3;
	double xt0, yt0, xt1, yt1, xt2, yt2, xt3, yt3;
	unsigned char  i, j;
	unsigned char ASCII;
	for (int k = 0; k<Text.length(); k++)
	{
		ASCII = (unsigned char)Text[k];
		j = ASCII / 16;
		i = ASCII - j * 16;
		j = 15 - j;
		x0 = x + k*(sizex + 1);
		y0 = y;
		x1 = x + k*(sizex + 1);
		y1 = y + sizey;
		x2 = x + sizex + k*(sizex + 1);
		y2 = y + sizey;
		x3 = x + sizex + k*(sizex + 1);
		y3 = y;
		xt0 = i*0.0625;
		yt0 = (j + 1)*0.0625;
		xt1 = i*0.0625;
		yt1 = j*0.0625;
		xt2 = (i + 1)*0.0625;
		yt2 = j*0.0625;
		xt3 = (i + 1)*0.0625;
		yt3 = (j + 1)*0.0625;
		glBegin(GL_QUADS);
		glTexCoord2d(xt0, yt0); glVertex2d(x0, y0);
		glTexCoord2d(xt1, yt1); glVertex2d(x1, y1);
		glTexCoord2d(xt2, yt2); glVertex2d(x2, y2);
		glTexCoord2d(xt3, yt3); glVertex2d(x3, y3);
		glEnd();
	}
}

bool GraphicalController::add_scissor(const frectangle_t& rect)
{
	frectangle_t GlobalScissor = m_scissors.front();
	frectangle_t LocalScissor(rect.x, 1024 - rect.y - rect.h, rect.x + rect.w, 1024 - rect.y);
	if ((GlobalScissor.x > LocalScissor.w) || (GlobalScissor.w < LocalScissor.x) || (GlobalScissor.y > LocalScissor.h) || (GlobalScissor.h < LocalScissor.y)) return false;
	if (LocalScissor.x < GlobalScissor.x) LocalScissor.x = GlobalScissor.x;
	if (LocalScissor.w > GlobalScissor.w) LocalScissor.w = GlobalScissor.w;
	if (LocalScissor.y < GlobalScissor.y) LocalScissor.y = GlobalScissor.y;
	if (LocalScissor.h > GlobalScissor.h) LocalScissor.h = GlobalScissor.h;
	m_scissors.push_front(LocalScissor);
	glScissor(LocalScissor.x, LocalScissor.y, LocalScissor.w - LocalScissor.x, LocalScissor.h - LocalScissor.y);
	return true;
}

void GraphicalController::remove_scissor()
{
	m_scissors.pop_front();
	frectangle_t Scissor = m_scissors.front();
	glScissor(Scissor.x, Scissor.y, Scissor.w - Scissor.x, Scissor.h - Scissor.y);
}

void GraphicalController::blur_rect(int x, int y, int width, int height)
{
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_sprites[19]);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, x, 1024 - y - height, width, height);
	double Tx = width / 1024.0;
	double Ty = height / 1024.0;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FBO);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_sprites[20], 0);
	glUseProgramObjectARB(m_horizontal_shader);
	set_uniform_sampler(m_horizontal_shader, "Map");
	draw_sprite_FBO(Tx, Ty, 0, 1024 - height, 0, 1024, width, 1024, width, 1024 - height);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_sprites[19], 0);
	glUseProgramObjectARB(m_vertical_shader);
	set_uniform_sampler(m_vertical_shader, "Map");
	glBindTexture(GL_TEXTURE_2D, m_sprites[20]);
	draw_sprite_FBO(Tx, Ty, 0, 1024 - height, 0, 1024, width, 1024, width, 1024 - height);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glUseProgramObjectARB(0);
	glEnable(GL_SCISSOR_TEST);
	glBindTexture(GL_TEXTURE_2D, m_sprites[19]);
	draw_sprite_FBO(Tx, Ty, x, y, x, y + height, x + width, y + height, x + width, y);
}

GLcharARB* GraphicalController::load_shader_source(char* filename)
{
	GLcharARB* ShaderSource;
	std::ifstream ShaderFile(filename, std::ifstream::binary);
	if (ShaderFile)
	{
		ShaderFile.seekg(0, ShaderFile.end);
		int length = ShaderFile.tellg();
		ShaderFile.seekg(0, ShaderFile.beg);
		ShaderSource = new GLcharARB[length + 1];
		ShaderFile.read(ShaderSource, length);
		ShaderSource[length] = '\0';
		ShaderFile.close();
	}
	else {
		return nullptr;
	}
	return ShaderSource;
}

GLhandleARB GraphicalController::load_shader(char* vPath, char* fPath)
{
	const GLcharARB* VertexShaderSource = load_shader_source(vPath);
	const GLcharARB* FragmentShaderSource = load_shader_source(fPath);
	GLhandleARB Program;
	GLhandleARB VertexShader;
	GLhandleARB FragmentShader;
	Program = glCreateProgramObjectARB();
	VertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	FragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	glShaderSourceARB(VertexShader, 1, &VertexShaderSource, NULL);
	glCompileShaderARB(VertexShader);
	GLint compileStatus;
	glGetObjectParameterivARB(VertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &compileStatus);
	show_error(VertexShader);
	glShaderSourceARB(FragmentShader, 1, &FragmentShaderSource, NULL);
	glCompileShaderARB(FragmentShader);
	glGetObjectParameterivARB(FragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &compileStatus);
	show_error(FragmentShader);
	glAttachObjectARB(Program, VertexShader);
	glAttachObjectARB(Program, FragmentShader);
	glLinkProgramARB(Program);
	return Program;
}

bool GraphicalController::set_uniform_vector(GLhandleARB program, const char * name, const float * value)
{
	int loc = glGetUniformLocationARB(program, name);
	if (loc < 0)
		return false;
	glUniform4fvARB(loc, 1, value);
	return true;
}

bool GraphicalController::set_uniform_sampler(GLhandleARB object, const char * name)
{
	int loc = glGetUniformLocationARB(object, name);
	if (loc < 0)
		return false;
	glUniform1iARB(loc, 0);
	return true;
}

bool GraphicalController::set_uniform_ptr(GLhandleARB program, const char * name, const unsigned int value)
{
	int loc = glGetUniformLocationARB(program, name);
	if (loc < 0)
		return false;
	glUniform1uiv(loc, 1, &value);
	return true;
}

void GraphicalController::unload_shader_source(GLcharARB* ShaderSource)
{
	if (ShaderSource != nullptr)
	{
		delete[] ShaderSource;
		ShaderSource = nullptr;
	}
}

bool GraphicalController::check_OpenGL_error()
{
	bool    retCode = true;

	for (;;)
	{
		GLenum  glErr = glGetError();

		if (glErr == GL_NO_ERROR)
			return retCode;

		printf("glError: %s\n", gluErrorString(glErr));
		MessageBox(NULL, "glError: %s\n", "1", MB_OK);
		retCode = false;
		glErr = glGetError();
	}

	return retCode;
}

void GraphicalController::show_error(GLhandleARB object)
{
	GLint LogLen;
	GLcharARB * infoLog;
	int charsWritten = 0;
	check_OpenGL_error();
	glGetObjectParameterivARB(object, GL_OBJECT_INFO_LOG_LENGTH_ARB, &LogLen);
	infoLog = new GLcharARB[LogLen];
	glGetInfoLogARB(object, LogLen, &charsWritten, infoLog);
	if (infoLog[0] != '\0'){ MessageBox(NULL, infoLog, "Error", MB_OK); }
}

position_t GraphicalController::get_OpenGL_position(float x, float y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	TPoint _Point;
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);
	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &_Point.x, &_Point.y, &_Point.z);
	return position_t(_Point.x, _Point.y);
}

GLuint GraphicalController::load_texture(const char * filename)
{
	GLuint texture;
	char* data;
	FILE* file;
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER info;
	fopen_s(&file, filename, "rb");
	fread(&header, sizeof(header), 1, file);
	fread(&info, sizeof(info), 1, file);
	data = new char[info.biWidth * info.biHeight * 4];
	fread(data, info.biWidth * info.biHeight * sizeof(GLuint), 1, file);
	fclose(file);
	for (GLuint i = 0; i<info.biWidth * info.biHeight * 4; i += 4)
	{
		int r, g, b, a;
		b = data[i];
		g = data[i + 1];
		r = data[i + 2];
		a = data[i + 3];
		data[i] = r;
		data[i + 1] = g;
		data[i + 2] = b;
		data[i + 3] = a;
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.biWidth, info.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, info.biWidth, info.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
	delete data;
	return texture;
}