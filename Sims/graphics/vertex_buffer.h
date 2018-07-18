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
		uint32 GetStride() const;

		void* GetData();
		const void* GetData() const;

		// may be null ptr if can not find specify usage
		void* GetDataByUsage(VertexElementUsage::Type usage, uint32 usageIndex = 0);
		const void* GetDataByUsage(VertexElementUsage::Type usage, uint32 usageIndex = 0) const;
	private:
		friend class VertexStream;

		const VertexStream* vertexStream_;
		uint32 offset_;
		uint32 count_;
		uint32 lockFlags_;
		char*  data_;
	};

	class VertexBuffer : public IResourceOperation
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

		// count 0 for lock total buffer
		LockedVertexBuffer* Lock(uint32 lockFlags, uint32 offset = 0, uint32 count = 0);
		void Unlock(LockedVertexBuffer* L);

		const IndexRange& GetInvalidRange() const { return invalidRange_; }

		// ~ IResourceOperation
		virtual void Invalidate() override;
	protected:
		virtual void Create();
		// ~ IResourceOperation

		friend class LockedVertexBuffer;
		Buffer* GetVertexData() { return &vertexData_; }
	private:
		VertexDeclarationRef vertexDecl_;
		const VertexStream* vertexStream_;
		uint32 vertexCount_;

		Buffer vertexData_;

		IndexRange invalidRange_;
		LockedVertexBuffer lockedVB_;
		bool isLocked_;
	};
}

