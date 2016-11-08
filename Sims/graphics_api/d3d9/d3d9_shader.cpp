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

namespace sims
{
	namespace d3d9
	{
		D3D9Shader::D3D9Shader()
			: Shader()
			, so_(nullptr)
			, table_(nullptr)
		{}

		D3D9Shader::~D3D9Shader()
		{
			DeleteInternal();
		}

		bool D3D9Shader::IsValid() const
		{
			return so_ != nullptr;
		}

		bool D3D9Shader::Compile(ShaderDomain::Type type, const string& source)
		{
			ASSERT(type == ShaderDomain::Vertex || type == ShaderDomain::Fragment);
			ASSERT(!IsValid());

			type_ = type;
			source_ = source;

			// compile shader
			ID3DXBuffer* shaderBuffer = nullptr;
			ID3DXBuffer* errorBuffer = nullptr;
			uint32 flags = D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY;
#ifdef _DEBUG
			flags |= D3DXSHADER_DEBUG;
			flags |= D3DXSHADER_SKIPOPTIMIZATION;
#endif
			if (type == ShaderDomain::Vertex)
			{
				CHECK_HR = D3DXCompileShader(source.c_str(),
					source.size(),
					0,
					0,
					"main",
					g_RendererCaps.GetVSVersionProfile(),
					flags,
					&shaderBuffer,
					&errorBuffer,
					&table_);
			}
			else if (type == ShaderDomain::Fragment)
			{
				CHECK_HR = D3DXCompileShader(source.c_str(),
					source.size(),
					0,
					0,
					"main",
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
				return false;
			}

			// create shader
			return CreateShaderObj(type, (uint8*)shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize());
		}

		bool D3D9Shader::LoadBinary(ShaderDomain::Type type, uint8* byteCode, uint32 byteCodeLength)
		{
			return CreateShaderObj(type, byteCode, byteCodeLength);
		}

		bool D3D9Shader::CreateShaderObj(ShaderDomain::Type type, uint8* byteCode, uint32)
		{
			if (type == ShaderDomain::Vertex)
			{
				IDirect3DVertexShader9* vertexShader = nullptr;
				CHECK_HR = g_pD3DD->CreateVertexShader((DWORD*)byteCode, &vertexShader);
				so_ = vertexShader;
				return so_ != nullptr;
			}
			else if (type == ShaderDomain::Fragment)
			{
				IDirect3DPixelShader9* pixelShader = nullptr;
				CHECK_HR = g_pD3DD->CreatePixelShader((DWORD*)byteCode, &pixelShader);
				so_ = pixelShader;
				return so_ != nullptr;
			}
			return false;
		}

		void D3D9Shader::Delete()
		{
			DeleteInternal();
		}

		void D3D9Shader::DeleteInternal()
		{
			SAFE_RELEASE(table_);
			if (so_)
			{
				if (type_ == ShaderDomain::Vertex)
				{
					IDirect3DVertexShader9* vertexShader = (IDirect3DVertexShader9*)so_;
					SAFE_RELEASE(vertexShader);
				}
				else if (type_ == ShaderDomain::Fragment)
				{
					IDirect3DPixelShader9* pixelShader = (IDirect3DPixelShader9*)so_;
					SAFE_RELEASE(pixelShader);
				}
				so_ = nullptr;
			}
		}

		void D3D9Shader::Bind()
		{
			if (type_ == ShaderDomain::Vertex)
			{
				IDirect3DVertexShader9* vertexShader = (IDirect3DVertexShader9*)so_;
				CHECK_HR = g_pD3DD->SetVertexShader(vertexShader);
			}
			else if (type_ == ShaderDomain::Fragment)
			{
				IDirect3DPixelShader9* pixelShader = (IDirect3DPixelShader9*)so_;
				CHECK_HR = g_pD3DD->SetPixelShader(pixelShader);
			}
		}

		UniformLoc D3D9Shader::GetUniformLoc(const char* name, UniformLoc parent)
		{
			D3DXHANDLE h = table_->GetConstantByName((D3DXHANDLE)parent, name);
			return (UniformLoc)h;
		}
	}
}