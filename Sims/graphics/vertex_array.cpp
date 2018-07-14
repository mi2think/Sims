/********************************************************************
	created:	2018/01/08
	created:	8:1:2018   11:28
	filename: 	D:\Code\Sims\Sims\graphics\vertex_array.cpp
	file path:	D:\Code\Sims\Sims\graphics
	file base:	vertex_array
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Vertex Array
*********************************************************************/
#include "vertex_array.h"
#include "graphics_api/sims_sdk_hw.h"

namespace sims
{
	VertexArray::VertexArray(const VertexDeclarationRef& vertexDecl)
		: vertexDecl_(vertexDecl)
	{
	}

	VertexArray::VertexArray(const VertexDeclarationRef& vertexDecl, const VertexBufferRef& vb1)
		: vertexDecl_(vertexDecl)
	{
		vertexBuffers_.reserve(1);
		vertexBuffers_.push_back(vb1);
	}

	VertexArray::VertexArray(const VertexDeclarationRef& vertexDecl, const VertexBufferRef& vb1, const VertexBufferRef& vb2)
		: vertexDecl_(vertexDecl)
	{
		vertexBuffers_.reserve(2);
		vertexBuffers_.push_back(vb1);
		vertexBuffers_.push_back(vb2);
	}

	VertexArray::VertexArray(const VertexDeclarationRef& vertexDecl, const VertexBufferRef& vb1, const VertexBufferRef& vb2, const VertexBufferRef& vb3)
		: vertexDecl_(vertexDecl)
	{
		vertexBuffers_.reserve(3);
		vertexBuffers_.push_back(vb1);
		vertexBuffers_.push_back(vb2);
		vertexBuffers_.push_back(vb3);
	}

	void VertexArray::Create()
	{
		if (HWResource_)
		{
			Release();
		}

		HWResource_ = hw::CreateResource<VertexArrayResource>();
	}
}