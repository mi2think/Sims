/********************************************************************
	created:	2016/11/30
	created:	30:11:2016   21:03
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_vertex_buffer.h
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_vertex_buffer
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	D3D9 Vertex Buffer
*********************************************************************/
#pragma once

#include "graphics/vertex_buffer.h"
#include "d3d9_common.h"

namespace sims
{
	namespace d3d9
	{
		class D3D9VertexBuffer : public VertexBuffer
		{
		public:
			D3D9VertexBuffer();
			D3D9VertexBuffer(const VertexDeclarationRef& vertexDecl, uint32 vertexCount);
			~D3D9VertexBuffer();

			virtual void HWUpdateVertexBuffer();
			virtual void HWBindVertexBuffer();
			virtual void HWDeleteVertexBuffer();
		private:
			IDirect3DVertexBuffer9* vb_;
			IDirect3DVertexDeclaration9* decl_;
		};
	}
}
