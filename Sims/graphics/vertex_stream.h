/********************************************************************
	created:	2016/11/27
	created:	27:11:2016   21:12
	filename: 	D:\Code\Sims\Sims\graphics\vertex_stream.h
	file path:	D:\Code\Sims\Sims\graphics
	file base:	vertex_stream
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Vertex Stream
*********************************************************************/
#pragma once

#include "graphics_fwd.h"

namespace sims
{
	// POD
	class VertexStream
	{
	public:
		VertexStream() = default;
		VertexStream(VertexStreamUsage::Type usage, uint32 index, VertexStreamElementType::Type type, uint32 elementCount);

		VertexStreamUsage::Type GetUsage() const { return usage_; }
		uint32 GetIndex() const { return index_; }
		VertexStreamElementType::Type GetElementType() const { return elementType_; }
		uint32 GetElementCount() const { return elementCount_; }
		uint32 GetElementSize() const;

		uint32 GetSize() const;

		uint32 GetAlign() const;
		uint32 GetOffset() const { return offset_; }
	private:
		friend class VertexDeclaration;
		void SetOffset(uint32 offset);

		VertexStreamUsage::Type usage_;
		uint32 index_;
		VertexStreamElementType::Type elementType_;
		uint32 elementCount_;

		// offset in vertex declaration
		uint32 offset_;
	};
}

