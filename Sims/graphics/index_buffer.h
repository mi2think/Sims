/********************************************************************
	created:	2016/11/30
	created:	30:11:2016   16:30
	filename: 	D:\Code\Sims\Sims\graphics\index_buffer.h
	file path:	D:\Code\Sims\Sims\graphics
	file base:	index_buffer
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Index Buffer
*********************************************************************/
#pragma once

#include "graphics_fwd.h"
#include "render_resource.h"

namespace sims
{
	typedef uint16 IndexType;

	class LockedIndexBuffer
	{
	public:
		LockedIndexBuffer();
		LockedIndexBuffer(IndexBuffer* indexBuffer, uint32 lockFlags, uint32 offset, uint32 count);

		uint32 GetLockFlags() const { return lockFlags_; }
		uint32 GetOffset() const { return offset_; }
		uint32 GetCount() const { return count_; }

		IndexType* GetData();
		const IndexType* GetData() const;

		void Init(IndexBuffer* indexBuffer, uint32 lockFlags, uint32 offset, uint32 count);
		void Clear();
	private:
		friend class IndexBuffer;

		IndexBuffer* indexBuffer_;
		uint32 offset_;
		uint32 count_;
		uint32 lockFlags_;
		TBuffer<IndexType>* indexData_;
	};

	class IndexBuffer : public IResourceOperation
	{
	public:
		IndexBuffer();
		IndexBuffer(uint32 indexCount);
		IndexBuffer(uint32 indexCount, IndexType* data);
		~IndexBuffer();

		bool Valid() const { return indexData_.GetData() != nullptr; }
		uint32 GetIndexCount() const { return indexCount_; }
		void Resize(uint32 indexCount);

		// count 0 for lock total buffer
		LockedIndexBuffer* Lock(uint32 lockFlags, uint32 offset = 0, uint32 count = 0);
		void Unlock(LockedIndexBuffer* L);

		const IndexRange& GetInvalidRange() const { return invalidRange_; }

		// ~ IResourceOperation
		virtual void Invalidate() override;
	protected:
		virtual void Create();
		// ~ IResourceOperation

		friend class LockedIndexBuffer;
		TBuffer<IndexType>* GetIndexData() { return &indexData_; }
	private:
		uint32 indexCount_;
		TBuffer<IndexType> indexData_;
		IndexRange invalidRange_;

		LockedIndexBuffer lockedIB_;
		bool isLocked_;
	};
}
