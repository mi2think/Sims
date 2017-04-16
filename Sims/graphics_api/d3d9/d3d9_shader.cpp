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
			InternalReleaseResource();
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
				VERIFYD3DRESULT(D3DXCompileShader(source.GetData(),
					source.GetSize(),
					0,
					0,
					shader_->GetEntryName().GetData(),
					g_RendererCaps.GetVSVersionProfile(),
					flags,
					&shaderBuffer,
					&errorBuffer,
					&table_));
			}
			else if (type == ShaderDomain::Fragment)
			{
				VERIFYD3DRESULT(D3DXCompileShader(source.GetData(),
					source.GetSize(),
					0,
					0,
					shader_->GetEntryName().GetData(),
					g_RendererCaps.GetPSVersionProfile(),
					flags,
					&shaderBuffer,
					&errorBuffer,
					&table_));
			}

			if (errorBuffer != nullptr)
			{
				LOG_ERROR("%s", (char*)errorBuffer->GetBufferPointer());
				SAFE_RELEASE(errorBuffer);
				return;
			}

			if (type == ShaderDomain::Vertex)
			{
				VERIFYD3DRESULT(g_pD3DD->CreateVertexShader((DWORD*)shaderBuffer->GetBufferPointer(), &vsResource_));
			}
			else if (type == ShaderDomain::Fragment)
			{
				VERIFYD3DRESULT(g_pD3DD->CreatePixelShader((DWORD*)shaderBuffer->GetBufferPointer(), &psResource_));
			}

			AnalyseConstants();
		}

		void D3D9ShaderResource::BindResource() const
		{
			auto type = shader_->GetType();
			if (type == ShaderDomain::Vertex)
			{
				VERIFYD3DRESULT(g_pD3DD->SetVertexShader(vsResource_));
			}
			else if (type == ShaderDomain::Fragment)
			{
				VERIFYD3DRESULT(g_pD3DD->SetPixelShader(psResource_));
			}
		}

		void D3D9ShaderResource::ReleaseResource()
		{
			InternalReleaseResource();
		}

		void D3D9ShaderResource::InternalReleaseResource()
		{
			auto type = shader_->GetType();

			SAFE_RELEASE(table_);
			if (resource_)
			{
				if (type == ShaderDomain::Vertex)
					SAFE_RELEASE(vsResource_);
				else if (type == ShaderDomain::Fragment)
					SAFE_RELEASE(psResource_);

				resource_ = nullptr;
			}

			constants_.clear();
			samplers_.clear();
		}

		void D3D9ShaderResource::AnalyseConstants()
		{
			D3DXCONSTANTTABLE_DESC tableDesc;
			VERIFYD3DRESULT(table_->GetDesc(&tableDesc));

			D3DXCONSTANT_DESC cDesc;
			for (uint32 i = 0; i < tableDesc.Constants; ++i)
			{
				uint32 count = 1;
				table_->GetConstantDesc(table_->GetConstant(0, i), &cDesc, &count);

				ConstVar var;
				var.regIndex = cDesc.RegisterIndex;
				var.name = cDesc.Name;
				var.type = cDesc.Type;

				vector<ConstVar>* pV = &constants_;
				if (cDesc.Type >= D3DXPT_SAMPLER && cDesc.Type <= D3DXPT_SAMPLERCUBE)
					pV = &samplers_;

				pV->push_back(var);
			}
		}

		void D3D9ShaderResource::SetConstant(const char* name, const void* data, uint32 dataSize)
		{
			const ConstVar* pVar = nullptr;
			for (const auto& var : constants_)
			{
				if (name == var.name)
				{
					pVar = &var;
				}
			}
			ASSERT(pVar != nullptr);

			auto type = shader_->GetType();
			if (type == ShaderDomain::Vertex)
			{
				if (pVar->type == D3DXPT_BOOL)
				{
					VERIFYD3DRESULT(g_pD3DD->SetVertexShaderConstantB(pVar->regIndex, (const BOOL*)data, dataSize / sizeof(BOOL)));
				}
				else if (pVar->type == D3DXPT_INT)
				{
					VERIFYD3DRESULT(g_pD3DD->SetVertexShaderConstantI(pVar->regIndex, (const int*)data, dataSize / (sizeof(int) * 4)));
				}
				else if (pVar->type == D3DXPT_FLOAT)
				{
					VERIFYD3DRESULT(g_pD3DD->SetVertexShaderConstantF(pVar->regIndex, (const float*)data, dataSize / (sizeof(float) * 4)));
				}
			}
			else if (type == ShaderDomain::Fragment)
			{
				if (pVar->type == D3DXPT_BOOL)
				{
					VERIFYD3DRESULT(g_pD3DD->SetPixelShaderConstantB(pVar->regIndex, (const BOOL*)data, dataSize / sizeof(BOOL)));
				}
				else if (pVar->type == D3DXPT_INT)
				{
					VERIFYD3DRESULT(g_pD3DD->SetPixelShaderConstantI(pVar->regIndex, (const int*)data, dataSize / (sizeof(int) * 4)));
				}
				else if (pVar->type == D3DXPT_FLOAT)
				{
					VERIFYD3DRESULT(g_pD3DD->SetPixelShaderConstantF(pVar->regIndex, (const float*)data, dataSize / (sizeof(float) * 4)));
				}
			}
		}

		uint32 D3D9ShaderResource::GetSamplerStage(const char* name)
		{
			for (const auto& var : samplers_)
			{
				if (name == var.name)
					return var.regIndex;
			}
			ASSERT(false && "not find sampler!");
			return 0;
		}
	}
}