/********************************************************************
	created:	2018/01/08
	created:	8:1:2018   11:28
	filename: 	D:\Code\Sims\Sims\graphics\hw\vertex_array.cpp
	file path:	D:\Code\Sims\Sims\graphics\hw
	file base:	vertex_array
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Vertex Array
*********************************************************************/
#include "vertex_array.h"
#include "graphics_api/sims_sdk_hw.h"

namespace sims
{
	VertexArray::VertexArray(const VertexDeclarationRef& vertexDecl, const IndexBufferRef& indexBuffer, const vector<VertexBufferRef>& vertexBuffers)
		: vertexDecl_(vertexDecl)
	{
		indexBuffer_ = indexBuffer;
		vertexBuffers_ = vertexBuffers;
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