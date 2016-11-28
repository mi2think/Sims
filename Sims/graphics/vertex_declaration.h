/********************************************************************
	created:	2016/11/27
	created:	27:11:2016   21:59
	filename: 	D:\Code\Sims\Sims\graphics\vertex_declaration.h
	file path:	D:\Code\Sims\Sims\graphics
	file base:	vertex_declaration
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Vertex Declaration
*********************************************************************/
#pragma once

#include "graphics_fwd.h"
#include "vertex_stream.h"

namespace sims
{
	class VertexDeclaration
	{
	public:
		static VertexDeclarationRef Get(const VertexStream* streams, uint32 streamCount);
		~VertexDeclaration();

		const VertexStream* GetStreams() const { return streams_; }
		uint32 GetStreamCount() const { return streamCount_; }
		uint32 GetStride() const { return stride_; }
	private:
		// stride computed by steams
		VertexDeclaration(VertexStream* streams, uint32 streamCount, uint32 stride);

		VertexStream* streams_;
		uint32 streamCount_;
		uint32 stride_;
	};
}
