/********************************************************************
	created:	2018/01/08
	created:	8:1:2018   15:21
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_vertex_array.h
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_vertex_array
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	OGL Vertex Array
*********************************************************************/
#pragma once

#include "graphics/render_resource.h"
#include "ogl_common.h"

namespace sims
{
	namespace ogl
	{
		class OGLVertexArrayResource : public VertexArrayResource
		{
		public:
			OGLVertexArrayResource();
			~OGLVertexArrayResource();

			virtual void UpdateResource() override;
			virtual void BindResource() const override;
			virtual void ReleaseResource() override;
		private:
			void InternalReleaseResource();

			GLuint vao_;
		};
	}
}
