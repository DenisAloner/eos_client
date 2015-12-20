#include "GraphicalController.h"
#include "utils/log.h"
#include "TileManager.h"


GraphicalController::GraphicalController()
{
	try
	{
		m_actions[0] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_0.bmp");
		m_actions[1] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_1.bmp");
		m_actions[2] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_2.bmp");
		m_actions[3] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_3.bmp");
		m_actions[4] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_4.bmp");
		m_actions[5] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_5.bmp");
		m_actions[6] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_6.bmp");
		m_actions[7] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_7.bmp");
		m_actions[8] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_8.bmp");
		m_actions[9] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_9.bmp");
		m_actions[10] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_10.bmp");
		m_actions[11] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_11.bmp");
		m_actions[12] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_12.bmp");
		m_actions[13] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\ico_action_autoexplore.bmp");
		m_actions[14] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_13.bmp");

		m_horizontal_shader = load_shader("EoS_blur", "EoS_blur_horizontal");
		m_vertical_shader = load_shader("EoS_blur", "EoS_blur_vertical");
		m_mask_shader = load_shader("EoS_mask", "EoS_mask");
		m_tile_shader = load_shader("EoS_tile", "EoS_tile");
		m_tile_shader_hide = load_shader("EoS_tile", "EoS_tile_hide");
		m_empty_01 = create_empty_texture(dimension_t(1024, 1024));
		m_empty_02 = create_empty_texture(dimension_t(1024, 1024));
		m_blur = create_empty_texture(dimension_t(1024/4, 1024/4));

		m_close = load_texture(FileSystem::instance().m_resource_path + "Tiles\\EoS_Close.bmp");
		m_preselect = load_texture(FileSystem::instance().m_resource_path + "Tiles\\preselection.bmp");
		m_select = load_texture(FileSystem::instance().m_resource_path + "Tiles\\iso_select.bmp");
		m_font = load_texture(FileSystem::instance().m_resource_path + "Tiles\\EoS_Font.bmp");
		m_cursor = load_texture(FileSystem::instance().m_resource_path + "Tiles\\EoS_Cursor.bmp");
		m_no_image = load_texture(FileSystem::instance().m_resource_path + "Tiles\\no_tile.bmp");
		m_no_visible = load_texture(FileSystem::instance().m_resource_path + "Tiles\\no_visible.bmp");
		m_novisible = load_texture(FileSystem::instance().m_resource_path + "Tiles\\novisible.bmp");
		load_configuration();
	}
	catch(std::logic_error e)
	{
		LOG(FATAL) << "Ошибка при загрузке ресурсов: " << e.what();
	}

	m_scissors.push_front(frectangle_t(0.0f, 0.0f, 1024.0f, 1024.0f));
	glGenFramebuffers(1, &m_FBO);
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

bool GraphicalController::ValidateSuccessful(int obj)
{
	int status;
	glGetProgramiv(obj, GL_VALIDATE_STATUS, &status);
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
	glMultiTexCoord2f(GL_TEXTURE0, tx1, ty2);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
	glVertex2d(x0, y0);
	glMultiTexCoord2f(GL_TEXTURE0, tx1, ty1);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
	glVertex2d(x1, y1);
	glMultiTexCoord2f(GL_TEXTURE0, tx2, ty1);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
	glVertex2d(x2, y2);
	glMultiTexCoord2f(GL_TEXTURE0, tx2, ty2);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
	glVertex2d(x3, y3);
	//glTexCoord2d(tx1, ty2); glVertex2d(x0, y0);
	//glTexCoord2d(tx1, ty1); glVertex2d(x1, y1);
	//glTexCoord2d(tx2, ty1); glVertex2d(x2, y2);
	//glTexCoord2d(tx2, ty2); glVertex2d(x3, y3);
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
	glUseProgram(0);
	glActiveTexture(GL_TEXTURE0);
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
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_empty_02, 0);
	glUseProgram(m_horizontal_shader);
	set_uniform_sampler(m_horizontal_shader, "Map",0);
	draw_sprite_FBO(Tx, Ty, 0, 1024 - height, 0, 1024, width, 1024, width, 1024 - height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_empty_01, 0);
	glUseProgram(m_vertical_shader);
	set_uniform_sampler(m_vertical_shader, "Map",0);
	glBindTexture(GL_TEXTURE_2D, m_empty_02);
	draw_sprite_FBO(Tx, Ty, 0, 1024 - height, 0, 1024, width, 1024, width, 1024 - height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glEnable(GL_SCISSOR_TEST);
	glBindTexture(GL_TEXTURE_2D, m_empty_01);
	draw_sprite_FBO(Tx, Ty, x, y, x, y + height, x + width, y + height, x + width, y);
	glColor4f(1.0, 1.0, 1.0, 1.0);
}

bool GraphicalController::set_uniform_vector(GLuint program, const char * name, const float * value)
{
	GLint loc = glGetUniformLocation(program, name);
	if (loc < 0)
		return false;
	glUniform4fv(loc, 1, value);
	return true;
}

bool GraphicalController::set_uniform_sampler(GLuint object, const char * name, int index)
{
	GLint loc = glGetUniformLocation(object, name);
	if (loc < 0)
		return false;
	glUniform1i(loc, index);
	return true;
}

bool GraphicalController::set_uniform_ptr(GLuint program, const char * name, const int value)
{
	GLint loc = glGetUniformLocation(program, name);
	if (loc < 0)
		return false;
	glUniform1iv(loc, 1, &value);
	return true;
}

bool GraphicalController::set_uniform_float(GLuint program, const char * name, const float value)
{
	GLint loc = glGetUniformLocation(program, name);
	if (loc < 0)
		return false;
	glUniform1f(loc,value);
	return true;
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
	// ВНИМАНИЕ! Нельзя вызывать функции gl вне графического потока!
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);
	winX = x;
	winY = (float)viewport[3] - y;
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
	BITMAPFILEHEADER* header;
	BITMAPINFOHEADER* info;
	char* data;
	bytearray buff;
	if (!FileSystem::instance().load_from_file(path, buff))
	{
		MessageBox(NULL, path.c_str(), "", MB_OK);
		throw std::logic_error("Не удалось загрузить файл `" + path + "`");
	}
	header = reinterpret_cast<BITMAPFILEHEADER*>(buff.get());
	info = reinterpret_cast<BITMAPINFOHEADER*>(buff.get() + sizeof(*header));
	data = reinterpret_cast<char*>(buff.get() + sizeof(*header)+sizeof(*info));
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, info->biWidth, info->biHeight, GL_BGRA, GL_UNSIGNED_BYTE, data);
	return texture;
}

std::string GraphicalController::load_shader_source(const std::string& path)
{
	std::ifstream fstream(path, std::ifstream::binary);
	fstream.seekg(0, std::ios::end);
	const int length = fstream.tellg();
	fstream.seekg(0, std::ios::beg);

	std::string ret;
	ret.reserve(length);
	ret.assign((std::istreambuf_iterator<char>(fstream)), std::istreambuf_iterator<char>());

	fstream.close();
	return ret;
}

GLuint GraphicalController::load_shader(const std::string& vPath, const std::string& fPath)
{
	const std::string v_src = load_shader_source(FileSystem::instance().m_resource_path + "Shaders\\" + vPath + ".vsh");
	const std::string f_src = load_shader_source(FileSystem::instance().m_resource_path + "Shaders\\" + fPath + ".fsh");
	const char *VertexShaderSource = v_src.c_str();
	const char *FragmentShaderSource = f_src.c_str();
	GLuint Program;
	GLuint VertexShader;
	GLuint FragmentShader;
	Program = glCreateProgram();
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(VertexShader, 1, &VertexShaderSource, NULL);
	glCompileShader(VertexShader);
	if (!CompileSuccessful(VertexShader))
		LOG(FATAL) << "Не удалось скомпилировать вершинный шейдер `" << vPath << "`";

	glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL);
	glCompileShader(FragmentShader);
	if (!CompileSuccessful(FragmentShader))
		LOG(FATAL) << "Не удалось скомпилировать фрагментный шейдер `" << fPath << "`";

	glAttachShader(Program, VertexShader);
	glAttachShader(Program, FragmentShader);
	glLinkProgram(Program);
	if (!LinkSuccessful(Program))
		LOG(FATAL) << "Не удалось слинковать шейдерную программу!";
	glValidateProgram(Program);
	if (!ValidateSuccessful(Program))
		LOG(FATAL) << "Ошибка при проверке шейдерной программы!";

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
	glGenerateMipmap(GL_TEXTURE_2D);
	return texture;
}

void GraphicalController::parser(const std::string& command)
{
	LOG(INFO) << command;
	std::size_t found = 0;
	std::string args;
	command_e key;
	std::vector<std::string> arg;
	found = command.find(" ");
	if (found != std::string::npos)
	{
		key = m_commands.find(command.substr(0, found))->second;
		args = command.substr(found + 1);
		std::size_t pos = 0;
		do
		{
			found = args.find(",", pos);
			if (found != -1)
			{
				arg.push_back(args.substr(pos, found - pos));
				pos = found + 1;
			}
			else break;
		} while (true);
		arg.push_back(args.substr(pos));
	}
	else {
		key = m_commands.find(command)->second;
	}
	switch (key)
	{
	case command_e::single:
	{
		TileManager_Single* m_tile_manager = new TileManager_Single();
		m_tile_manager->load_from_file(arg[0], object_direction_e::down, 0);
		m_tile_manager->m_index = m_tile_managers.size();
		m_tile_managers.push_back(m_tile_manager);
		break;
	}
	case command_e::single_animate:
	{
		TileManager_Single_animate* m_tile_manager = new TileManager_Single_animate();
		m_tile_manager->load_from_file(arg[0], object_direction_e::down, 0);
		m_tile_manager->load_from_file(arg[1], object_direction_e::down, 1);
		m_tile_manager->load_from_file(arg[2], object_direction_e::down, 2);
		m_tile_manager->load_from_file(arg[3], object_direction_e::down, 3);
		m_tile_manager->m_index = m_tile_managers.size();
		m_tile_managers.push_back(m_tile_manager);
		break;
	}
	case command_e::rotating:
	{
		TileManager_rotating* m_tile_manager = new TileManager_rotating();
		m_tile_manager->load_from_file(arg[0], object_direction_e::top, 0);
		m_tile_manager->load_from_file(arg[1], object_direction_e::right, 2);
		m_tile_manager->load_from_file(arg[2], object_direction_e::down, 4);
		m_tile_manager->load_from_file(arg[3], object_direction_e::left, 6);
		m_tile_manager->m_index = m_tile_managers.size();
		m_tile_managers.push_back(m_tile_manager);
		break;
	}
	case command_e::rotating8:
	{
		TileManager_rotating8* m_tile_manager = new TileManager_rotating8();
		m_tile_manager->load_from_file(arg[0] + "_top", object_direction_e::top, 0);
		m_tile_manager->load_from_file(arg[0] + "_topright", object_direction_e::topright, 1);
		m_tile_manager->load_from_file(arg[0] + "_right", object_direction_e::right, 2);
		m_tile_manager->load_from_file(arg[0] + "_downright", object_direction_e::downright, 3);
		m_tile_manager->load_from_file(arg[0] + "_down", object_direction_e::down, 4);
		m_tile_manager->load_from_file(arg[0] + "_downleft", object_direction_e::downleft, 5);
		m_tile_manager->load_from_file(arg[0] + "_left", object_direction_e::left, 6);
		m_tile_manager->load_from_file(arg[0] + "_topleft", object_direction_e::topleft, 7);
		m_tile_manager->m_index = m_tile_managers.size();
		m_tile_managers.push_back(m_tile_manager);
		break;
	}
	}
}

void GraphicalController::load_configuration()
{
	m_commands["single"] = command_e::single;
	m_commands["single_animate"] = command_e::single_animate;
	m_commands["rotating"] = command_e::rotating;
	m_commands["rotating8"] = command_e::rotating8;
	bytearray buffer;
	FileSystem::instance().load_from_file(FileSystem::instance().m_resource_path + "Configs\\Tiles.txt", buffer);
	std::string config(buffer);
	std::size_t pos = 0;
	std::size_t found = 0;
	while (pos != std::string::npos)
	{
		found = config.find("\r\n", pos);
		if (found != std::string::npos)
		{
			if (found - pos > 0)
			{
				parser(config.substr(pos, found - pos));

			}
			pos = found + 2;
		}
		else {
			parser(config.substr(pos));
			pos = std::string::npos;
		}
	}
}