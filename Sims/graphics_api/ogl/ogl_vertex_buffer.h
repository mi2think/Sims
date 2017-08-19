/********************************************************************
	created:	2017/08/17
	created:	17:8:2017   16:47
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_vertex_buffer.h
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_vertex_buffer
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	OGL Vertex Buffer
*********************************************************************/
#pragma once

#include "graphics/render_resource.h"
#include "ogl_common.h"

namespace sims
{
	namespace ogl
	{
		class OGLVertexBufferResource : public VertexBufferResource
		{
		public:
			OGLVertexBufferResource();
			~OGLVertexBufferResource();

			virtual void UpdateResource();
			virtual void BindResource() const;
			virtual void ReleaseResource();
		private:
			void InternalReleaseResource();

			GLuint vbo_;
		};
	}
}