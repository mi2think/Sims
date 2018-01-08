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
#include "ogl_glut_callbacks.h"
#include "ogl_renderer_caps.h"

#include <glew.h>
#include <freeglut.h>

#ifdef UpdateResource
	#undef UpdateResource
#endif

namespace sims
{
	namespace ogl
	{
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
#ifdef SIMS_DEBUG
		GLenum gl_check_error(const char* category = nullptr);
#else
		inline GLenum gl_check_error(const char* category = nullptr) { return GL_NO_ERROR; }
#endif 

		GLenum ToGLTextureFilterType(TextureFilter::Type filter);
		GLenum ToGLTextureWrapType(TextureWrap::Type wrap);
		GLenum ToGLElementType(DataType::Type type);
		GLenum ToGLPrimitiveType(PrimitiveType::Type type);

		template<typename T>
		void AllocIndicesByRange(vector<T>& indices, const IndexRange& range)
		{
			ASSERT(!range.IsEmpty());
			indices.resize(range.count);
			for (int i = 0; i < (int)indices.size(); ++i)
			{
				indices[i] = T(range.begin + i);
			}
		}
	}
}

#pragma warning(pop)

#endif
