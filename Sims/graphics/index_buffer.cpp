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

	IndexType* LockedIndexBuffer::GetData() 
	{ 
		return indexData_->GetData() + offset_;
	}

	const IndexType* LockedIndexBuffer::GetData() const 
	{ 
		return indexData_->GetData() + offset_;
	}

	//////////////////////////////////////////////////////////////////////////

	IndexBuffer::IndexBuffer()
		: indexCount_(0)
		, isLocked_(false)
		, invalidRange_(0, 0)
	{
	}

	IndexBuffer::IndexBuffer(uint32 indexCount)
		: indexCount_(indexCount)
		, isLocked_(false)
		, invalidRange_(0, indexCount)
	{
		indexData_.Resize(indexCount);
	}

	IndexBuffer::IndexBuffer(uint32 indexCount, IndexType* data)
		: indexCount_(indexCount)
		, isLocked_(false)
		, invalidRange_(0, indexCount)
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
		ASSERT(!isLocked_ && !HWResource_);
		indexData_.Resize(indexCount);
		indexCount_ = indexCount;
	}

	LockedIndexBuffer* IndexBuffer::Lock(uint32 lockFlags, uint32 offset, uint32 count)
	{
		if (isLocked_)
		{
			ASSERT(false && "lock locked index buffer");
			return nullptr;
		}

		if (count == 0)
			count = indexCount_ - offset;
		if (offset + count > indexCount_)
		{
			ASSERT(false && "over range of index buffer");
			return nullptr;
		}

		isLocked_ = true;
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

		if (!isLocked_)
		{
			ASSERT(false && "can not unlock unlocked vertex buffer");
			return;
		}

		if ((L->GetLockFlags() & LockFlags::LockWrite) != 0)
			invalidRange_.Union(IndexRange(L->GetOffset(), L->GetCount()));

		L->Clear();
		isLocked_ = false;
	}

	void IndexBuffer::Invalidate()
	{
		if (isLocked_ || invalidRange_.IsEmpty())
			return;
		invalidRange_.ResetRange();

		IResourceOperation::Invalidate();
	}

	void IndexBuffer::Create()
	{
		if (HWResource_)
		{
			Release();
		}

		HWResource_ = hw::CreateResource<IndexBufferResource>();
	}
}