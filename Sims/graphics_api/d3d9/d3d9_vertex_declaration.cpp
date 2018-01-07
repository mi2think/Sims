/********************************************************************
	created:	2018/01/07
	created:	7:1:2018   19:08
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_vertex_declaration.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_vertex_declaration
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	D3D9 Vertex Declaration
*********************************************************************/
#include "d3d9_vertex_declaration.h"
#include "graphics/vertex_declaration.h"

namespace sims
{
	namespace d3d9
	{
		D3D9VertexDeclarationResource::D3D9VertexDeclarationResource()
			: VertexDeclarationResource()
			, decl_(nullptr)
		{}

		D3D9VertexDeclarationResource::~D3D9VertexDeclarationResource()
		{
			InternalReleaseResource();
		}

		void D3D9VertexDeclarationResource::UpdateResource()
		{
			if (!decl_)
			{
				uint32 elementCount = 0;
				uint32 streamCount = vertexDecl_->GetStreamCount();
				for (uint32 i = 0; i < streamCount; ++i)
				{
					elementCount += vertexDecl_->GetStream(i)->GetVertexElementCount();
				}

				TBuffer<D3DVERTEXELEMENT9> elements(elementCount + 1); // +1 for D3DDECL_END
				uint32 index = 0;
				for (uint32 i = 0; i < streamCount; ++i)
				{
					const VertexStream* vertexStream = vertexDecl_->GetStream(i);
					for (uint32 j = 0; j < vertexStream->GetVertexElementCount(); ++j)
					{
						FillD3DVertexElement(&elements[index], &vertexStream->GetVertexElement(j), i);
						++index;
					}
				}
				elements[index] = D3DDECL_END();
				VERIFYD3DRESULT(g_pD3DD->CreateVertexDeclaration(elements.GetData(), &decl_));
			}
		}

		void D3D9VertexDeclarationResource::BindResource() const
		{
			ASSERT(decl_);

			VERIFYD3DRESULT(g_pD3DD->SetVertexDeclaration(decl_));
		}

		void D3D9VertexDeclarationResource::ReleaseResource()
		{
			InternalReleaseResource();
		}

		void D3D9VertexDeclarationResource::InternalReleaseResource()
		{
			SAFE_RELEASE(decl_);
		}
	}
}
