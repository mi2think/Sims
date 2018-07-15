/********************************************************************
	created:	2018/01/08
	created:	8:1:2018   15:23
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_vertex_array.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_vertex_array
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	OGL Vertex Array
*********************************************************************/
#include "ogl_vertex_array.h"
#include "graphics/vertex_array.h"

namespace sims
{
	namespace ogl
	{
		OGLVertexArrayResource::OGLVertexArrayResource()
			: VertexArrayResource()
			, vao_(0)
		{
		}

		OGLVertexArrayResource::~OGLVertexArrayResource()
		{
			InternalReleaseResource();
		}

		void OGLVertexArrayResource::UpdateResource()
		{
			if (OGLCapsInfo::SupportVertexArrayObjects())
			{
				if (vao_ == 0)
				{
					glGenVertexArrays(1, &vao_);
					glBindVertexArray(vao_);

					// configure attributes in VAO
					auto count = vertexArray_->GetVertexBufferCount();
					for (uint32 i = 0; i < count; ++i)
					{
						auto vertexBuffer = vertexArray_->GetVertexBuffer(i);
						vertexBuffer->Invalidate();
						vertexBuffer->Bind();
					}

					gl_check_error("OGLVertexArrayResource::UpdateResource");
				}
			}
			// fall back
			else
			{
				auto count = vertexArray_->GetVertexBufferCount();
				for (uint32 i = 0; i < count; ++i)
				{
					auto vertexBuffer = vertexArray_->GetVertexBuffer(i);
					vertexBuffer->Invalidate();
				}
			}
		}

		void OGLVertexArrayResource::BindResource() const
		{
			if (OGLCapsInfo::SupportVertexArrayObjects())
			{
				ASSERT(vao_ != 0);
				glBindVertexArray(vao_);

				gl_check_error("OGLVertexArrayResource::BindResource");
			}
			// fall back
			else
			{
				auto count = vertexArray_->GetVertexBufferCount();
				for (uint32 i = 0; i < count; ++i)
				{
					auto vertexBuffer = vertexArray_->GetVertexBuffer(i);
					vertexBuffer->Bind();
				}
			}
		}

		void OGLVertexArrayResource::ReleaseResource()
		{
			InternalReleaseResource();
		}

		void OGLVertexArrayResource::InternalReleaseResource()
		{
			if (vao_ != 0)
			{
				glDeleteBuffers(1, &vao_);
				vao_ = 0;
			}
		}
	}
}
