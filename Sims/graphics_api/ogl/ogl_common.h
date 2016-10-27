/********************************************************************
	created:	2016/10/12
	created:	12:10:2016   16:56
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_common.h
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_common
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	OGL
*********************************************************************/
#ifndef __COMMON_OGL_H__
#define __COMMON_OGL_H__

#pragma warning(push)
#pragma warning( disable : 4505 )  // unreferenced local function has been removed in freeglut_std.h

#define FREEGLUT_STATIC // defined so you can link to freeglut_static.lib when compiling
#define GLEW_STATIC     // defined so you can link to glew's static .lib when compiling

#include "graphics/graphics_fwd.h"

#include <glew.h>
#include <freeglut.h>

namespace sims
{
	namespace ogl
	{
		// glut call backs
		class GLUT_Callbacks
		{
		public:
			virtual ~GLUT_Callbacks() {}

			virtual void KeyboardFunc(unsigned char key, int x, int y) = 0;
			virtual void KeyboardSpecialFunc(int key, int x, int y) = 0;
			virtual void KeyboardUpFunc(unsigned char key, int x, int y) = 0;

			virtual void MouseFunc(int button, int state, int x, int y) = 0;
			virtual void MotionFunc(int x, int y) = 0;
			virtual void PassiveMotionFunc(int x, int y) = 0;

			virtual void DisplayFunc() = 0;
			virtual void IdleFunc() = 0;
		};

		extern GLUT_Callbacks* g_callback;

		// init glut and display mode with:
		//     depth,
		//     stencil,
		//     double buffer,
		//     rgba
		void glut_init();

		// create window and center it
		//     60hz if full screen
		void glut_create_window(uint32 width, uint32 height, const char* title, bool fullscreen);

		// bind glut call back to g_callback
		void glut_bind_callbacks();

		// enter main loop
		void glut_main_loop();

		void glut_swap_buffers();

		// init gl states:
		//    clear color, depth & stencil
		//    depth test: GL_LESS
		//    cull face: GL_BACK
		void gl_init_states(float r, float g, float b, float a);

		// check error
		GLenum gl_check_error(const char* category = nullptr);


		GLenum ToGLTextureFilterType(TextureFilter::Type filter);
		GLenum ToGLTextureWrapType(TextureWrap::Type wrap);
	}
}

#pragma warning(pop)

#endif
