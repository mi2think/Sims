/********************************************************************
	created:	2018/01/08
	created:	8:1:2018   11:13
	filename: 	D:\Code\Sims\Sims\graphics\vertex_array.h
	file path:	D:\Code\Sims\Sims\graphics
	file base:	vertex_array
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	
		Vertex Array is a container of vertex data.
*********************************************************************/
#pragma once

#include "graphics_fwd.h"
#include "render_resource.h"
#include "vertex_declaration.h"
#include "vertex_buffer.h"

namespace sims
{
	class VertexArray
	{
	public:
		VertexArray(const VertexDeclarationRef& vertexDecl);
		VertexArray(const VertexDeclarationRef& vertexDecl, const VertexBufferRef& vb1);
		VertexArray(const VertexDeclarationRef& vertexDecl, const VertexBufferRef& vb1, const VertexBufferRef& vb2);
		VertexArray(const VertexDeclarationRef& vertexDecl, const VertexBufferRef& vb1, const VertexBufferRef& vb2, const VertexBufferRef& vb3);

		VertexDeclarationRef GetVertexDeclaration() const { return vertexDecl_; }
		uint32 GetVertexBufferCount() const { return vertexBuffers_.size(); }
		VertexBufferRef GetVertexBuffer(int index) const { return vertexBuffers_[index]; }

		void SetStorageFlags(uint32 flags) { storageFlags_ = flags; }
		uint32 GetStorageFlags() const { return storageFlags_; }

		// propagates changes on the vertex array to the renderer.
		//   you must call invalidate after modifying vertex array data,
		//   for it to be uploaded to GPU.
		void Invalidate();

		// Hardware resource
		VertexArrayResourceRef& HWResource() { return HWResource_; }
	private:
		VertexDeclarationRef vertexDecl_;
		vector<VertexBufferRef> vertexBuffers_;

		uint32 storageFlags_;
		VertexArrayResourceRef HWResource_;
	};
}

