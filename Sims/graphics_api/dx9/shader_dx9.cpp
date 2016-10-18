/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   10:42
	filename: 	D:\Code\Sims\Sims\graphics_api\dx9\shader_dx9.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\dx9
	file base:	shader_dx9
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Shader dx9
*********************************************************************/
#include "shader_dx9.h"
#include "renderer_caps_dx9.h"

namespace sims
{
	namespace dx9
	{
		DX9Shader::DX9Shader()
			: RHIShader()
			, constTable_(nullptr)
		{}

		DX9Shader::DX9Shader(ShaderType type, const string& source)
			: RHIShader(type, source)
			, constTable_(nullptr)
		{}

		DX9Shader::~DX9Shader()
		{
			Delete();
		}

		bool DX9Shader::Compile(ShaderType type, const string& source)
		{
			ASSERT(type == ST_Vertex || type == ST_Fragment);

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
			if (type == ST_Vertex)
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
					&constTable_);
			}
			else if (type == ST_Fragment)
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
					&constTable_);
			}

			if (errorBuffer != nullptr)
			{
				LOG_ERROR("%s", (char*)errorBuffer->GetBufferPointer());
				SAFE_RELEASE(errorBuffer);
				return false;
			}

			// create shader
			return CreateShader(type, (uint8*)shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize());
		}

		bool DX9Shader::LoadBinary(ShaderType type, uint8* byteCode, uint32 byteCodeLength)
		{
			return CreateShader(type, byteCode, byteCodeLength);
		}

		bool DX9Shader::CreateShader(ShaderType type, uint8* byteCode, uint32)
		{
			if (type == ST_Vertex)
			{
				IDirect3DVertexShader9* vertexShader = nullptr;
				CHECK_HR = g_pD3DD->CreateVertexShader((DWORD*)byteCode, &vertexShader);
				id_ = vertexShader;
				return id_ != nullptr;
			}
			else if (type == ST_Fragment)
			{
				IDirect3DPixelShader9* pixelShader = nullptr;
				CHECK_HR = g_pD3DD->CreatePixelShader((DWORD*)byteCode, &pixelShader);
				id_ = pixelShader;
				return id_ != nullptr;
			}
			return false;
		}

		void DX9Shader::Delete()
		{
			SAFE_RELEASE(constTable_);
			if (type_ == ST_Vertex)
			{
				IDirect3DVertexShader9* vertexShader = (IDirect3DVertexShader9*)id_;
				SAFE_RELEASE(vertexShader);
			}
			else if (type_ == ST_Fragment)
			{
				IDirect3DPixelShader9* pixelShader = (IDirect3DPixelShader9*)id_;
				SAFE_RELEASE(pixelShader);
			}
		}

		void DX9Shader::Use()
		{
			if (type_ == ST_Vertex)
			{
				IDirect3DVertexShader9* vertexShader = (IDirect3DVertexShader9*)id_;
				CHECK_HR = g_pD3DD->SetVertexShader(vertexShader);
			}
			else if (type_ == ST_Fragment)
			{
				IDirect3DPixelShader9* pixelShader = (IDirect3DPixelShader9*)id_;
				CHECK_HR = g_pD3DD->SetPixelShader(pixelShader);
			}
		}
	}
}