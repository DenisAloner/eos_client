#include "GraphicalController.h"
#include "utils/log.h"


GraphicalController::GraphicalController()
{
	try
	{
		m_actions[action_e::move] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_0.bmp");
		m_actions[action_e::push] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_1.bmp");
		m_actions[action_e::turn] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_2.bmp");
		m_actions[action_e::open_inventory] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_3.bmp");
		m_actions[action_e::cell_info] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_4.bmp");

		m_horizontal_shader = load_shader("EoS_blur", "EoS_blur_horizontal");
		m_vertical_shader = load_shader("EoS_blur", "EoS_blur_vertical");
		m_mask_shader = load_shader("EoS_mask", "EoS_mask");
		m_tile_shader = load_shader("EoS_tile", "EoS_tile");
		m_empty_01 = create_empty_texture(dimension_t(1024, 1024));
		m_empty_02 = create_empty_texture(dimension_t(1024, 1024));
		m_close = load_texture(FileSystem::instance().m_resource_path + "Tiles\\EoS_Close.bmp");
		m_select = load_texture(FileSystem::instance().m_resource_path + "Tiles\\EoS_Select.bmp");
		m_font = load_texture(FileSystem::instance().m_resource_path + "Tiles\\EoS_Font.bmp");
		m_cursor = load_texture(FileSystem::instance().m_resource_path + "Tiles\\EoS_Cursor.bmp");
	}
	catch(std::logic_error e)
	{
		LOG(FATAL) << "������ ��� �������� ��������: " << e.what();
	}

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

void GraphicalController::draw_tile(tile_t& tile,double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3)
{
	glBegin(GL_QUADS);
	glTexCoord2d(tile.coordinat[0], tile.coordinat[1]); glVertex2d(x0, y0);
	glTexCoord2d(tile.coordinat[0], tile.coordinat[3]); glVertex2d(x1, y1);
	glTexCoord2d(tile.coordinat[2], tile.coordinat[3]); glVertex2d(x2, y2);
	glTexCoord2d(tile.coordinat[2], tile.coordinat[1]); glVertex2d(x3, y3);
	glEnd();
}

bool GraphicalController::CompileSuccessful(int obj)
{
	int status;
	glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
	return status == GL_TRUE;
}

bool GraphicalController::LinkSuccessful(int obj)
{
	int status;
	glGetProgramiv(obj, GL_LINK_STATUS, &status);
	return status == GL_TRUE;
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

void GraphicalController::draw_tile_FBO(double tx1, double ty1, double tx2, double ty2, double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3)
{
	glBegin(GL_QUADS);
	glTexCoord2d(tx1, ty2); glVertex2d(x0, y0);
	glTexCoord2d(tx1, ty1); glVertex2d(x1, y1);
	glTexCoord2d(tx2, ty1); glVertex2d(x2, y2);
	glTexCoord2d(tx2, ty2); glVertex2d(x3, y3);
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
	glBindTexture(GL_TEXTURE_2D, m_empty_01);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, x, 1024 - y - height, width, height);
	double Tx = width / 1024.0;
	double Ty = height / 1024.0;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FBO);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_empty_02, 0);
	glUseProgramObjectARB(m_horizontal_shader);
	set_uniform_sampler(m_horizontal_shader, "Map");
	draw_sprite_FBO(Tx, Ty, 0, 1024 - height, 0, 1024, width, 1024, width, 1024 - height);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_empty_01, 0);
	glUseProgramObjectARB(m_vertical_shader);
	set_uniform_sampler(m_vertical_shader, "Map");
	glBindTexture(GL_TEXTURE_2D, m_empty_02);
	draw_sprite_FBO(Tx, Ty, 0, 1024 - height, 0, 1024, width, 1024, width, 1024 - height);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glUseProgramObjectARB(0);
	glEnable(GL_SCISSOR_TEST);
	glBindTexture(GL_TEXTURE_2D, m_empty_01);
	draw_sprite_FBO(Tx, Ty, x, y, x, y + height, x + width, y + height, x + width, y);
	glColor4f(1.0, 1.0, 1.0, 1.0);
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

bool GraphicalController::set_uniform_ptr(GLhandleARB program, const char * name, const int value)
{
	int loc = glGetUniformLocationARB(program, name);
	if (loc < 0)
		return false;
	glUniform1iv(loc, 1, &value);
	return true;
}

bool GraphicalController::set_uniform_float(GLhandleARB program, const char * name, const float value)
{
	int loc = glGetUniformLocationARB(program, name);
	if (loc < 0)
		return false;
	glUniform1f(loc,value);
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

	struct
	{
		GLdouble x, y, z;
	} point;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);
	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &point.x, &point.y, &point.z);
	return position_t(point.x, point.y);
}

//GLuint GraphicalController::load_texture(const char * filename)
//{
//	GLuint texture;
//	char* data;
//	FILE* file;
//	BITMAPFILEHEADER header;
//	BITMAPINFOHEADER info;
//	fopen_s(&file, filename, "rb");
//	fread(&header, sizeof(header), 1, file);
//	fread(&info, sizeof(info), 1, file);
//	data = new char[info.biWidth * info.biHeight * 4];
//	fread(data, info.biWidth * info.biHeight * sizeof(GLuint), 1, file);
//	fclose(file);
//	for (GLuint i = 0; i<info.biWidth * info.biHeight * 4; i += 4)
//	{
//		int r, g, b, a;
//		b = data[i];
//		g = data[i + 1];
//		r = data[i + 2];
//		a = data[i + 3];
//		data[i] = r;
//		data[i + 1] = g;
//		data[i + 2] = b;
//		data[i + 3] = a;
//	}
//	glGenTextures(1, &texture);
//	glBindTexture(GL_TEXTURE_2D, texture);
//	//glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.biWidth, info.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, info.biWidth, info.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
//	delete data;
//	return texture;
//}

GLuint GraphicalController::load_texture(const std::string& path)
{
	GLuint texture;
	FILE* file;
	BITMAPFILEHEADER* header;
	BITMAPINFOHEADER* info;
	char* data;
	bytearray buff;
	if (!FileSystem::instance().load_from_file(path, buff))
	{
		MessageBox(NULL, path.c_str(), "", MB_OK);
		throw std::logic_error("�� ������� ��������� ���� `" + path + "`");
	}
	header = reinterpret_cast<BITMAPFILEHEADER*>(buff.get());
	info = reinterpret_cast<BITMAPINFOHEADER*>(buff.get() + sizeof(*header));
	data = reinterpret_cast<char*>(buff.get() + sizeof(*header)+sizeof(*info));
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, info->biWidth, info->biHeight, GL_BGRA, GL_UNSIGNED_BYTE, data);
	return texture;
}

GLcharARB* GraphicalController::load_shader_source(const std::string& path)
{
	GLcharARB* ShaderSource;
	std::ifstream ShaderFile(path, std::ifstream::binary);
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

GLhandleARB GraphicalController::load_shader(const std::string& vPath, const std::string& fPath)
{
	const GLcharARB* VertexShaderSource = load_shader_source(FileSystem::instance().m_resource_path + "Shaders\\" + vPath + ".vsh");
	const GLcharARB* FragmentShaderSource = load_shader_source(FileSystem::instance().m_resource_path + "Shaders\\" + fPath + ".fsh");
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
	if (!CompileSuccessful(VertexShader))
		LOG(FATAL) << "�� ������� �������������� ��������� ������ `" << vPath << "`";
	glShaderSourceARB(FragmentShader, 1, &FragmentShaderSource, NULL);
	glCompileShaderARB(FragmentShader);
	glGetObjectParameterivARB(FragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &compileStatus);
	if (!CompileSuccessful(FragmentShader))
		LOG(FATAL) << "�� ������� �������������� ����������� ������ `" << fPath << "`";
	glAttachObjectARB(Program, VertexShader);
	glAttachObjectARB(Program, FragmentShader);
	glLinkProgramARB(Program);
	if (!LinkSuccessful(Program))
		LOG(FATAL) << "�� ������� ���������� ��������� ���������!";
	return Program;
}

GLint GraphicalController::create_empty_texture(dimension_t size)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.w, size.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	return texture;
}