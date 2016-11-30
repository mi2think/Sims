/********************************************************************
	created:	2016/11/28
	created:	28:11:2016   16:14
	filename: 	D:\Code\Sims\Sims\graphics\vertex_buffer.h
	file path:	D:\Code\Sims\Sims\graphics
	file base:	vertex_buffer
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Vertex Buffer
*********************************************************************/
#pragma once

#include "graphics_fwd.h"
#include "vertex_stream.h"
#include "vertex_declaration.h"

namespace sims
{
	class LockedVertexBuffer
	{
	public:
		LockedVertexBuffer();
		LockedVertexBuffer(VertexBuffer* vertexBuffer, uint32 lockFlags, uint32 offset, uint32 count);
		
		uint32 GetLockFlags() const { return lockFlags_; }
		uint32 GetOffset() const { return offset_; }
		uint32 GetCount() const { return count_; }

		const VertexDeclarationRef& GetVertexDeclaration() const;
		void* GetData();
		const void* GetData() const;

		void Init(VertexBuffer* vertexBuffer, uint32 lockFlags, uint32 offset, uint32 count);
		void Clear();
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
		enum VertexBufferLockFlags
		{
			LockRead = BIT(0),
			LockWrite = BIT(1),
			LockRW = LockRead | LockWrite
		};

		VertexBuffer();
		VertexBuffer(const VertexDeclarationRef& vertexDecl, uint32 vertexCount);
		virtual ~VertexBuffer();

		bool Valid() const { return vertexData_.GetData() != nullptr; }
		const VertexDeclarationRef& GetVertexDeclaration() const { return vertexDecl_; }
		int GetVertexCount() const { return vertexCount_; }

		void SetStorageFlags(uint32 flags) { storageFlags_ = flags; }
		uint32 GetStorageFlags() const { return storageFlags_; }

		// count 0 for lock total buffer
		LockedVertexBuffer* Lock(uint32 lockFlags, uint32 offset = 0, uint32 count = 0);
		void Unlock(LockedVertexBuffer* L);

		// propagates changes on the vertex buffer to the renderer.
		//   you must call invalidate after modifying vertex buffer data,
		//   for it to be uploaded to GPU.
		void Invalidate();

		virtual void HWUpdateVertexBuffer() {}
		virtual void HWBindVertexBuffer() {}
		virtual void HWDeleteVertexBuffer() {}
	private:
		friend class LockedVertexBuffer;
		Buffer* GetVertexData() { return &vertexData_; }
	protected:
		VertexDeclarationRef vertexDecl_;
		uint32 vertexCount_;
		Buffer vertexData_;
		uint32 storageFlags_;

		LockedVertexBuffer lockedVB_;
		int32 lockedCount_;
	};
}

