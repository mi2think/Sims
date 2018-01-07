/********************************************************************
	created:	2016/11/28
	created:	28:11:2016   16:14
	filename: 	D:\Code\Sims\Sims\graphics\vertex_buffer.h
	file path:	D:\Code\Sims\Sims\graphics
	file base:	vertex_buffer
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:
		Vertex Buffer is buffer store vertex element, it's responsible for 
		vertex data update.

*********************************************************************/
#pragma once

#include "graphics_fwd.h"
#include "vertex_stream.h"
#include "vertex_declaration.h"
#include "render_resource.h"

namespace sims
{
	class LockedVertexBuffer
	{
	public:
		LockedVertexBuffer();
		LockedVertexBuffer(VertexBuffer* vertexBuffer, uint32 lockFlags, uint32 offset, uint32 count);
		
		void Init(VertexBuffer* vertexBuffer, uint32 lockFlags, uint32 offset, uint32 count);
		void Clear();

		uint32 GetLockFlags() const { return lockFlags_; }
		uint32 GetOffset() const { return offset_; }
		uint32 GetCount() const { return count_; }

		void* GetData() { return vertexData_->GetData(); }
		const void* GetData() const { return vertexData_->GetData(); }

		void* GetLockData();
		const void* GetLockData() const;
	private:
		friend class VertexBuffer;

		VertexBuffer* vertexBuffer_;
		uint32 offset_;
		uint32 count_;
		uint32 lockFlags_;
		Buffer* vertexData_;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer();
		VertexBuffer(const VertexDeclarationRef& vertexDecl, uint32 streamIndex);
		VertexBuffer(const VertexDeclarationRef& vertexDecl, uint32 vertexCount, uint32 streamIndex);
		~VertexBuffer();

		bool Valid() const { return vertexData_.GetData() != nullptr; }
		uint32 GetVertexCount() const { return vertexCount_; }
		const VertexStream* GetVertexStream() const { return vertexStream_; }
		void Resize(uint32 vertexCount);

		void SetStorageFlags(uint32 flags) { storageFlags_ = flags; }
		uint32 GetStorageFlags() const { return storageFlags_; }

		// count 0 for lock total buffer
		LockedVertexBuffer* Lock(uint32 lockFlags, uint32 offset = 0, uint32 count = 0);
		void Unlock(LockedVertexBuffer* L);

		const IndexRange& GetInvalidRange() const { return invalidRange_; }

		// propagates changes on the vertex buffer to the renderer.
		//   you must call invalidate after modifying vertex buffer data,
		//   for it to be uploaded to GPU.
		void Invalidate();

		// Hardware resource
		VertexBufferResourceRef& HWResource() { return HWResource_; }
	private:
		friend class LockedVertexBuffer;
		Buffer* GetVertexData() { return &vertexData_; }
	private:
		VertexDeclarationRef vertexDecl_;
		const VertexStream* vertexStream_;
		uint32 vertexCount_;
		uint32 streamIndex_;

		Buffer vertexData_;
		uint32 storageFlags_;
		IndexRange invalidRange_;
		LockedVertexBuffer lockedVB_;
		bool isLocked_;

		VertexBufferResourceRef HWResource_;
	};
}

