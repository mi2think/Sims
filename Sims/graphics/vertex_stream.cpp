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
	uint32 VertexElement::GetComponentSize() const
	{
		uint32 size = 0;
		switch (componentType_)
		{
		case DataType::S8:
		case DataType::U8:
			size = 1;
			break;
		case DataType::S16:
		case DataType::U16:
			size = 2;
			break;
		case DataType::S32:
		case DataType::U32:
		case DataType::F32:
			size = 4;
			break;
		default:
			ASSERT(false && "do not support type!");
			break;
		}
		return size;
	}

	uint32 VertexElement::GetTotalSize() const
	{
		return componentCount_ * GetComponentSize();
	}

	void VertexElement::SetOffset(uint32 offset)
	{
		offset_ = offset;
	}

	VertexStream::VertexStream(uint32 index, const VertexElement& v1)
		: index_(index)
	{
		vertexElements_.reserve(1);
		vertexElements_.push_back(v1);

		CalcVertexElementOffset();
	}

	VertexStream::VertexStream(uint32 index, const VertexElement& v1, const VertexElement& v2)
		: index_(index)
	{
		vertexElements_.reserve(2);
		vertexElements_.push_back(v1);
		vertexElements_.push_back(v2);

		CalcVertexElementOffset();
	}

	VertexStream::VertexStream(uint32 index, const VertexElement& v1, const VertexElement& v2, const VertexElement& v3)
		: index_(index)
	{
		vertexElements_.reserve(3);
		vertexElements_.push_back(v1);
		vertexElements_.push_back(v2);
		vertexElements_.push_back(v3);

		CalcVertexElementOffset();
	}

	uint32 VertexStream::GetAlign() const
	{
		return 4; // default
	}

	void VertexStream::CalcVertexElementOffset()
	{
		stride_ = 0;
		for (uint32 i = 0; i < vertexElements_.size(); ++i)
		{
			auto& element = vertexElements_[i];

			stride_ = align(stride_, GetAlign());
			element.SetOffset(stride_);
			stride_ += element.GetTotalSize();
		}
	}
}