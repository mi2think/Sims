/********************************************************************
	created:	2016/11/30
	created:	30:11:2016   16:36
	filename: 	D:\Code\Sims\Sims\graphics\index_buffer.cpp
	file path:	D:\Code\Sims\Sims\graphics
	file base:	index_buffer
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Index Buffer
*********************************************************************/
#include "index_buffer.h"
#include "graphics_api/sims_sdk_hw.h"

namespace sims
{
	LockedIndexBuffer::LockedIndexBuffer()
	{
		Clear();
	}

	LockedIndexBuffer::LockedIndexBuffer(IndexBuffer* indexBuffer, uint32 lockFlags, uint32 offset, uint32 count)
	{
		Init(indexBuffer, lockFlags, offset, count);
	}

	void LockedIndexBuffer::Init(IndexBuffer* indexBuffer, uint32 lockFlags, uint32 offset, uint32 count)
	{
		indexBuffer_ = indexBuffer;
		lockFlags_ = lockFlags;
		offset_ = offset;
		count_ = count;
		indexData_ = indexBuffer->GetIndexData();
	}

	void LockedIndexBuffer::Clear()
	{
		indexBuffer_ = nullptr;
		lockFlags_ = LockFlags::LockRead;
		offset_ = 0;
		count_ = 0;
		indexData_ = nullptr;
	}

	IndexBuffer::IndexBuffer()
		: indexCount_(0)
		, storageFlags_(StorageFlags::Local)
		, lockedCount_(0)
	{
	}

	IndexBuffer::IndexBuffer(uint32 indexCount)
		: indexCount_(indexCount)
		, storageFlags_(StorageFlags::Local)
		, lockedCount_(0)
	{
		indexData_.Resize(indexCount);
	}

	IndexBuffer::IndexBuffer(uint32 indexCount, IndexType* data)
		: indexCount_(indexCount)
		, storageFlags_(StorageFlags::Local)
		, lockedCount_(0)
	{
		ASSERT(indexCount > 0);

		indexData_.Resize(indexCount);
		memcpy(indexData_.GetData(), data, sizeof(IndexType) * indexCount);
	}

	IndexBuffer::~IndexBuffer()
	{
	}

	void IndexBuffer::Resize(uint32 indexCount)
	{
		ASSERT(lockedCount_ == 0);
		indexData_.Resize(indexCount);
	}

	LockedIndexBuffer* IndexBuffer::Lock(uint32 lockFlags, uint32 offset, uint32 count)
	{
		if (lockedCount_ > 0)
		{
			// read is ok if pre-lock is read
			if ((lockFlags & LockFlags::LockWrite) != 0 ||
				(lockedIB_.lockFlags_ & LockFlags::LockWrite) != 0)
			{
				ASSERT(false && "lock locked index buffer");
				return nullptr;
			}
		}

		if (count == 0)
			count = indexCount_ - offset;
		if (offset + count > indexCount_)
		{
			ASSERT(false && "over range of index buffer");
			return nullptr;
		}

		++lockedCount_;
		lockedIB_.Init(this, lockFlags, offset, count);
		return &lockedIB_;
	}

	void IndexBuffer::Unlock(LockedIndexBuffer* L)
	{
		if (!L || L->indexBuffer_ != this)
		{
			ASSERT(false && "can not unlock index buffer");
			return;
		}

		--lockedCount_;
		if (lockedCount_ == 0)
			L->Clear();
		else
			ASSERT(L->GetLockFlags() == LockFlags::LockRead && "multi-lock must be read");
	}

	void IndexBuffer::Invalidate()
	{
		if ((storageFlags_ & StorageFlags::Hardware) == 0)
			return;

		// update index buffer
		if (!HWResource_)
			HWResource_ = hw::CreateResource<IndexBufferResource>();

		HWResource_->Attach(this);
		HWResource_->UpdateResource();
	}
}