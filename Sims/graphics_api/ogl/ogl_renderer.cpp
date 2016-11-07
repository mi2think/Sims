/********************************************************************
	created:	2016/10/27
	created:	27:10:2016   16:16
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_renderer.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_renderer
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	OGL Renderer
*********************************************************************/
#include "ogl_renderer.h"
#include "ogl_common.h"
#include "graphics/texture.h"

namespace sims
{
	namespace ogl
	{
		void OGLRenderer::BeginFrame(uint32 clearFlags, Color color, float depth, uint32 stencil)
		{
			uint32 flags = 0;
			if ((clearFlags & ClearFlags::Color) != 0)
			{
				auto c = color.GetRGBAVector4();
				glClearColor(c.x, c.y, c.z, c.w);
				flags |= GL_COLOR_BUFFER_BIT;
			}
			if ((clearFlags & ClearFlags::Depth) != 0)
			{
				glClearDepth(depth);
				flags |= GL_DEPTH_BUFFER_BIT;
			}
			if ((clearFlags & ClearFlags::Stencil) != 0)
			{
				glClearStencil(stencil);
				flags |= GL_STENCIL_BUFFER_BIT;
			}
			glClear(flags);
		}

		void OGLRenderer::EndFrame()
		{
		}

		void OGLRenderer::PresentFrame()
		{
			glut_swap_buffers();
		}
	}
}