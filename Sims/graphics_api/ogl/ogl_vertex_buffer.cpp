/********************************************************************
	created:	2017/08/17
	created:	17:8:2017   17:01
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_vertex_buffer.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_vertex_buffer
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	OGL Vertex Buffer
*********************************************************************/
#include "ogl_vertex_buffer.h"
#include "graphics/vertex_buffer.h"

namespace sims
{
	namespace ogl
	{
		OGLVertexBufferResource::OGLVertexBufferResource()
			: VertexBufferResource()
			, vbo_(0)
		{}

		OGLVertexBufferResource::~OGLVertexBufferResource()
		{
			InternalReleaseResource();
		}

		void OGLVertexBufferResource::UpdateResource()
		{
			if (vbo_ == 0)
			{
				glGenBuffers(1, &vbo_);
				gl_check_error("OGLVertexBufferResource::UpdateResource - glGenBuffers");
			}
		}
	}
}
