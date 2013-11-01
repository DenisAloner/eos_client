#pragma once
#include "glew.h"
#include <iostream>
#include <fstream>
#include <Windows.h>

struct TPoint
{
	GLdouble x,y,z;
};

class TGraph
{
public:

	GLhandleARB RenderShader;
	GLhandleARB LightShader;
	GLuint ButtonTexture;

	GLuint Font;

	TGraph()
	{
		RenderShader=LoadShader("C:\\ExplorersOfSaarum\\EoS_Render.vsh","C:\\ExplorersOfSaarum\\EoS_Render.fsh");
		LightShader=LoadShader("C:\\ExplorersOfSaarum\\EoS_Light.vsh","C:\\ExplorersOfSaarum\\EoS_Light.fsh");
		Font=LoadTexture("C:\\ExplorersOfSaarum\\FontTile.bmp",true);
		ButtonTexture=LoadTexture("C:\\ExplorersOfSaarum\\Button.bmp",false);
	}

 GLcharARB* LoadShaderSource(char* filename)
{
	GLcharARB* ShaderSource;
	std::ifstream ShaderFile (filename, std::ifstream::binary);
	if (ShaderFile) 
	{
		ShaderFile.seekg (0, ShaderFile.end);
		int length = ShaderFile.tellg();
		ShaderFile.seekg (0, ShaderFile.beg);
		ShaderSource= new GLcharARB[length+1];
		ShaderFile.read (ShaderSource,length);
		ShaderSource[length] = '\0';
		ShaderFile.close();
	} else {
		return nullptr;
	}
	return ShaderSource;
}


void UnloadShaderSource(GLcharARB* ShaderSource)
{
	if (ShaderSource !=nullptr)
   {
	   delete[] ShaderSource;
	   ShaderSource = nullptr;
   }
}

bool    checkOpenGLError ()
{
    bool    retCode = true;

    for ( ; ; )
    {
        GLenum  glErr = glGetError ();

        if ( glErr == GL_NO_ERROR )
            return retCode;

        printf ( "glError: %s\n", gluErrorString ( glErr ) );
		MessageBox(NULL,"glError: %s\n","1",MB_OK);
        retCode = false;
        glErr   = glGetError ();
    }

    return retCode;
}

void ShowError(GLhandleARB object)
{
	GLint LogLen;
	GLcharARB * infoLog;
	int charsWritten = 0;
	checkOpenGLError(); 
	glGetObjectParameterivARB (object, GL_OBJECT_INFO_LOG_LENGTH_ARB, &LogLen);
	infoLog=new GLcharARB[LogLen];
    glGetInfoLogARB (object, LogLen, &charsWritten, infoLog );
    MessageBox(NULL,infoLog,"Error",MB_OK);
}


GLhandleARB LoadShader(char* vPath, char* fPath)
{
	const GLcharARB* VertexShaderSource=LoadShaderSource(vPath);
	const GLcharARB* FragmentShaderSource=LoadShaderSource(fPath);
	GLhandleARB Program;
	GLhandleARB VertexShader;
	GLhandleARB FragmentShader;
	Program = glCreateProgramObjectARB();
	VertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	FragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	glShaderSourceARB(VertexShader, 1, &VertexShaderSource, NULL);
	glCompileShaderARB(VertexShader);
	GLint compileStatus;
	glGetObjectParameterivARB (VertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &compileStatus );
	ShowError(VertexShader);
	glShaderSourceARB(FragmentShader, 1, &FragmentShaderSource, NULL);
	glCompileShaderARB(FragmentShader);
	glGetObjectParameterivARB (FragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &compileStatus );
	ShowError(FragmentShader);
	glAttachObjectARB(Program, VertexShader);
	glAttachObjectARB(Program, FragmentShader);
	glLinkProgramARB(Program);
	return Program;
}

bool setUniformVector ( GLhandleARB program, const char * name, const float * value )
{
    int loc = glGetUniformLocationARB ( program, name );
    if ( loc < 0 )
        return false;
    glUniform4fvARB ( loc, 1, value );
    return true;
}

bool setUniformSampler ( GLhandleARB object,const char * name )
{
    int loc = glGetUniformLocationARB ( object, name );
    if ( loc < 0 )
        return false;
    glUniform1iARB ( loc,0);
    return true;
}

bool setUniformPtr ( GLhandleARB program, const char * name, const unsigned int value )
{
    int loc = glGetUniformLocationARB ( program, name );
    if ( loc < 0 )
    return false;
	glUniform1uiv(loc,1,&value);
    return true;
}

TPoint GetOGLPos(float x, float y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
	TPoint Point;
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &Point.x, &Point.y, &Point.z);
    return Point;
}

 TPoint GetMouseCoordinat(HWND hWnd)
 {
	 POINT mouse;
	 GetCursorPos(&mouse); 
	 ScreenToClient(hWnd, &mouse);
	 GLfloat winX, winY, winZ;
	 winX = (float)mouse.x;
	 winY = (float)mouse.y;
	 return GetOGLPos(winX,winY);
 }

 GLuint LoadTexture( const char * filename, bool alpha) 
    {
		GLuint texture;
        char* data;
        FILE* file;
		BITMAPFILEHEADER header;
		BITMAPINFOHEADER info;
		file=fopen(filename,"rb");
		fread(&header,sizeof(header),1,file);
		fread(&info,sizeof(info),1,file);
        data = new char[info.biWidth * info.biHeight*4];
        fread( data, info.biWidth * info.biHeight * sizeof(GLuint), 1, file ); 
        fclose(file);
		for(GLuint i=0;i<info.biWidth * info.biHeight*4;i+=4)
		{
	       int r,g,b,a;
		   b=data[i];
		   g=data[i+1];
		   r=data[i+2];
		   a=data[i+3];
		   data[i]=r;
		   data[i+1]=g;
		   data[i+2]=b;
		   data[i+3]=a;	
		}
        glGenTextures( 1, &texture );
        glBindTexture( GL_TEXTURE_2D, texture);
        //glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		if(alpha)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  	GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  	GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		} else {
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  	GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  	GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
	   
      

  
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.biWidth, info.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        delete data;
        return texture; 
    }

  void TextXY(int x,int y,char* Text,int size)
  {
	  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	  glUseProgramObjectARB(0);
	  glActiveTextureARB(GL_TEXTURE0_ARB);
	  glBindTexture(GL_TEXTURE_2D, Font);
	  int x0,y0,x1,y1,x2,y2,x3,y3;
	  float xt0,yt0,xt1,yt1,xt2,yt2,xt3,yt3;
	  int i,j;
	  unsigned int ASCII;
	  for(int k=0;k<strlen(Text);k++)
	  {
	  ASCII=(unsigned int)Text[k];
	  j=ASCII/16;
	  i=ASCII-j*16;
	  j=15-j;
	  x0=x+k*(size+1);
	  y0=y;
	  x1=x+k*(size+1);
	  y1=y+size;
	  x2=x+size+k*(size+1);
	  y2=y+size;
	  x3=x+size+k*(size+1);
	  y3=y;
	  xt0=i*0.0625;
	  yt0=(j+1)*0.0625;
	  xt1=i*0.0625;
	  yt1=j*0.0625;
	  xt2=(i+1)*0.0625;
	  yt2=j*0.0625;
	  xt3=(i+1)*0.0625;
	  yt3=(j+1)*0.0625;
	  glBegin(GL_QUADS);
	  glTexCoord2d(xt0,yt0); glVertex2d(x0, y0);
	  glTexCoord2d(xt1,yt1); glVertex2d(x1, y1);
	  glTexCoord2d(xt2,yt2); glVertex2d(x2, y2);
	  glTexCoord2d(xt3,yt3); glVertex2d(x3, y3);
	  glEnd();
	  }
  }

  void DrawSprite(double x0,double y0,double x1,double y1,double x2,double y2,double x3,double y3);
  
  void CenterText(int x,int y,char* Text,int size);

};

