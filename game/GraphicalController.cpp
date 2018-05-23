#include "GraphicalController.h"
#include "utils/log.h"
#include "TileManager.h"


GraphicalController::GraphicalController(dimension_t size)
{
	m_size = size;
	
	Load_font(FileSystem::instance().m_resource_path + "Fonts\\9746.ttf");

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
		m_actions[15] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_14.bmp");

		m_horizontal_shader = load_shader("EoS_blur", "EoS_blur_horizontal");
		m_vertical_shader = load_shader("EoS_blur", "EoS_blur_vertical");
		m_mask_shader = load_shader("EoS_mask", "EoS_mask");
		m_mask_shader2 = load_shader("EoS_mask", "EoS_mask2");
		m_tile_shader = load_shader("EoS_tile", "EoS_tile");
		m_tile_shader_hide = load_shader("EoS_tile", "EoS_tile_hide");
		m_empty_01 = create_empty_texture(m_size);
		m_empty_02 = create_empty_texture(m_size);
		m_empty_03 = create_empty_texture(m_size);
		m_blur = create_empty_texture(dimension_t(m_size.w/4, m_size.h/4));

		m_close = load_texture(FileSystem::instance().m_resource_path + "Tiles\\EoS_Close.bmp");
		m_preselect = load_texture(FileSystem::instance().m_resource_path + "Tiles\\preselection.bmp");
		m_select = load_texture(FileSystem::instance().m_resource_path + "Tiles\\iso_select.bmp");
		m_cursor = load_texture(FileSystem::instance().m_resource_path + "Tiles\\EoS_Cursor.bmp");
		m_no_image = load_texture(FileSystem::instance().m_resource_path + "Tiles\\no_tile.bmp");
		m_visible = load_texture(FileSystem::instance().m_resource_path + "Tiles\\visible.bmp");
		m_dir = load_texture(FileSystem::instance().m_resource_path + "Tiles\\directions.bmp");
		m_logo = load_texture(FileSystem::instance().m_resource_path + "Tiles\\logo.bmp");
	}
	catch(std::logic_error e)
	{
		LOG(FATAL) << "Ошибка при загрузке ресурсов: " << e.what();
	}

	m_scissors.push_front(frectangle_t(0.0f, 0.0f, (float)m_size.w, (float)m_size.h));
	glGenFramebuffers(1, &m_FBO);
}

void GraphicalController::Load_font(std::string font_filename)
{
	m_error = FT_Init_FreeType(&m_library);
	if (m_error != 0)
	{
		LOG(FATAL) << "Ошибка инициализации FreeType";
	}

	m_error = FT_New_Face(m_library, font_filename.c_str(), 0, &m_face);
	if (m_error != 0)
	{
		LOG(FATAL) << "Ошибка инициализации шрифта";
	}

	m_error = FT_Set_Pixel_Sizes(m_face, font_size_c, 0);
	m_slot = m_face->glyph;
	get_symbol(u'?');
	get_symbol(u' ');
	m_unicode_symbols[u' '] = m_unicode_symbols[u'?'];
}

font_symbol_t& GraphicalController::get_symbol(char16_t value)
{
	std::u16string temp{ value };
	auto symbol = m_unicode_symbols.find(value);
	if(symbol == m_unicode_symbols.end())
	{
		
		m_error = FT_Load_Char(m_face, value, FT_LOAD_RENDER);

		if (m_error != 0)
		{
			LOG(INFO) << "Ошибка загрузки символа";
			return m_unicode_symbols[u'?'];
		}

		bool is_ok = wglMakeCurrent(Application::instance().m_hDC, Application::instance().subhRC);
		GLenum err;
		
		GLuint texture;
		glGenTextures(1, &texture);
		
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_slot->bitmap.width, m_slot->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, m_slot->bitmap.buffer);
		LOG(INFO) << std::to_string(texture) <<"    "<< Parser::UTF16_to_CP1251({ value });
		font_symbol_t s;
		s.id = texture;
		s.size.w = m_slot->bitmap.width;
		s.size.h = m_slot->bitmap.rows;
		s.bearing.w = m_slot->bitmap_left;
		s.bearing.h = s.size.h - m_slot->bitmap_top;
		m_unicode_symbols[value] = s;
		wglMakeCurrent(Application::instance().m_hDC, Application::instance().m_hRC);
		return m_unicode_symbols[value];
	}
	else
	{
		return symbol->second;
	}
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

void GraphicalController::set_VSync(bool sync)
{
	// Function pointer for the wgl extention function we need to enable/disable
	// vsync
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = nullptr;

	const char *extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));

	if (strstr(extensions, "WGL_EXT_swap_control") == nullptr)
	{
		return;
	}
	else
	{
		wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALPROC>(wglGetProcAddress("wglSwapIntervalEXT"));

		if (wglSwapIntervalEXT)
			wglSwapIntervalEXT(sync);
	}
}

//void GraphicalController::output_text(int x, int y, std::string& Text, int sizex, int sizey)
//{
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glUseProgram(0);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, m_font);
//	int x0, y0, x1, y1, x2, y2, x3, y3;
//	double xt0, yt0, xt1, yt1, xt2, yt2, xt3, yt3;
//	unsigned char  i, j;
//	unsigned char ASCII;
//	for (int k = 0; k<Text.length(); k++)
//	{
//		ASCII = (unsigned char)Text[k];
//		j = ASCII / 16;
//		i = ASCII - j * 16;
//		j = 15 - j;
//		x0 = x + k*(sizex + 1);
//		y0 = y;
//		x1 = x + k*(sizex + 1);
//		y1 = y + sizey;
//		x2 = x + sizex + k*(sizex + 1);
//		y2 = y + sizey;
//		x3 = x + sizex + k*(sizex + 1);
//		y3 = y;
//		xt0 = i*0.0625;
//		yt0 = (j + 1)*0.0625;
//		xt1 = i*0.0625;
//		yt1 = j*0.0625;
//		xt2 = (i + 1)*0.0625;
//		yt2 = j*0.0625;
//		xt3 = (i + 1)*0.0625;
//		yt3 = (j + 1)*0.0625;
//		glBegin(GL_QUADS);
//		glTexCoord2d(xt0, yt0); glVertex2d(x0, y0);
//		glTexCoord2d(xt1, yt1); glVertex2d(x1, y1);
//		glTexCoord2d(xt2, yt2); glVertex2d(x2, y2);
//		glTexCoord2d(xt3, yt3); glVertex2d(x3, y3);
//		glEnd();
//	}
//}

void GraphicalController::output_text(int x, int y, std::u16string& Text, int sizex, int sizey)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(0);
	glActiveTexture(GL_TEXTURE0);
	y = y + (m_face->size->metrics.ascender >> 6);
	int x0, y0, x1, y1, x2, y2, x3, y3;
	int shift_x = 0;
	for (int k = 0; k<Text.length(); k++)
	{
		font_symbol_t& fs = get_symbol(Text[k]);
		if (Text[k] != 32)
		{
			glBindTexture(GL_TEXTURE_2D, fs.id);
			x0 = x + shift_x;
			y0 = y + fs.bearing.h;
			x1 = x0;
			y1 = y - fs.size.h + fs.bearing.h + 1;
			x2 = x + shift_x + fs.size.w - 1;
			y2 = y1;
			x3 = x2;
			y3 = y0;
			glBegin(GL_QUADS);
			glTexCoord2d(0, 1); glVertex2d(x0, y0);
			glTexCoord2d(0, 0); glVertex2d(x1, y1);
			glTexCoord2d(1, 0); glVertex2d(x2, y2);
			glTexCoord2d(1, 1); glVertex2d(x3, y3);
			glEnd();
		}
		shift_x += fs.size.w;
	}
}


std::size_t GraphicalController::measure_text_width(std::u16string& Text)
{
	int result = 0;
	for (int k = 0; k<Text.length(); k++)
	{
		font_symbol_t& fs = get_symbol(Text[k]);
		result += fs.size.w;
	}
	return result;
}

void GraphicalController::center_text(int x, int y, std::u16string Text, int sizex, int sizey)
{
	int width = 0;
	for (int k = 0; k<Text.length(); k++)
	{
		font_symbol_t& fs = get_symbol(Text[k]);
		width += fs.size.w;
	}
	int cx = x - (width / 2);
	int cy = y - ((m_face->size->metrics.ascender - m_face->size->metrics.descender) >> 7);
	output_text(cx, cy, Text, sizex, sizey);
}


position_t GraphicalController::center_aling_to_point(int x, int y, std::u16string text)
{
	return position_t(x - (measure_text_width(text) / 2), y);
}

bool GraphicalController::add_scissor(const frectangle_t& rect)
{
	frectangle_t GlobalScissor = m_scissors.front();
	frectangle_t LocalScissor(rect.x, (float)m_size.h - rect.y - rect.h, rect.x + rect.w, (float)m_size.h - rect.y);
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
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, x, m_size.h - y - height, width, height);
	double Tx = width / (float)m_size.w;
	double Ty = height / (float)m_size.h;
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_empty_02, 0);
	glUseProgram(m_horizontal_shader);
	set_uniform_sampler(m_horizontal_shader, "Map",0);
	draw_sprite_FBO(Tx, Ty, rectangle_t(0, m_size.h, width, -height));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_empty_01, 0);
	glUseProgram(m_vertical_shader);
	set_uniform_sampler(m_vertical_shader, "Map",0);
	glBindTexture(GL_TEXTURE_2D, m_empty_02);
	draw_sprite_FBO(Tx, Ty, rectangle_t(0, m_size.h, width, -height));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glEnable(GL_SCISSOR_TEST);
	glBindTexture(GL_TEXTURE_2D, m_empty_01);
	draw_sprite_FBO(Tx, Ty, rectangle_t(x,y+height, width, -height));
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
		//MessageBox(NULL, path.c_str(), "", MB_OK);
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

GLuint GraphicalController::png_texture_load(const std::string& path)
{
	
	// This function was originally written by David Grayson for
	// https://github.com/DavidEGrayson/ahrs-visualizer

	png_byte header[8];

	const char * file_name = path.c_str();
	FILE *fp = fopen(file_name, "rb");
	if (fp == 0)
	{
		perror(file_name);
		return 0;
	}

	// read the header
	fread(header, 1, 8, fp);

	if (png_sig_cmp(header, 0, 8))
	{
		fprintf(stderr, "error: %s is not a PNG.\n", file_name);
		fclose(fp);
		return 0;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		fprintf(stderr, "error: png_create_read_struct returned 0.\n");
		fclose(fp);
		return 0;
	}

	// create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		fprintf(stderr, "error: png_create_info_struct returned 0.\n");
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		return 0;
	}

	// create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		fprintf(stderr, "error: png_create_info_struct returned 0.\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		fclose(fp);
		return 0;
	}

	// the code in this if statement gets called if libpng encounters an error
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "error from libpng\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return 0;
	}

	// init png reading
	png_init_io(png_ptr, fp);

	// let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	// variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 temp_width, temp_height;

	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
		NULL, NULL, NULL);

	//if (width) { *width = temp_width; }
	//if (height) { *height = temp_height; }

	//printf("%s: %lux%lu %d\n", file_name, temp_width, temp_height, color_type);

	if (bit_depth != 8)
	{
		fprintf(stderr, "%s: Unsupported bit depth %d.  Must be 8.\n", file_name, bit_depth);
		return 0;
	}

	GLint format;
	switch (color_type)
	{
	case PNG_COLOR_TYPE_RGB:
		format = GL_RGB;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		format = GL_RGBA;
		break;
	default:
		fprintf(stderr, "%s: Unknown libpng color type %d.\n", file_name, color_type);
		return 0;
	}

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// glTexImage2d requires rows to be 4-byte aligned
	rowbytes += 3 - ((rowbytes - 1) % 4);

	// Allocate the image_data as a big block, to be given to opengl
	png_byte * image_data = (png_byte *)malloc(rowbytes * temp_height * sizeof(png_byte) + 15);
	if (image_data == NULL)
	{
		fprintf(stderr, "error: could not allocate memory for PNG image data\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return 0;
	}

	// row_pointers is for pointing to image_data for reading the png with libpng
	png_byte ** row_pointers = (png_byte **)malloc(temp_height * sizeof(png_byte *));
	if (row_pointers == NULL)
	{
		fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		free(image_data);
		fclose(fp);
		return 0;
	}

	// set the individual row_pointers to point at the correct offsets of image_data
	for (unsigned int i = 0; i < temp_height; i++)
	{
		row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
	}

	// read the png into image_data through row_pointers
	LOG(INFO) << file_name;
	png_read_image(png_ptr, row_pointers);

	// Generate the OpenGL texture object
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	/*glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);*/
	//gluBuild2DMipmaps(GL_TEXTURE_2D, format, temp_width, temp_height, format, GL_UNSIGNED_BYTE, image_data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, format, temp_width, temp_height, 0, format, GL_UNSIGNED_BYTE, image_data);
	

	//LOG(INFO) << std::to_string(temp_width)<<"  "<< std::to_string(temp_height);
	/*glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format, temp_width, temp_height, 0, format, GL_UNSIGNED_BYTE, image_data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);*/

	// clean up
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	free(image_data);
	free(row_pointers);
	fclose(fp);
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

void GraphicalController::draw_sprite(rectangle_t rect)
{
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2d(rect.a.x, rect.b.y);
	glTexCoord2d(0, 1); glVertex2d(rect.a.x, rect.a.y);
	glTexCoord2d(1, 1); glVertex2d(rect.b.x, rect.a.y);
	glTexCoord2d(1, 0); glVertex2d(rect.b.x, rect.b.y);
	glEnd();
}

void GraphicalController::draw_rectangle(rectangle_t rect)
{
	//glBegin(GL_LINE_LOOP);
	//glVertex2d(px, py);
	//glVertex2d(px + m_size.w, py);
	//glVertex2d(px + m_size.w, py + m_size.h);
	//glVertex2d(px, py + m_size.h);
	//glEnd();
	glBegin(GL_LINES);
	glVertex2d(rect.a.x, rect.b.y);
	glVertex2d(rect.a.x, rect.a.y);
	glVertex2d(rect.a.x, rect.a.y);
	glVertex2d(rect.b.x, rect.a.y);
	glVertex2d(rect.b.x, rect.a.y);
	glVertex2d(rect.b.x, rect.b.y);
	glVertex2d(rect.b.x, rect.b.y);
	glVertex2d(rect.a.x, rect.b.y);
	glEnd();
}

void GraphicalController::stroke_cell(int x,int y, int xs, int ys)
{
	int x0, y0;
	x0 = (x - y) * tile_size_x_half + xs;
	y0 = (x + y) * tile_size_y_half + ys;
	glBegin(GL_LINES);
	glVertex2d(x0, y0 - tile_size_y_half);
	glVertex2d(x0 + tile_size_x_half, y0 - tile_size_y);
	glVertex2d(x0 + tile_size_x_half, y0 - tile_size_y);
	glVertex2d(x0 + tile_size_x, y0 - tile_size_y_half);
	glVertex2d(x0 + tile_size_x, y0 - tile_size_y_half);
	glVertex2d(x0 + tile_size_x_half, y0);
	glVertex2d(x0 + tile_size_x_half, y0);
	glVertex2d(x0, y0 - tile_size_y_half);
	glEnd();
}

void GraphicalController::draw_tile(tile_t& tile, rectangle_t rect)
{
	glBegin(GL_QUADS);
	glTexCoord2d(tile.coordinat[0], tile.coordinat[1]); glVertex2d(rect.a.x, rect.b.y);
	glTexCoord2d(tile.coordinat[0], tile.coordinat[3]); glVertex2d(rect.a.x, rect.a.y);
	glTexCoord2d(tile.coordinat[2], tile.coordinat[3]); glVertex2d(rect.b.x, rect.a.y);
	glTexCoord2d(tile.coordinat[2], tile.coordinat[1]); glVertex2d(rect.b.x, rect.b.y);
	glEnd();
}

void GraphicalController::draw_sprite_FBO(double TexWidth, double TexHeight, rectangle_t rect)
{
	glBegin(GL_QUADS);
	glTexCoord2d(0, TexHeight); 
	glVertex2d(rect.a.x, rect.b.y);
	glTexCoord2d(0, 0); 
	glVertex2d(rect.a.x, rect.a.y);
	glTexCoord2d(TexWidth, 0); 
	glVertex2d(rect.b.x, rect.a.y);
	glTexCoord2d(TexWidth, TexHeight); 
	glVertex2d(rect.b.x, rect.b.y);
	glEnd();
}

void GraphicalController::draw_tile_FBO(double tx1, double ty1, double tx2, double ty2, rectangle_t rect)
{
	glBegin(GL_QUADS);
	glMultiTexCoord2f(GL_TEXTURE0, tx1, ty2);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
	glVertex2d(rect.a.x, rect.b.y);
	glMultiTexCoord2f(GL_TEXTURE0, tx1, ty1);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
	glVertex2d(rect.a.x, rect.a.y);
	glMultiTexCoord2f(GL_TEXTURE0, tx2, ty1);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
	glVertex2d(rect.b.x, rect.a.y);
	glMultiTexCoord2f(GL_TEXTURE0, tx2, ty2);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
	glVertex2d(rect.b.x, rect.b.y);
	glEnd();
}