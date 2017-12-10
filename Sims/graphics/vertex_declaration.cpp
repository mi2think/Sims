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
				if (checkStream.GetIndex() == stream.GetIndex() &&
					checkStream.GetUsage() == stream.GetUsage())
				{
					return false;
				}
			}
		}
		return true;
	}

	VertexDeclaration::VertexDeclaration(VertexStream* streams, uint32 streamCount, uint32 stride)
		: streams_(streams)
		, streamCount_(streamCount)
		, stride_(stride)
	{
		ASSERT(CheckStreams(streams_, streamCount_)
			&& "two vertex stream has same usage and index");
	}

	VertexDeclaration::~VertexDeclaration()
	{
		SAFE_DELETEARRAY(streams_);
	}

	VertexDeclarationRef VertexDeclaration::Get(const VertexStream* streams, uint32 streamCount)
	{
		VertexStream* myStreams = new VertexStream[streamCount];
		memcpy(myStreams, streams, streamCount * sizeof(VertexStream));

		// compute stride
		uint32 stride = 0;
		for (uint32 i = 0; i < streamCount; ++i)
		{
			VertexStream& stream = myStreams[i];
			stride = align(stride, stream.GetAlign());
			stream.SetOffset(stride);

			stride += stream.GetSize();
		}

		return VertexDeclarationRef(new VertexDeclaration(myStreams, streamCount, stride));
	}
}