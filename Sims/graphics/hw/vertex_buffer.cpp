/********************************************************************
	created:	2016/11/28
	created:	28:11:2016   16:15
	filename: 	D:\Code\Sims\Sims\graphics\hw\vertex_buffer.cpp
	file path:	D:\Code\Sims\Sims\graphics\hw
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

	void LockedVertexBuffer::Clear()
	{
		vertexStream_ = nullptr;
		offset_ = count_ = 0;
		lockFlags_ = LockFlags::LockRead;
		data_ = nullptr;
	}

	void LockedVertexBuffer::Init(VertexBuffer* vertexBuffer, uint32 lockFlags, uint32 offset, uint32 count)
	{
		vertexStream_ = vertexBuffer->GetVertexStream();
		offset_ = offset;
		count_ = count;
		lockFlags_ = lockFlags;
		data_ = vertexBuffer->GetVertexData()->GetData() + offset_ * vertexStream_->GetStride();
	}

	uint32 LockedVertexBuffer::GetStride() const
	{
		return vertexStream_->GetStride();
	}

	void* LockedVertexBuffer::GetData()
	{
		return data_;
	}

	const void* LockedVertexBuffer::GetData() const
	{ 
		return data_;
	}

	void* LockedVertexBuffer::GetDataByUsage(VertexElementUsage::Type usage, uint32 usageIndex)
	{
		const VertexElement* p = vertexStream_->GetVertexElementByUsage(usage, usageIndex);
		if (p)
		{
			return data_ + p->GetOffset();
		}
		return nullptr;
	}

	const void* LockedVertexBuffer::GetDataByUsage(VertexElementUsage::Type usage, uint32 usageIndex) const
	{
		const VertexElement* p = vertexStream_->GetVertexElementByUsage(usage, usageIndex);
		if (p)
		{
			return data_ + p->GetOffset();
		}
		return nullptr;
	}

	//////////////////////////////////////////////////////////////////////////


	VertexBuffer::VertexBuffer()
		: vertexCount_(0)
		, isLocked_(false)
		, invalidRange_(0, 0)
		, vertexStream_(nullptr)
	{}

	VertexBuffer::VertexBuffer(const VertexDeclarationRef& vertexDecl, uint32 streamIndex)
		: vertexDecl_(vertexDecl)
		, vertexCount_(0)
		, isLocked_(false)
		, invalidRange_(0, 0)
		, vertexStream_(nullptr)
	{
		vertexStream_ = vertexDecl->GetStream(streamIndex);
	}

	VertexBuffer::VertexBuffer(const VertexDeclarationRef& vertexDecl, uint32 vertexCount, uint32 streamIndex)
		: vertexDecl_(vertexDecl)
		, vertexCount_(vertexCount)
		, isLocked_(false)
		, invalidRange_(0, vertexCount)
		, vertexStream_(nullptr)
	{
		vertexStream_ = vertexDecl->GetStream(streamIndex);
		vertexData_.Resize(vertexCount_ * vertexStream_->GetStride());
	}

	VertexBuffer::~VertexBuffer()
	{
	}

	void VertexBuffer::Resize(uint32 vertexCount)
	{
		ASSERT(!isLocked_ && ! HWResource_);
		vertexData_.Resize(vertexCount * vertexStream_->GetStride());
		vertexCount_ = vertexCount;
	}

	LockedVertexBuffer* VertexBuffer::Lock(uint32 lockFlags, uint32 offset, uint32 count)
	{
		if (isLocked_)
		{
			ASSERT(false && "lock locked vertex buffer");
			return nullptr;
		}

		if (count == 0)
			count = vertexCount_ - offset;
		if (offset + count > vertexCount_)
		{
			ASSERT(false && "over range of vertex buffer");
			return nullptr;
		}

		isLocked_ = true;
		lockedVB_.Init(this, lockFlags, offset, count);
		return &lockedVB_;
	}

	void VertexBuffer::Unlock(LockedVertexBuffer* L)
	{
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

	void VertexBuffer::Invalidate()
	{
		if (isLocked_ || invalidRange_.IsEmpty())
			return;
		invalidRange_.ResetRange();

		IResourceOperation::Invalidate();
	}

	void VertexBuffer::Create()
	{
		if (HWResource_)
		{
			Release();
		}

		HWResource_ = hw::CreateResource<VertexBufferResource>();
	}
}