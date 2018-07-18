/********************************************************************
	created:	2016/11/27
	created:	27:11:2016   21:12
	filename: 	D:\Code\Sims\Sims\graphics\vertex_stream.h
	file path:	D:\Code\Sims\Sims\graphics
	file base:	vertex_stream
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	
		Vertex Stream is responsible for explain vertex data format in vertex buffer,
		each vertex buffer has a vertex stream, and each vertex stream has one vertex
		element or more.

*********************************************************************/
#pragma once

#include "graphics_fwd.h"

namespace sims
{
	class VertexElement
	{
	public:
		VertexElement(VertexElementUsage::Type usage, uint32 usageIndex, DataType::Type componentType, uint32 elementCount)
			: usage_(usage)
			, usageIndex_(usageIndex)
			, componentType_(componentType)
			, componentCount_(elementCount)
			, offset_(0)
		{}

		VertexElementUsage::Type GetUsage() const { return usage_; }
		uint32 GetUsageIndex() const { return usageIndex_; }
		DataType::Type GetComponentType() const { return componentType_; }
		uint32 GetComponentCount() const { return componentCount_; }
		uint32 GetComponentSize() const;
		uint32 GetTotalSize() const;
		uint32 GetOffset() const { return offset_; }
	private:
		friend class VertexStream;
		void SetOffset(uint32 offset);

		VertexElementUsage::Type usage_;
		uint32 usageIndex_;
		DataType::Type componentType_;
		uint32 componentCount_;
		// offset in vertex stream
		uint32 offset_;
	};

	class VertexStream
	{
	public:
		VertexStream();
		VertexStream(const VertexElement& v1, uint32 index = 0);
		VertexStream(const VertexElement& v1, const VertexElement& v2, uint32 index = 0);
		VertexStream(const VertexElement& v1, const VertexElement& v2, const VertexElement& v3, uint32 index = 0);
		VertexStream(const VertexElement* v, uint32 num, uint32 index = 0);

		uint32 SetIndex(uint32 index) { index_ = index; }
		uint32 GetIndex() const { return index_; }
		uint32 GetAttriBaseIndex() const { return attriBaseIndex_; }
		uint32 GetStride() const { return stride_; }
		uint32 GetAlign() const;

		uint32 GetVertexElementCount() const { return vertexElements_.size(); }
		const VertexElement& GetVertexElement(int elementIndex) const { return vertexElements_[elementIndex]; }
		const VertexElement* GetVertexElementByUsage(VertexElementUsage::Type usage, uint32 usageIndex = 0) const;
	private:
		void CalcVertexElementOffset();

		friend class VertexDeclaration;
		void SetAttriBaseIndex(uint32 attriBaseIndex);
		
		// stream index
		uint32 index_;
		uint32 stride_;
		vector<VertexElement> vertexElements_;
		// attribute index
		uint32 attriBaseIndex_;
	};
}

