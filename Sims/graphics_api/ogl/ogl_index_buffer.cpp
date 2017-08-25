/********************************************************************
	created:	2017/08/25
	created:	25:8:2017   11:45
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_index_buffer.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_index_buffer
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	OGL Index Buffer
*********************************************************************/
#include "ogl_index_buffer.h"
#include "graphics/index_buffer.h"

namespace sims
{
	namespace ogl
	{
		OGLIndexBufferResource::OGLIndexBufferResource()
			: IndexBufferResource()
			, ibo_(0)
		{}

		OGLIndexBufferResource::~OGLIndexBufferResource()
		{
			InternalReleaseResource();
		}

		void OGLIndexBufferResource::UpdateResource()
		{
			const IndexRange& range = indexBuffer_->GetInvalidRange();
			auto L = indexBuffer_->Lock(LockFlags::LockRead, range.begin, range.count);
			uint32 dataSize = L->GetCount() * sizeof(IndexType);
			uint32 dataOffset = L->GetOffset() * sizeof(IndexType);

			if (ibo_ == 0)
			{
				ASSERT(dataOffset == 0);
				glGenBuffers(1, &ibo_);
				gl_check_error("OGLIndexBufferResource::UpdateResource - glGenBuffers");

				GLenum usage = GL_STATIC_DRAW;
				if ((indexBuffer_->GetStorageFlags() & StorageFlags::HintDynamic) != 0)
					usage = GL_DYNAMIC_DRAW;

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, L->GetLockData(), usage);
				gl_check_error("OGLIndexBufferResource::UpdateResource - glBufferData");
			}
			else
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, dataOffset, dataSize, L->GetLockData());
				gl_check_error("OGLIndexBufferResource::UpdateResource - glBufferSubData");
			}

			indexBuffer_->Unlock(L);
		}

		void OGLIndexBufferResource::BindResource() const
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
		}

		void OGLIndexBufferResource::InternalReleaseResource()
		{
			if (ibo_ != 0)
			{
				glDeleteBuffers(1, &ibo_);
				ibo_ = 0;
			}
		}
	}
}
