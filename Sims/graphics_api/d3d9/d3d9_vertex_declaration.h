/********************************************************************
	created:	2018/01/07
	created:	7:1:2018   19:06
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_vertex_declaration.h
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_vertex_declaration
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	D3D9 Vertex Declaration
*********************************************************************/
#pragma once

#include "graphics/render_resource.h"
#include "d3d9_common.h"

namespace sims
{
	namespace d3d9
	{
		class D3D9VertexDeclarationResource : public VertexDeclarationResource
		{
		public:
			D3D9VertexDeclarationResource();
			~D3D9VertexDeclarationResource();

			virtual void UpdateResource() override;
			virtual void BindResource() const override;
			virtual void ReleaseResource() override;
		private:
			void InternalReleaseResource();

			IDirect3DVertexDeclaration9* decl_;
		};
	}
}

