#include "GraphicalController.h"

GraphicalController::GraphicalController()
{
	Sprites[0] = LoadTexture("C:\\ExplorersOfSaarum\\plitka_na_pol.bmp", false);
	Sprites[1] = LoadTexture("C:\\ExplorersOfSaarum\\Stena.bmp", false);
	Sprites[2] = LoadTexture("C:\\ExplorersOfSaarum\\sprite_8.bmp", false);
	Sprites[3] = LoadTexture("C:\\ExplorersOfSaarum\\ElfSprite-1.bmp", false);
	Sprites[4] = LoadTexture("C:\\ExplorersOfSaarum\\ElfSprite-2.bmp", false);
	Sprites[5] = LoadTexture("C:\\ExplorersOfSaarum\\ElfSprite-3.bmp", false);
	Sprites[6] = LoadTexture("C:\\ExplorersOfSaarum\\ElfSprite-4.bmp", false);
	Sprites[7] = LoadTexture("C:\\ExplorersOfSaarum\\orc.bmp", false);
	Sprites[8] = LoadTexture("C:\\ExplorersOfSaarum\\select.bmp", false);
	Sprites[9] = LoadTexture("C:\\ExplorersOfSaarum\\Sunduk.bmp", false);
	Sprites[10] = LoadTexture("C:\\ExplorersOfSaarum\\ElfSprite-5.bmp", false);
	Sprites[11] = LoadTexture("C:\\ExplorersOfSaarum\\ElfSprite-6.bmp", false);
	Sprites[12] = LoadTexture("C:\\ExplorersOfSaarum\\ElfSprite-7.bmp", false);
	Sprites[13] = LoadTexture("C:\\ExplorersOfSaarum\\ElfSprite-8.bmp", false);
	Sprites[14] = LoadTexture("C:\\ExplorersOfSaarum\\Elf2.bmp", false);
	Sprites[15] = LoadTexture("C:\\ExplorersOfSaarum\\Stena2.bmp", false);
	Sprites[16] = LoadTexture("C:\\ExplorersOfSaarum\\plitka_na_pol2.bmp", false);
	Sprites[18] = LoadTexture("C:\\ExplorersOfSaarum\\IB2.bmp", false);
	Sprites[19] = LoadTexture("C:\\ExplorersOfSaarum\\Screen.bmp", false);
	Sprites[20] = LoadTexture("C:\\ExplorersOfSaarum\\Screen.bmp", false);
	Sprites[21] = LoadTexture("C:\\ExplorersOfSaarum\\EoS_Cursor.bmp",true);

	Actions[ActionKind_Move] = LoadTexture("C:\\ExplorersOfSaarum\\Action_0.bmp", false);
	Actions[ActionKind_Push] = LoadTexture("C:\\ExplorersOfSaarum\\Action_1.bmp", false);
	Actions[ActionKind_Turn] = LoadTexture("C:\\ExplorersOfSaarum\\Action_2.bmp", false);
	Actions[ActionKind_OpenInventory] = LoadTexture("C:\\ExplorersOfSaarum\\Bag.bmp", false);

	RenderShader = LoadShader("C:\\ExplorersOfSaarum\\EoS_Render.vsh", "C:\\ExplorersOfSaarum\\EoS_Render.fsh");
	LightShader = LoadShader("C:\\ExplorersOfSaarum\\EoS_Light.vsh", "C:\\ExplorersOfSaarum\\EoS_Light.fsh");
	HorizontalShader = LoadShader("C:\\ExplorersOfSaarum\\EoS_Render.vsh", "C:\\ExplorersOfSaarum\\EoS_HorizontalBlur.fsh");
	VerticalShader = LoadShader("C:\\ExplorersOfSaarum\\EoS_Render.vsh", "C:\\ExplorersOfSaarum\\EoS_VerticalBlur.fsh");
	Font = LoadTexture("C:\\ExplorersOfSaarum\\FontRender4.bmp", true);
	ButtonTexture = LoadTexture("C:\\ExplorersOfSaarum\\Button.bmp", false);
	PaperTexture = LoadTexture("C:\\ExplorersOfSaarum\\Paper.bmp", false);
	bool error = FT_Init_FreeType(&library);
	if (error)
	{
		MessageBox(NULL, "Font", "Error", MB_OK);
	}
	error = FT_New_Face(library, "C:\\ExplorersOfSaarum\\arial.ttf", 0, &face);
	if (error == FT_Err_Unknown_File_Format)
	{
		MessageBox(NULL, "The font file could be opened and read", "Error", MB_OK);
	}
	else if (error)
	{
		MessageBox(NULL, "The font file broken", "Error", MB_OK);
	}
	error = FT_Set_Char_Size(face,0 ,32 * 64, 300, 300);
	if (error)
	{
		MessageBox(NULL, "Font", "Error", MB_OK);
	}
	slot = face->glyph;
	error = FT_Load_Char(face, 1070, FT_LOAD_RENDER);
	if (error)
	{
		MessageBox(NULL, "Font", "Error", MB_OK);
	}
	Sprites[17] = BindTexture(&slot->bitmap, true);
	Scissors.push_front(new GLint[4]{0, 0, 1024, 1024});
	glGenFramebuffersEXT(1, &FBO);
}

void GraphicalController::DrawSprite(double x0,double y0,double x1,double y1,double x2,double y2,double x3,double y3)
	{
		glBegin(GL_QUADS);
		glTexCoord2d(0,1); glVertex2d(x0, y0);
		glTexCoord2d(0,0); glVertex2d(x1, y1);
		glTexCoord2d(1,0); glVertex2d(x2, y2);
		glTexCoord2d(1,1); glVertex2d(x3, y3);
		glEnd();
	}

void GraphicalController::DrawSpriteFBO(double TexWidth, double TexHeight, double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3)
{
	glBegin(GL_QUADS);
	glTexCoord2d(0, TexHeight); glVertex2d(x0, y0);
	glTexCoord2d(0, 0); glVertex2d(x1, y1);
	glTexCoord2d(TexWidth, 0); glVertex2d(x2, y2);
	glTexCoord2d(TexWidth, TexHeight); glVertex2d(x3, y3);
	glEnd();
}

void GraphicalController::CenterText(int x,int y,std::string Text,int sizex,int sizey)
{
	int cx=x-(Text.length())*(sizex+1)/2;
	int cy=y-sizey/2;
	STextXY(cx,cy,Text,sizex,sizey);
}

void GraphicalController::setVSync(bool sync)
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

void GraphicalController::STextXY(int x, int y, std::string& Text, int sizex, int sizey)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgramObjectARB(0);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, Font);
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

GLuint GraphicalController::BindTexture(FT_Bitmap* data, bool alpha)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	if (alpha)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
	else {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	////glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.biWidth, info.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA,data->width,data->rows, GL_RGBA, GL_UNSIGNED_BYTE, data->buffer);
	//for (int i = 0; i < data->rows*data->width*4; i++)
	//{
	//	if (data->buffer[i] != 0)
	//	{
	//		
	//	}
	//}
	//glTexImage2D(GL_TEXTURE_2D,0,GL_ALPHA,data->width,data->rows,0,GL_ALPHA,GL_UNSIGNED_BYTE,data->buffer);
	//char buf2[32];
	//itoa(data->width, buf2, 10);
	//MessageBox(0, buf2, "X", MB_OK);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_ALPHA, data->width, data->rows, GL_ALPHA, GL_UNSIGNED_BYTE, data->buffer);
	return texture;
}

bool GraphicalController::AddScissor(GLint* rect)
{
	GLint* GlobalScissor = Scissors.front();
	GLint* LocalScissor = new GLint[4]{ rect[0], 1024 - rect[1] - rect[3], rect[0] + rect[2], 1024 - rect[1] };
	if ((GlobalScissor[0] >LocalScissor[2]) || (GlobalScissor[2]<LocalScissor[0]) || (GlobalScissor[1]>LocalScissor[3]) || (GlobalScissor[3]<LocalScissor[1]))
	{
		return false;
	}
	if (LocalScissor[0]<GlobalScissor[0])
	{
		LocalScissor[0]=GlobalScissor[0];
	}
	if (LocalScissor[2]>GlobalScissor[2])
	{
		LocalScissor[2] = GlobalScissor[2];
	}
	if (LocalScissor[1]<GlobalScissor[1])
	{
		LocalScissor[1] = GlobalScissor[1];
	}
	if (LocalScissor[3]>GlobalScissor[3])
	{
		LocalScissor[3]=GlobalScissor[3];
	}
	Scissors.push_front(LocalScissor);
	glScissor(LocalScissor[0], LocalScissor[1], LocalScissor[2] - LocalScissor[0], LocalScissor[3] - LocalScissor[1]);
	return true;
}

void GraphicalController::RemoveScissor()
{
	Scissors.pop_front();
	GLint* Scissor = Scissors.front();
	glScissor(Scissor[0], Scissor[1], Scissor[2] - Scissor[0], Scissor[3] - Scissor[1]);
}

void GraphicalController::BlurRect(int x, int y, int width, int height)
{
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Sprites[19]);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, x, 1024 - y - height, width, height);
	double Tx = width / 1024.0;
	double Ty = height / 1024.0;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Sprites[20], 0);
	glUseProgramObjectARB(HorizontalShader);
	setUniformSampler(HorizontalShader, "Map");
	DrawSpriteFBO(Tx, Ty, 0, 1024 - height, 0, 1024, width, 1024, width, 1024 - height);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Sprites[19], 0);
	glUseProgramObjectARB(VerticalShader);
	setUniformSampler(VerticalShader, "Map");
	glBindTexture(GL_TEXTURE_2D, Sprites[20]);
	DrawSpriteFBO(Tx, Ty, 0, 1024 - height, 0, 1024, width, 1024, width, 1024 - height);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glUseProgramObjectARB(0);
	glEnable(GL_SCISSOR_TEST);
	glBindTexture(GL_TEXTURE_2D, Sprites[19]);
	DrawSpriteFBO(Tx, Ty, x, y, x, y + height, x + width, y + height, x + width, y);
}

GLcharARB* GraphicalController::LoadShaderSource(char* filename)
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

GLhandleARB GraphicalController::LoadShader(char* vPath, char* fPath)
{
	const GLcharARB* VertexShaderSource = LoadShaderSource(vPath);
	const GLcharARB* FragmentShaderSource = LoadShaderSource(fPath);
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
	ShowError(VertexShader);
	glShaderSourceARB(FragmentShader, 1, &FragmentShaderSource, NULL);
	glCompileShaderARB(FragmentShader);
	glGetObjectParameterivARB(FragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &compileStatus);
	ShowError(FragmentShader);
	glAttachObjectARB(Program, VertexShader);
	glAttachObjectARB(Program, FragmentShader);
	glLinkProgramARB(Program);
	return Program;
}

bool GraphicalController::setUniformVector(GLhandleARB program, const char * name, const float * value)
{
	int loc = glGetUniformLocationARB(program, name);
	if (loc < 0)
		return false;
	glUniform4fvARB(loc, 1, value);
	return true;
}

bool GraphicalController::setUniformSampler(GLhandleARB object, const char * name)
{
	int loc = glGetUniformLocationARB(object, name);
	if (loc < 0)
		return false;
	glUniform1iARB(loc, 0);
	return true;
}

bool GraphicalController::setUniformPtr(GLhandleARB program, const char * name, const unsigned int value)
{
	int loc = glGetUniformLocationARB(program, name);
	if (loc < 0)
		return false;
	glUniform1uiv(loc, 1, &value);
	return true;
}

void GraphicalController::UnloadShaderSource(GLcharARB* ShaderSource)
{
	if (ShaderSource != nullptr)
	{
		delete[] ShaderSource;
		ShaderSource = nullptr;
	}
}

bool GraphicalController::checkOpenGLError()
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

void GraphicalController::ShowError(GLhandleARB object)
{
	GLint LogLen;
	GLcharARB * infoLog;
	int charsWritten = 0;
	checkOpenGLError();
	glGetObjectParameterivARB(object, GL_OBJECT_INFO_LOG_LENGTH_ARB, &LogLen);
	infoLog = new GLcharARB[LogLen];
	glGetInfoLogARB(object, LogLen, &charsWritten, infoLog);
	if (infoLog[0] != '\0'){ MessageBox(NULL, infoLog, "Error", MB_OK); }
}

Point GraphicalController::GetOGLPos(float x, float y)
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
	return Point(_Point.x, _Point.y);
}

GLuint GraphicalController::LoadTexture(const char * filename, bool alpha)
{
	GLuint texture;
	char* data;
	FILE* file;
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER info;
	file = fopen(filename, "rb");
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
	if (alpha)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
	else {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.biWidth, info.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, info.biWidth, info.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
	delete data;
	return texture;
}