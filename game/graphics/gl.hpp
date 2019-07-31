#ifndef GL_HPP
#define GL_HPP


#if defined __DESKTOP__
	#ifdef _WIN32	// Windows
        #include <glbinding/gl/gl.h>
        #include <glbinding/glbinding.h>
	#else			// Linux
		#define GL_GLEXT_PROTOTYPES
		#include <GL/gl.h>
		#include <GL/glext.h>
	#endif
#else
	#error "Unknown platform"
#endif

#endif//GL_HPP
