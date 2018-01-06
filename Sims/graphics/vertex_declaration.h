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

		const VertexStream* GetStream(uint32 index) const { ASSERT(index < streamCount_); return &streams_[index]; }
		const VertexStream* GetStreams() const { return streams_; }
		uint32 GetStreamCount() const { return streamCount_; }
	private:
		VertexDeclaration(VertexStream* streams, uint32 streamCount);

		VertexStream* streams_;
		uint32 streamCount_;
	};
}
