#define STB_IMAGE_IMPLEMENTATION
#include "GraphicalController.h"
#include "TileManager.h"
#include <stb_image.h>

using namespace gl;

void GraphicalController::check_gl_error(const std::string& text)
{
	auto err = glGetError();
	while (err != GL_NO_ERROR) {
		std::string error;

		switch (err) {
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}

		Logger::instance().info (text + ": GL_" + error);
		err = glGetError();
	}
}

GraphicalController::GraphicalController(const dimension_t size)
{
	m_size = size;
	
	load_font(FileSystem::instance().m_resource_path + "Fonts\\augusta_modern.ttf");

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
		m_actions[16] = load_texture(FileSystem::instance().m_resource_path + "Tiles\\Action_15.bmp");
		m_actions[17] = png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\up_z_level.png");

		m_horizontal_shader = load_shader("EoS_blur", "EoS_blur_horizontal");
		m_vertical_shader = load_shader("EoS_blur", "EoS_blur_vertical");
		m_mask_shader = load_shader("EoS_mask", "EoS_mask");
		m_mask_shader2 = load_shader("EoS_mask", "EoS_mask2");
		m_tile_shader = load_shader("EoS_tile", "EoS_tile");
		m_tile_shader_hide = load_shader("EoS_tile", "EoS_tile_hide");
		m_atlas_shader = load_shader("EoS_Tile_atlas", "EoS_Tile_atlas");
		m_empty_01 = create_empty_texture(m_size);
		m_empty_02 = create_empty_texture(m_size);
		m_empty_03 = create_empty_texture(m_size);
		m_blur = create_empty_texture(dimension_t(m_size.w/4, m_size.h/4));

		m_close = load_texture(FileSystem::instance().m_resource_path + "Tiles\\EoS_Close.bmp");
		m_preselect = load_texture(FileSystem::instance().m_resource_path + "Tiles\\preselection.bmp");
		m_select = load_texture(FileSystem::instance().m_resource_path + "Tiles\\iso_select.bmp");
		m_cursor = load_texture(FileSystem::instance().m_resource_path + "Tiles\\EoS_Cursor.bmp");
		m_no_image = png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\atlas_1.png");
		m_visible = load_texture(FileSystem::instance().m_resource_path + "Tiles\\visible.bmp");
		m_dir = load_texture(FileSystem::instance().m_resource_path + "Tiles\\directions.bmp");
		m_logo = load_texture(FileSystem::instance().m_resource_path + "Tiles\\logo.bmp");
	}
	catch(std::logic_error& e)
	{
		Logger::instance().critical("Ошибка при загрузке ресурсов: {}" , e.what());
	}

	m_scissors.push_front(frectangle_t(0.0f, 0.0f, (float)m_size.w, (float)m_size.h));
	glGenFramebuffers(1, &m_FBO);
}

void GraphicalController::load_font(const std::string& font_filename)
{
	m_error = FT_Init_FreeType(&m_library);
	if (m_error != 0)
	{
		Logger::instance().critical ("Ошибка инициализации FreeType");
	}

	m_error = FT_New_Face(m_library, font_filename.c_str(), 0, &m_face);
	if (m_error != 0)
	{
		Logger::instance().critical( "Ошибка инициализации шрифта");
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
			Logger::instance().critical( "Ошибка загрузки символа");
			return m_unicode_symbols[u'?'];
		}

		bool is_ok = wglMakeCurrent(Application::instance().m_hDC, Application::instance().subhRC);
		GLenum err;
		
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_slot->bitmap.width, m_slot->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, m_slot->bitmap.buffer);
		font_symbol_t s;
		s.id = texture;
		s.size.w = int(m_slot->bitmap.width);
		s.size.h = int(m_slot->bitmap.rows);
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

bool GraphicalController::compile_successful(int obj)
{
	int status;
	glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
	return GLboolean(status) == GL_TRUE;
}

bool GraphicalController::link_successful(int obj)
{
	int status;
	glGetProgramiv(obj, GL_LINK_STATUS, &status);
	return GLboolean(status) == GL_TRUE;
}

bool GraphicalController::validate_successful(int obj)
{
	int status;
	glGetProgramiv(obj, GL_VALIDATE_STATUS, &status);
	return GLboolean(status) == GL_TRUE;
}

void GraphicalController::set_VSync(bool sync)
{
	// Function pointer for the wgl extention function we need to enable/disable
	// vsync
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wgl_swap_interval_ext = nullptr;

	const auto extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));

	if (strstr(extensions, "WGL_EXT_swap_control") == nullptr)
	{
		return;
	}
	else
	{
		wgl_swap_interval_ext = reinterpret_cast<PFNWGLSWAPINTERVALPROC>(wglGetProcAddress("wglSwapIntervalEXT"));

		if (wgl_swap_interval_ext)
			wgl_swap_interval_ext(sync);
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

void GraphicalController::output_text(int x, int y, std::u16string& text, int sizex, int sizey)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(0);
	glActiveTexture(GL_TEXTURE0);
	y = y + (m_face->size->metrics.ascender >> 6);
	auto shift_x = 0;
	for (auto k : text)
	{
		auto& fs = get_symbol(k);
		if (k != 32)
		{
			glBindTexture(GL_TEXTURE_2D, fs.id);
			const auto x0 = x + shift_x;
			const auto y0 = y + fs.bearing.h;
			const auto x1 = x0;
			const auto y1 = y - fs.size.h + fs.bearing.h + 1;
			const auto x2 = x + shift_x + fs.size.w - 1;
			const auto y2 = y1;
			const auto x3 = x2;
			const auto y3 = y0;
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


std::size_t GraphicalController::measure_text_width(std::u16string& text)
{
	auto result = 0;
	for (auto k : text)
	{
		auto& fs = get_symbol(k);
		result += fs.size.w;
	}
	return result;
}

void GraphicalController::center_text(int x, int y, std::u16string text, int sizex, int sizey)
{
	auto width = 0;
	for (auto k : text)
	{
		auto& fs = get_symbol(k);
		width += fs.size.w;
	}
	const auto cx = x - (width / 2);
	const auto cy = y - ((m_face->size->metrics.ascender - m_face->size->metrics.descender) >> 7);
	output_text(cx, cy, text, sizex, sizey);
}


position_t GraphicalController::center_aling_to_point(int x, int y, std::u16string text)
{
	return position_t(x - (measure_text_width(text) / 2), y);
}

bool GraphicalController::add_scissor(const frectangle_t& rect)
{
	const auto global_scissor = m_scissors.front();
	frectangle_t local_scissor(rect.x, float(m_size.h) - rect.y - rect.h, rect.x + rect.w, float(m_size.h) - rect.y);
	if ((global_scissor.x > local_scissor.w) || (global_scissor.w < local_scissor.x) || (global_scissor.y > local_scissor.h) || (global_scissor.h < local_scissor.y)) return false;
	if (local_scissor.x < global_scissor.x) local_scissor.x = global_scissor.x;
	if (local_scissor.w > global_scissor.w) local_scissor.w = global_scissor.w;
	if (local_scissor.y < global_scissor.y) local_scissor.y = global_scissor.y;
	if (local_scissor.h > global_scissor.h) local_scissor.h = global_scissor.h;
	m_scissors.push_front(local_scissor);
	glScissor(local_scissor.x, local_scissor.y, local_scissor.w - local_scissor.x, local_scissor.h - local_scissor.y);
	return true;
}

void GraphicalController::remove_scissor()
{
	m_scissors.pop_front();
	const auto scissor = m_scissors.front();
	glScissor(scissor.x, scissor.y, scissor.w - scissor.x, scissor.h - scissor.y);
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
	draw_sprite_fbo(Tx, Ty, rectangle_t(0, m_size.h, width, -height));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_empty_01, 0);
	glUseProgram(m_vertical_shader);
	set_uniform_sampler(m_vertical_shader, "Map",0);
	glBindTexture(GL_TEXTURE_2D, m_empty_02);
	draw_sprite_fbo(Tx, Ty, rectangle_t(0, m_size.h, width, -height));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glEnable(GL_SCISSOR_TEST);
	glBindTexture(GL_TEXTURE_2D, m_empty_01);
	draw_sprite_fbo(Tx, Ty, rectangle_t(x,y+height, width, -height));
	glColor4f(1.0, 1.0, 1.0, 1.0);
}

bool GraphicalController::set_uniform_vector(const GLuint program, const char * name, const float * value)
{
	const auto location = glGetUniformLocation(program, name);
	if (location < 0)
		return false;
	glUniform4fv(location, 1, value);
	return true;
}

bool GraphicalController::set_uniform_sampler(const GLuint object, const char * name, const int index)
{
	const auto location = glGetUniformLocation(object, name);
	if (location < 0)
		return false;
	glUniform1i(location, index);
	return true;
}

bool GraphicalController::set_uniform_ptr(const GLuint program, const char * name, const int value)
{
	const auto loc = glGetUniformLocation(program, name);
	if (loc < 0)
		return false;
	glUniform1iv(loc, 1, &value);
	return true;
}

bool GraphicalController::set_uniform_float(const GLuint program, const char * name, const float value)
{
	const auto location = glGetUniformLocation(program, name);
	if (location < 0)
		return false;
	glUniform1f(location,value);
	return true;
}

#define SWAP_ROWS_DOUBLE(a, b) { double* _tmp = a; (a) = (b); (b) = _tmp; }
#define SWAP_ROWS_FLOAT(a, b) { float* _tmp = a; (a) = (b); (b) = _tmp; }
#define MAT(m, r, c) (m)[(c) * 4 + (r)]

// This code comes directly from GLU except that it is for float
int glhInvertMatrixf2(GLdouble* m, GLdouble* out)
{
	float wtmp[4][8];
	float m0, s;
	float* r0, * r1, * r2, * r3;
	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
	r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
		r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
		r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
		r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
		r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
		r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
		r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
		r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
		r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
		r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
		r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
		r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;
	/* choose pivot - or die */
	if (fabsf(r3[0]) > fabsf(r2[0]))
		SWAP_ROWS_FLOAT(r3, r2);
	if (fabsf(r2[0]) > fabsf(r1[0]))
		SWAP_ROWS_FLOAT(r2, r1);
	if (fabsf(r1[0]) > fabsf(r0[0]))
		SWAP_ROWS_FLOAT(r1, r0);
	if (0.0 == r0[0])
		return 0;
	/* eliminate first variable */
	auto m1 = r1[0] / r0[0];
	auto m2 = r2[0] / r0[0];
	auto m3 = r3[0] / r0[0];
	s = r0[1];
	r1[1] -= m1 * s;
	r2[1] -= m2 * s;
	r3[1] -= m3 * s;
	s = r0[2];
	r1[2] -= m1 * s;
	r2[2] -= m2 * s;
	r3[2] -= m3 * s;
	s = r0[3];
	r1[3] -= m1 * s;
	r2[3] -= m2 * s;
	r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0) {
		r1[4] -= m1 * s;
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r0[5];
	if (s != 0.0) {
		r1[5] -= m1 * s;
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r0[6];
	if (s != 0.0) {
		r1[6] -= m1 * s;
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r0[7];
	if (s != 0.0) {
		r1[7] -= m1 * s;
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}
	/* choose pivot - or die */
	if (fabsf(r3[1]) > fabsf(r2[1]))
		SWAP_ROWS_FLOAT(r3, r2);
	if (fabsf(r2[1]) > fabsf(r1[1]))
		SWAP_ROWS_FLOAT(r2, r1);
	if (0.0 == r1[1])
		return 0;
	/* eliminate second variable */
	m2 = r2[1] / r1[1];
	m3 = r3[1] / r1[1];
	r2[2] -= m2 * r1[2];
	r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3];
	r3[3] -= m3 * r1[3];
	s = r1[4];
	if (0.0 != s) {
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r1[5];
	if (0.0 != s) {
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r1[6];
	if (0.0 != s) {
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r1[7];
	if (0.0 != s) {
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}
	/* choose pivot - or die */
	if (fabsf(r3[2]) > fabsf(r2[2]))
		SWAP_ROWS_FLOAT(r3, r2);
	if (0.0 == r2[2])
		return 0;
	/* eliminate third variable */
	m3 = r3[2] / r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
		r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];
	/* last check */
	if (0.0 == r3[3])
		return 0;
	s = 1.0 / r3[3];		/* now back substitute row 3 */
	r3[4] *= s;
	r3[5] *= s;
	r3[6] *= s;
	r3[7] *= s;
	m2 = r2[3];			/* now back substitute row 2 */
	s = 1.0 / r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
		r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
		r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
		r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
	m1 = r1[2];			/* now back substitute row 1 */
	s = 1.0 / r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
		r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
		r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
	m0 = r0[1];			/* now back substitute row 0 */
	s = 1.0 / r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
		r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);
	MAT(out, 0, 0) = r0[4];
	MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
	MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
	MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
	MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
	MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
	MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
	MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
	MAT(out, 3, 3) = r3[7];
	return 1;
}

void multiply_matrix_by_vector4_by4_open_gl_float(GLdouble* resultvector, const GLdouble* matrix, const GLdouble* pvector)
{
	resultvector[0] = matrix[0] * pvector[0] + matrix[4] * pvector[1] + matrix[8] * pvector[2] + matrix[12] * pvector[3];
	resultvector[1] = matrix[1] * pvector[0] + matrix[5] * pvector[1] + matrix[9] * pvector[2] + matrix[13] * pvector[3];
	resultvector[2] = matrix[2] * pvector[0] + matrix[6] * pvector[1] + matrix[10] * pvector[2] + matrix[14] * pvector[3];
	resultvector[3] = matrix[3] * pvector[0] + matrix[7] * pvector[1] + matrix[11] * pvector[2] + matrix[15] * pvector[3];
}


void multiply_matrices4_by4_open_gl_float(GLdouble* result, const GLdouble* matrix1, const GLdouble* matrix2)
{
	result[0] = matrix1[0] * matrix2[0] +
		matrix1[4] * matrix2[1] +
		matrix1[8] * matrix2[2] +
		matrix1[12] * matrix2[3];
	result[4] = matrix1[0] * matrix2[4] +
		matrix1[4] * matrix2[5] +
		matrix1[8] * matrix2[6] +
		matrix1[12] * matrix2[7];
	result[8] = matrix1[0] * matrix2[8] +
		matrix1[4] * matrix2[9] +
		matrix1[8] * matrix2[10] +
		matrix1[12] * matrix2[11];
	result[12] = matrix1[0] * matrix2[12] +
		matrix1[4] * matrix2[13] +
		matrix1[8] * matrix2[14] +
		matrix1[12] * matrix2[15];
	result[1] = matrix1[1] * matrix2[0] +
		matrix1[5] * matrix2[1] +
		matrix1[9] * matrix2[2] +
		matrix1[13] * matrix2[3];
	result[5] = matrix1[1] * matrix2[4] +
		matrix1[5] * matrix2[5] +
		matrix1[9] * matrix2[6] +
		matrix1[13] * matrix2[7];
	result[9] = matrix1[1] * matrix2[8] +
		matrix1[5] * matrix2[9] +
		matrix1[9] * matrix2[10] +
		matrix1[13] * matrix2[11];
	result[13] = matrix1[1] * matrix2[12] +
		matrix1[5] * matrix2[13] +
		matrix1[9] * matrix2[14] +
		matrix1[13] * matrix2[15];
	result[2] = matrix1[2] * matrix2[0] +
		matrix1[6] * matrix2[1] +
		matrix1[10] * matrix2[2] +
		matrix1[14] * matrix2[3];
	result[6] = matrix1[2] * matrix2[4] +
		matrix1[6] * matrix2[5] +
		matrix1[10] * matrix2[6] +
		matrix1[14] * matrix2[7];
	result[10] = matrix1[2] * matrix2[8] +
		matrix1[6] * matrix2[9] +
		matrix1[10] * matrix2[10] +
		matrix1[14] * matrix2[11];
	result[14] = matrix1[2] * matrix2[12] +
		matrix1[6] * matrix2[13] +
		matrix1[10] * matrix2[14] +
		matrix1[14] * matrix2[15];
	result[3] = matrix1[3] * matrix2[0] +
		matrix1[7] * matrix2[1] +
		matrix1[11] * matrix2[2] +
		matrix1[15] * matrix2[3];
	result[7] = matrix1[3] * matrix2[4] +
		matrix1[7] * matrix2[5] +
		matrix1[11] * matrix2[6] +
		matrix1[15] * matrix2[7];
	result[11] = matrix1[3] * matrix2[8] +
		matrix1[7] * matrix2[9] +
		matrix1[11] * matrix2[10] +
		matrix1[15] * matrix2[11];
	result[15] = matrix1[3] * matrix2[12] +
		matrix1[7] * matrix2[13] +
		matrix1[11] * matrix2[14] +
		matrix1[15] * matrix2[15];
}

position_t glhUnProjectf(float winx, float winy, float winz, GLdouble* modelview, GLdouble* projection, int* viewport)
{
	// Transformation matrices
	GLdouble m[16], a[16];
	GLdouble in[4], out[4];
	// Calculation for inverting a matrix, compute projection x modelview
	// and store in A[16]
	multiply_matrices4_by4_open_gl_float(a, projection, modelview);
	// Now compute the inverse of matrix A
	if (glhInvertMatrixf2(a, m) == 0)
		return position_t(0, 0);
	// Transformation of normalized coordinates between -1 and 1
	in[0] = (winx - float(viewport[0])) / float(viewport[2]) * 2.0 - 1.0;
	in[1] = (winy - float(viewport[1])) / float(viewport[3]) * 2.0 - 1.0;
	in[2] = 2.0 * winz - 1.0;
	in[3] = 1.0;
	// Objects coordinates
	multiply_matrix_by_vector4_by4_open_gl_float(out, m, in);
	if (out[3] == 0.0) return position_t(0, 0);
	out[3] = double(1.0) / out[3];
	return position_t(out[0] * out[3], out[1] * out[3]);
}

position_t GraphicalController::get_open_gl_position(float x, float y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat win_z;


	// ВНИМАНИЕ! Нельзя вызывать функции gl вне графического потока!
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);
	const auto win_x = x;
	const auto win_y = float(viewport[3]) - y;
	glReadPixels(x, int(win_y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);
	return glhUnProjectf(win_x, win_y, win_z, modelview, projection, viewport);
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
	bytearray buff;
	if (!FileSystem::instance().load_from_file(path, buff))
	{
		//MessageBox(NULL, path.c_str(), "", MB_OK);
		throw std::logic_error("Не удалось загрузить файл `" + path + "`");
	}
	auto header = reinterpret_cast<BITMAPFILEHEADER*>(buff.get());
	auto info = reinterpret_cast<BITMAPINFOHEADER*>(buff.get() + sizeof(*header));
	const auto data = reinterpret_cast<char*>(buff.get() + sizeof(*header) + sizeof(*info));
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info->biWidth, info->biHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	return texture;
}

GLuint GraphicalController::png_texture_load(const std::string& path)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	const auto file_name = path.c_str();
	const auto image_data = stbi_load(file_name,
		&width,
		&height,
		&channels,
		STBI_rgb);

	//GLenum format;
	//switch (channels)
	//{
	//case PNG_COLOR_TYPE_RGB:
	//	format = GL_RGB;
	//	break;
	//case PNG_COLOR_TYPE_RGB_ALPHA:
	//	format = GL_RGBA;
	//	break;
	//default:
	//	fprintf(stderr, "%s: Unknown libpng color type %d.\n", file_name, color_type);
	//	return 0;
	//}
	//

		// Generate the OpenGL texture object
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	/*glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);*/
	//gluBuild2DMipmaps(GL_TEXTURE_2D, format, temp_width, temp_height, format, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);
	return texture;
}

GLuint GraphicalController::texture_array_load(const std::vector<std::string>& path)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	for (std::size_t index = 0; index < path.size(); ++index)
	{
		const auto file_name = path[index].c_str();
		int width, height, channels;
		const auto image_data = stbi_load(file_name, &width, &height, &channels, STBI_rgb_alpha);
		if (index == 0) {
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, path.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		}
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, index, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		stbi_image_free(image_data);
	}
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	m_atlas = texture;
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
	const auto v_src = load_shader_source(FileSystem::instance().m_resource_path + "Shaders\\" + vPath + ".vsh");
	const auto f_src = load_shader_source(FileSystem::instance().m_resource_path + "Shaders\\" + fPath + ".fsh");
	auto vertex_shader_source = v_src.c_str();
	auto fragment_shader_source = f_src.c_str();
	const auto program = glCreateProgram();
	const auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	if (!compile_successful(vertex_shader))
	{
		auto max_length = 0;
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

		// The maxLength includes the NULL character
		std::vector<GLchar> error_log(max_length);
		glGetShaderInfoLog(vertex_shader, max_length, &max_length, &error_log[0]);
		const std::string str(error_log.begin(), error_log.end());
		Logger::instance().critical("Не удалось скомпилировать вершинный шейдер `" + vPath + "` " + str);
	}
		


	glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
	glCompileShader(fragment_shader);
	if (!compile_successful(fragment_shader))
		Logger::instance().critical( "Не удалось скомпилировать фрагментный шейдер `" + fPath + "`");

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	if (!link_successful(program))
		Logger::instance().critical( "Не удалось слинковать шейдерную программу!");
	glValidateProgram(program);
	if (!validate_successful(program))
		Logger::instance().critical ("Ошибка при проверке шейдерной программы!");

	return program;
}

GLint GraphicalController::create_empty_texture(dimension_t size)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.w, size.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);
	return texture;
}

void GraphicalController::draw_sprite(const rectangle_t &rect)
{
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2d(rect.a.x, rect.b.y);
	glTexCoord2d(0, 1); glVertex2d(rect.a.x, rect.a.y);
	glTexCoord2d(1, 1); glVertex2d(rect.b.x, rect.a.y);
	glTexCoord2d(1, 0); glVertex2d(rect.b.x, rect.b.y);
	glEnd();
}

void GraphicalController::draw_rectangle(const rectangle_t rect)
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

void GraphicalController::stroke_cell(const int x, const int y, const int xs, const int ys)
{
	const auto x0 = (x - y) * tile_size_x_half + xs;
	const auto y0 = (x + y) * tile_size_y_half + ys;
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

void GraphicalController::selection_cell(const int x, const int y,const int z, const int xs, const int ys)
{
	const auto x0 = (x - y) * tile_size_x_half + xs;
	const auto y0 = (x + y) * tile_size_y_half + ys;
	const auto y1 = y0 - (z + 1) * tile_size_y;
	glBegin(GL_LINES);
	//glVertex2d(x0, y0 - tile_size_y_half);
	//glVertex2d(x0 + tile_size_x_half, y0 - tile_size_y);
	//glVertex2d(x0 + tile_size_x_half, y0 - tile_size_y);
	//glVertex2d(x0 + tile_size_x, y0 - tile_size_y_half);
	glVertex2d(x0 + tile_size_x, y0 - tile_size_y_half);
	glVertex2d(x0 + tile_size_x_half, y0);
	glVertex2d(x0 + tile_size_x_half, y0);
	glVertex2d(x0, y0 - tile_size_y_half);

	glVertex2d(x0, y1 - tile_size_y_half);
	glVertex2d(x0 + tile_size_x_half, y1 - tile_size_y);
	glVertex2d(x0 + tile_size_x_half, y1 - tile_size_y);
	glVertex2d(x0 + tile_size_x, y1 - tile_size_y_half);
	glVertex2d(x0 + tile_size_x, y1 - tile_size_y_half);
	glVertex2d(x0 + tile_size_x_half, y1);
	glVertex2d(x0 + tile_size_x_half, y1);
	glVertex2d(x0, y1 - tile_size_y_half);

	glVertex2d(x0, y0 - tile_size_y_half);
	glVertex2d(x0, y1 - tile_size_y_half);
	glVertex2d(x0 + tile_size_x_half, y0);
	glVertex2d(x0 + tile_size_x_half, y1);
	glVertex2d(x0 + tile_size_x, y0 - tile_size_y_half);
	glVertex2d(x0 + tile_size_x, y1 - tile_size_y_half);

	glEnd();
}

void GraphicalController::stroke_cube(const int x, const int y, const int z, const int xs, const int ys)
{
	const auto x0 = (x - y) * tile_size_x_half + xs;
	const auto y0 = (x + y - z * 2) * tile_size_y_half + ys;
	const auto y1 = y0 - tile_size_y;
	glBegin(GL_LINES);
	//glVertex2d(x0, y0 - tile_size_y_half);
	//glVertex2d(x0 + tile_size_x_half, y0 - tile_size_y);
	//glVertex2d(x0 + tile_size_x_half, y0 - tile_size_y);
	//glVertex2d(x0 + tile_size_x, y0 - tile_size_y_half);
	glVertex2d(x0 + tile_size_x, y0 - tile_size_y_half);
	glVertex2d(x0 + tile_size_x_half, y0);
	glVertex2d(x0 + tile_size_x_half, y0);
	glVertex2d(x0, y0 - tile_size_y_half);

	glVertex2d(x0, y1 - tile_size_y_half);
	glVertex2d(x0 + tile_size_x_half, y1 - tile_size_y);
	glVertex2d(x0 + tile_size_x_half, y1 - tile_size_y);
	glVertex2d(x0 + tile_size_x, y1 - tile_size_y_half);
	glVertex2d(x0 + tile_size_x, y1 - tile_size_y_half);
	glVertex2d(x0 + tile_size_x_half, y1);
	glVertex2d(x0 + tile_size_x_half, y1);
	glVertex2d(x0, y1 - tile_size_y_half);

	glVertex2d(x0, y0 - tile_size_y_half);
	glVertex2d(x0, y1 - tile_size_y_half);
	glVertex2d(x0 + tile_size_x_half, y0);
	glVertex2d(x0 + tile_size_x_half, y1);
	glVertex2d(x0 + tile_size_x, y0 - tile_size_y_half);
	glVertex2d(x0 + tile_size_x, y1 - tile_size_y_half);

	glEnd();
}

void GraphicalController::draw_tile(tile_t& tile, rectangle_t rect)
{
	glBegin(GL_QUADS);
	glTexCoord2d(tile.coordinates[0], tile.coordinates[1]); glVertex2d(rect.a.x, rect.b.y);
	glTexCoord2d(tile.coordinates[0], tile.coordinates[3]); glVertex2d(rect.a.x, rect.a.y);
	glTexCoord2d(tile.coordinates[2], tile.coordinates[3]); glVertex2d(rect.b.x, rect.a.y);
	glTexCoord2d(tile.coordinates[2], tile.coordinates[1]); glVertex2d(rect.b.x, rect.b.y);
	glEnd();
}

void GraphicalController::draw_sprite_fbo(double tex_width, double tex_height, rectangle_t rect)
{
	glBegin(GL_QUADS);
	glTexCoord2d(0, tex_height); 
	glVertex2d(rect.a.x, rect.b.y);
	glTexCoord2d(0, 0); 
	glVertex2d(rect.a.x, rect.a.y);
	glTexCoord2d(tex_width, 0); 
	glVertex2d(rect.b.x, rect.a.y);
	glTexCoord2d(tex_width, tex_height); 
	glVertex2d(rect.b.x, rect.b.y);
	glEnd();
}

void GraphicalController::draw_tile_fbo(double tx1, double ty1, double tx2, double ty2, rectangle_t rect)
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