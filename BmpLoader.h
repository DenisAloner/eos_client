
#include<stdio.h>
#include<Windows.h>
#include <gl/gl.h>

struct RGB
{
 char red,green,blue,alpha;
};

class BmpLoader 
{
   public:

	   int width,height;
	   GLuint* pixels;

   BmpLoader(const char* filename)
   {
	   BITMAPFILEHEADER header;
	   BITMAPINFOHEADER info;
	   FILE *file;
	   file=fopen(filename,"rb");
	   fread(&header,sizeof(header),1,file);
	   fread(&info,sizeof(info),1,file);
	   pixels=new GLuint[info.biHeight*info.biWidth];
	   fseek(file,header.bfOffBits,0);
	   for(int i=0;i<info.biHeight;i++)
		   for(int j=0;j<info.biWidth;j+=4)
		   {
			   RGB rbg;
			   fread(&rbg,sizeof(rbg),1,file);
			   pixels[(i*info.biWidth)+j]=rbg.blue;
			    pixels[(i*info.biWidth)+j+1]=rbg.green;
			    pixels[(i*info.biWidth)+j+2]=rbg.red;
			   pixels[(i*info.biWidth)+j+3]=rbg.alpha;
		   }
   }

   ~BmpLoader()
   {
	   delete[] pixels;
   }
   
 
  GLuint loadTexture()
  {
        GLuint textureId;
        glGenTextures( 1, &textureId );
        glBindTexture( GL_TEXTURE_2D, textureId);
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,width,height, 0,GL_RGBA , GL_UNSIGNED_BYTE, pixels);
		return textureId;
}

};




