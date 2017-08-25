/********************************************************************
	created:	2017/08/25
	created:	25:8:2017   11:43
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_index_buffer.h
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_index_buffer
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	OGL Index Buffer
*********************************************************************/
#pragma once

#include "graphics/render_resource.h"
#include "ogl_common.h"

namespace sims
{
	namespace ogl
	{
		class OGLIndexBufferResource : public IndexBufferResource
		{
		public:
			OGLIndexBufferResource();
			~OGLIndexBufferResource();

			virtual void UpdateResource();
			virtual void BindResource() const;
			virtual void ReleaseResource();
		private:
			void InternalReleaseResource();

			GLuint ibo_;
		};
	}
}

