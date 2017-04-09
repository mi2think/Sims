/********************************************************************
	created:	2016/11/28
	created:	28:11:2016   16:15
	filename: 	D:\Code\Sims\Sims\graphics\vertex_buffer.cpp
	file path:	D:\Code\Sims\Sims\graphics
	file base:	vertex_buffer
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Vertex Buffer
*********************************************************************/
#include "vertex_buffer.h"
#include "graphics_api/sims_sdk_hw.h"

namespace sims
{
	LockedVertexBuffer::LockedVertexBuffer()
	{
		Clear();
	}

	LockedVertexBuffer::LockedVertexBuffer(VertexBuffer* vertexBuffer, uint32 lockFlags, uint32 offset, uint32 count)
	{
		Init(vertexBuffer, lockFlags, offset, count);
	}
	
	const VertexDeclarationRef& LockedVertexBuffer::GetVertexDeclaration() const
	{
		return vertexBuffer_->GetVertexDeclaration();
	}

	void LockedVertexBuffer::Clear()
	{
		vertexBuffer_ = nullptr;
		offset_ = count_ = 0;
		lockFlags_ = LockFlags::LockRead;
		vertexData_ = nullptr;
	}

	void LockedVertexBuffer::Init(VertexBuffer* vertexBuffer, uint32 lockFlags, uint32 offset, uint32 count)
	{
		vertexBuffer_ = vertexBuffer;
		offset_ = offset;
		count_ = count;
		lockFlags_ = lockFlags;
		vertexData_ = vertexBuffer_->GetVertexData();
	}

	VertexBuffer::VertexBuffer()
		: vertexCount_(0)
		, storageFlags_(StorageFlags::Local)
		, lockedCount_(0)
	{}

	VertexBuffer::VertexBuffer(const VertexDeclarationRef& vertexDecl, uint32 vertexCount)
		: vertexDecl_(vertexDecl)
		, vertexCount_(vertexCount)
		, storageFlags_(StorageFlags::Local)
		, lockedCount_(0)
	{
		vertexData_.Resize(vertexCount_ * vertexDecl->GetStride());
	}

	VertexBuffer::~VertexBuffer()
	{
	}

	LockedVertexBuffer* VertexBuffer::Lock(uint32 lockFlags, uint32 offset, uint32 count)
	{
		if (lockedCount_ > 0)
		{
			// read is ok if pre-lock is read
			if ((lockFlags & LockFlags::LockWrite) != 0 ||
				(lockedVB_.lockFlags_ & LockFlags::LockWrite) != 0)
			{
				ASSERT(false && "lock locked vertex buffer");
				return nullptr;
			}
		}

		if (count == 0)
			count = vertexCount_ - offset;
		if (offset + count > vertexCount_)
		{
			ASSERT(false && "over range of vertex buffer");
			return nullptr;
		}

		++lockedCount_;
		lockedVB_.Init(this, lockFlags, offset, count);
		return &lockedVB_;
	}

	void VertexBuffer::Unlock(LockedVertexBuffer* L)
	{
		if (!L || L->vertexBuffer_ != this)
		{
			ASSERT(false && "can not unlock vertex buffer");
			return;
		}

		--lockedCount_;
		if (lockedCount_ == 0)
			L->Clear();
		else
			ASSERT(L->GetLockFlags() == LockFlags::LockRead && "multi-lock must be read");
	}

	void VertexBuffer::Invalidate()
	{
		if ((storageFlags_ & StorageFlags::Hardware) == 0)
			return;

		// update vertex buffer
		if (!HWResource_)
			HWResource_ = hw::CreateResource<VertexBufferResource>();
		
		HWResource_->Attach(this);
		HWResource_->UpdateResource();
	}
}