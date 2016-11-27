/********************************************************************
	created:	2016/11/27
	created:	27:11:2016   21:41
	filename: 	D:\Code\Sims\Sims\graphics\vertex_stream.cpp
	file path:	D:\Code\Sims\Sims\graphics
	file base:	vertex_stream
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Vertex Stream
*********************************************************************/
#include "vertex_stream.h"

namespace sims
{
	VertexStream::VertexStream(VertexStreamUsage::Type usage, uint32 index, VertexStreamElementType::Type type, uint32 elementCount)
		: usage_(usage)
		, index_(index)
		, elementType_(type)
		, elementCount_(elementCount)
	{}

	uint32 VertexStream::GetElementSize() const
	{
		uint32 size = 0;
		switch (elementType_)
		{
		case VertexStreamElementType::S8:
		case VertexStreamElementType::U8:
			size = 1;
			break;
		case VertexStreamElementType::S16:
		case sims::VertexStreamElementType::U16:
			size = 2;
			break;
		case sims::VertexStreamElementType::S32:
		case sims::VertexStreamElementType::U32:
		case sims::VertexStreamElementType::F32:
			size = 4;
			break;
		default:
			ASSERT(false && "do not support type!");
			break;
		}
		return size;
	}

	uint32 VertexStream::GetSize() const
	{
		return elementCount_ * GetElementSize();
	}
}