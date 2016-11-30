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

namespace sims
{
	class LockedIndexBuffer
	{
	public:
		LockedIndexBuffer();
		LockedIndexBuffer(IndexBuffer* indexBuffer, uint32 lockFlags, uint32 offset, uint32 count);

		uint32 GetLockFlags() const { return lockFlags_; }
		uint32 GetOffset() const { return offset_; }
		uint32 GetCount() const { return count_; }

		uint16* GetData() { return indexData_->GetData(); }
		const uint16* GetData() const { return indexData_->GetData(); }

		void Init(IndexBuffer* indexBuffer, uint32 lockFlags, uint32 offset, uint32 count);
		void Clear();
	private:
		friend class IndexBuffer;

		IndexBuffer* indexBuffer_;
		uint32 offset_;
		uint32 count_;
		uint32 lockFlags_;
		TBuffer<uint16>* indexData_;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer();
		IndexBuffer(uint32 indexCount);
		IndexBuffer(uint32 indexCount, uint16* data);
		virtual ~IndexBuffer();

		bool Valid() const { return indexData_.GetData() != nullptr; }
		uint32 GetIndexCount() const { return indexCount_; }

		void SetStorageFlags(uint32 flags) { storageFlags_ = flags; }
		uint32 GetStorageFlags() const { return storageFlags_; }
	private:
		friend class LockedIndexBuffer;
		TBuffer<uint16>* GetIndexData() { return &indexData_; }
	protected:
		uint32 indexCount_;
		TBuffer<uint16> indexData_;
		uint32 storageFlags_;

		LockedIndexBuffer lockedIB_;
		int32 lockedCount_;
	};
}
