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
			const IndexRange& range = vertexBuffer_->GetInvalidRange();
			auto L = vertexBuffer_->Lock(LockFlags::LockRead, range.begin, range.count);
			uint32 dataSize = L->GetCount() * vertexDecl_->GetStride();
			uint32 dataOffset = L->GetOffset() * vertexDecl_->GetStride();

			if (vbo_ == 0)
			{
				ASSERT(dataOffset == 0);
				glGenBuffers(1, &vbo_);
				gl_check_error("OGLVertexBufferResource::UpdateResource - glGenBuffers");

				GLenum usage = GL_STATIC_DRAW;
				if ((vertexBuffer_->GetStorageFlags() & StorageFlags::HintDynamic) != 0)
					usage = GL_DYNAMIC_DRAW;

				glBindBuffer(GL_ARRAY_BUFFER, vbo_);
				
				glBufferData(GL_ARRAY_BUFFER, dataSize, L->GetLockData(), usage);
				gl_check_error("OGLVertexBufferResource::UpdateResource - glBufferData");
				
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			else
			{
				glBindBuffer(GL_ARRAY_BUFFER, vbo_);

				glBufferSubData(GL_ARRAY_BUFFER, dataOffset, dataSize, L->GetLockData());
				gl_check_error("OGLVertexBufferResource::UpdateResource - glBufferSubData");
				
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			vertexBuffer_->Unlock(L);
		}

		void OGLVertexBufferResource::BindResource() const
		{
			ASSERT(vbo_ != 0);

			glBindBuffer(GL_ARRAY_BUFFER, vbo_);

			// bind stream
			uint32 streamCount = vertexDecl_->GetStreamCount();
			for (uint32 i = 0; i < streamCount; ++i)
			{
				const VertexStream* vertexStream = vertexDecl_->GetStream(i);

				glEnableVertexAttribArray(i);
				gl_check_error("glEnableVertexAttribArray");

				glVertexAttribPointer(i,
					vertexStream->GetElementCount(),
					ToGLElementType(vertexStream->GetElementType()),
					GL_FALSE,
					vertexDecl_->GetStride(),
					(const void*)vertexStream->GetOffset());
				gl_check_error("glVertexAttribPointer");
			}
		}

		void OGLVertexBufferResource::ReleaseResource()
		{
			InternalReleaseResource();
		}

		void OGLVertexBufferResource::InternalReleaseResource()
		{
			if (vbo_ != 0)
			{
				glDeleteBuffers(1, &vbo_);
				vbo_ = 0;
			}
		}
	}
}
