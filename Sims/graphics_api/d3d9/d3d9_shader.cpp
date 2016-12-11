/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   10:42
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_shader.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_shader
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	D3D9 Shader
*********************************************************************/
#include "d3d9_shader.h"
#include "d3d9_renderer_caps.h"
#include "graphics/shader.h"

namespace sims
{
	namespace d3d9
	{
		D3D9ShaderResource::D3D9ShaderResource()
			: ShaderResource()
			, resource_(nullptr)
			, table_(nullptr)
		{}

		D3D9ShaderResource::~D3D9ShaderResource()
		{
			ASSERT(!resource_ && !table_);
		}

		void D3D9ShaderResource::UpdateResource()
		{
			ReleaseResource();

			// compile shader
			ID3DXBuffer* shaderBuffer = nullptr;
			ID3DXBuffer* errorBuffer = nullptr;

			auto type = shader_->GetType();
			const auto& source = shader_->GetSource();

			uint32 flags = D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY;
#ifdef _DEBUG
			flags |= D3DXSHADER_DEBUG;
			flags |= D3DXSHADER_SKIPOPTIMIZATION;
#endif
			if (type == ShaderDomain::Vertex)
			{
				CHECK_HR = D3DXCompileShader(source.GetData(),
					source.GetSize(),
					0,
					0,
					shader_->GetEntryName().GetData(),
					g_RendererCaps.GetVSVersionProfile(),
					flags,
					&shaderBuffer,
					&errorBuffer,
					&table_);
			}
			else if (type == ShaderDomain::Fragment)
			{
				CHECK_HR = D3DXCompileShader(source.GetData(),
					source.GetSize(),
					0,
					0,
					shader_->GetEntryName().GetData(),
					g_RendererCaps.GetPSVersionProfile(),
					flags,
					&shaderBuffer,
					&errorBuffer,
					&table_);
			}

			if (errorBuffer != nullptr)
			{
				LOG_ERROR("%s", (char*)errorBuffer->GetBufferPointer());
				SAFE_RELEASE(errorBuffer);
				return;
			}

			if (type == ShaderDomain::Vertex)
			{
				IDirect3DVertexShader9* vertexShader = nullptr;
				CHECK_HR = g_pD3DD->CreateVertexShader((DWORD*)shaderBuffer->GetBufferPointer(), &vertexShader);
				resource_ = vertexShader;
			}
			else if (type == ShaderDomain::Fragment)
			{
				IDirect3DPixelShader9* pixelShader = nullptr;
				CHECK_HR = g_pD3DD->CreatePixelShader((DWORD*)shaderBuffer->GetBufferPointer(), &pixelShader);
				resource_ = pixelShader;
			}
		}

		void D3D9ShaderResource::BindResource()
		{
			auto type = shader_->GetType();
			if (type == ShaderDomain::Vertex)
			{
				IDirect3DVertexShader9* vertexShader = (IDirect3DVertexShader9*)resource_;
				CHECK_HR = g_pD3DD->SetVertexShader(vertexShader);
			}
			else if (type == ShaderDomain::Fragment)
			{
				IDirect3DPixelShader9* pixelShader = (IDirect3DPixelShader9*)resource_;
				CHECK_HR = g_pD3DD->SetPixelShader(pixelShader);
			}
		}

		void D3D9ShaderResource::ReleaseResource()
		{
			auto type = shader_->GetType();

			SAFE_RELEASE(table_);
			if (resource_)
			{
				if (type == ShaderDomain::Vertex)
				{
					IDirect3DVertexShader9* vertexShader = (IDirect3DVertexShader9*)resource_;
					SAFE_RELEASE(vertexShader);
				}
				else if (type == ShaderDomain::Fragment)
				{
					IDirect3DPixelShader9* pixelShader = (IDirect3DPixelShader9*)resource_;
					SAFE_RELEASE(pixelShader);
				}
				resource_ = nullptr;
			}
		}

		//UniformLoc D3D9ShaderResource::GetUniformLoc(const char* name, UniformLoc parent)
		//{
		//	D3DXHANDLE h = table_->GetConstantByName((D3DXHANDLE)parent, name);
		//	return (UniformLoc)h;
		//}
	}
}