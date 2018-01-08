/********************************************************************
	created:	2018/01/08
	created:	8:1:2018   11:45
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_vertex_array.h
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_vertex_array
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	D3D9 Vertex Array
*********************************************************************/
#pragma once

#include "graphics/render_resource.h"
#include "d3d9_common.h"

namespace sims
{
	namespace d3d9
	{
		class D3D9VertexArrayResource : public VertexArrayResource
		{
		public:
			D3D9VertexArrayResource();
			~D3D9VertexArrayResource();

			virtual void UpdateResource() override;
			virtual void BindResource() const override;
			virtual void ReleaseResource() override;
		};
	}
}
