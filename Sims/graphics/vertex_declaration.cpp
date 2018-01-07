/********************************************************************
	created:	2016/11/28
	created:	28:11:2016   14:42
	filename: 	D:\Code\Sims\Sims\graphics\vertex_declaration.cpp
	file path:	D:\Code\Sims\Sims\graphics
	file base:	vertex_declaration
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Vertex Declaration
*********************************************************************/
#include "vertex_declaration.h"
#include "graphics_api/sims_sdk_hw.h"

namespace sims
{
	static bool CheckStreams(const VertexStream* streams, uint32 count)
	{
		for (uint32 i = 1; i < count; ++i)
		{
			const auto& stream = streams[i];
			for (uint32 j = 0; j <= i - 1; ++j)
			{
				const auto& checkStream = streams[j];
				if (checkStream.GetIndex() == stream.GetIndex())
				{
					return false;
				}
			}
		}
		return true;
	}

	VertexDeclaration::VertexDeclaration(VertexStream* streams, uint32 streamCount)
		: streams_(streams)
		, streamCount_(streamCount)
		, storageFlags_(StorageFlags::Local | StorageFlags::Hardware)
	{
		ASSERT(CheckStreams(streams_, streamCount_)
			&& "two vertex stream has same index");
	}

	VertexDeclaration::~VertexDeclaration()
	{
		SAFE_DELETEARRAY(streams_);
	}

	VertexDeclarationRef VertexDeclaration::Get(const VertexStream* streams, uint32 streamCount)
	{
		VertexStream* myStreams = new VertexStream[streamCount];

		uint32 attriBaseIndex = 0;
		for (uint32 i = 0; i < streamCount; ++i)
		{
			VertexStream& stream = myStreams[i];
			stream = streams[i];
			stream.SetAttriBaseIndex(attriBaseIndex);
			attriBaseIndex += stream.GetVertexElementCount();
		}

		return VertexDeclarationRef(new VertexDeclaration(myStreams, streamCount));
	}

	void VertexDeclaration::Invalidate()
	{
		if ((storageFlags_ & StorageFlags::Hardware) == 0)
			return;

		// update vertex buffer
		if (!HWResource_)
			HWResource_ = hw::CreateResource<VertexDeclarationResource>();

		HWResource_->Attach(this);
		HWResource_->UpdateResource();
	}
}