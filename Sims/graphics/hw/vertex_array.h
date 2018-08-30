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

namespace sims
{
	class VertexArray : public IResourceOperation
	{
	public:
		VertexArray(const VertexDeclarationRef& vertexDecl);
		VertexArray(const VertexDeclarationRef& vertexDecl, const VertexBufferRef& vb1);
		VertexArray(const VertexDeclarationRef& vertexDecl, const VertexBufferRef& vb1, const VertexBufferRef& vb2);
		VertexArray(const VertexDeclarationRef& vertexDecl, const VertexBufferRef& vb1, const VertexBufferRef& vb2, const VertexBufferRef& vb3);

		VertexDeclarationRef GetVertexDeclaration() const { return vertexDecl_; }
		uint32 GetVertexBufferCount() const { return vertexBuffers_.size(); }
		VertexBufferRef GetVertexBuffer(int index) const { return vertexBuffers_[index]; }

		// ~ IResourceOperation
	private:
		virtual void Create();
		// ~ IResourceOperation

		VertexDeclarationRef vertexDecl_;
		vector<VertexBufferRef> vertexBuffers_;
	};
}

