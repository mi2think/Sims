/********************************************************************
	created:	2018/01/08
	created:	8:1:2018   11:13
	filename: 	D:\Code\Sims\Sims\graphics\hw\vertex_array.h
	file path:	D:\Code\Sims\Sims\graphics\hw
	file base:	vertex_array
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	
		Vertex Array is a container of vertex data.
*********************************************************************/
#pragma once

#include "graphics/graphics_fwd.h"
#include "render_resource.h"
#include "vertex_declaration.h"
#include "vertex_buffer.h"
#include "index_buffer.h"

namespace sims
{
	class VertexArray : public IResourceOperation
	{
	public:
		VertexArray(const VertexDeclarationRef& vertexDecl, const IndexBufferRef& IndexBuffer, const vector<VertexBufferRef>&  vertexBuffers);

		VertexDeclarationRef GetVertexDeclaration() const { return vertexDecl_; }
		uint32 GetVertexBufferCount() const { return vertexBuffers_.size(); }
		VertexBufferRef GetVertexBuffer(int index) const { return vertexBuffers_[index]; }
		IndexBufferRef GetIndexBuffer() const { return indexBuffer_; }
		// ~ IResourceOperation
	private:
		virtual void Create();
		// ~ IResourceOperation

		VertexDeclarationRef vertexDecl_;
		IndexBufferRef indexBuffer_;
		vector<VertexBufferRef> vertexBuffers_;
	};
}

