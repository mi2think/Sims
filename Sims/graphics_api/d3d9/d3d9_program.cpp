/********************************************************************
	created:	2016/11/08
	created:	8:11:2016   14:29
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_program.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_program
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	D3D9 Program
*********************************************************************/
#include "d3d9_program.h"
#include "d3d9_shader.h"
#include "graphics/program.h"

namespace sims
{
	namespace d3d9
	{
		D3D9ProgramResource::D3D9ProgramResource()
			: ProgramResource()
		{
		}

		D3D9ProgramResource::~D3D9ProgramResource()
		{
		}

		void D3D9ProgramResource::UpdateResource()
		{
		}

		void D3D9ProgramResource::BindResource()
		{
			for (int i = 0; i < ShaderDomain::Max; ++i)
			{
				auto type = ShaderDomain::Type(i);
				auto shader = program_->GetShader(type);
				if (shader && shader->HWResource())
					shader->HWResource()->BindResource();
				else
				{
					switch (type)
					{
					case ShaderDomain::Vertex:
						g_pD3DD->SetVertexShader(nullptr);
						break;
					case ShaderDomain::Fragment:
						g_pD3DD->SetPixelShader(nullptr);
						break;
					default:
						break;
					}
				}
			}
		}

		void D3D9ProgramResource::ReleaseResource()
		{
		}
	}
}