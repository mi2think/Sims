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
#ifdef SIMS_DEBUG
			flags |= D3DXSHADER_DEBUG;
			flags |= D3DXSHADER_SKIPOPTIMIZATION;
#endif
			if (type == ShaderDomain::Vertex)
			{
				VERIFYD3DRESULT(D3DXCompileShader(source.c_str(),
					source.size(),
					0,
					0,
					shader_->GetEntryName().c_str(),
					g_RendererCaps.GetVSVersionProfile(),
					flags,
					&shaderBuffer,
					&errorBuffer,
					&table_));
			}
			else if (type == ShaderDomain::Fragment)
			{
				VERIFYD3DRESULT(D3DXCompileShader(source.c_str(),
					source.size(),
					0,
					0,
					shader_->GetEntryName().c_str(),
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

			AnalyseUniforms();
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

			uniforms_.clear();
		}

		void D3D9ShaderResource::AnalyseUniforms()
		{
			D3DXCONSTANTTABLE_DESC tableDesc;
			VERIFYD3DRESULT(table_->GetDesc(&tableDesc));

			D3DXCONSTANT_DESC cDesc;
			for (uint32 i = 0; i < tableDesc.Constants; ++i)
			{
				uint32 count = 1;
				table_->GetConstantDesc(table_->GetConstant(0, i), &cDesc, &count);

				UniformVar var;
				var.regIndex = cDesc.RegisterIndex;
				var.name = cDesc.Name;
				var.type = cDesc.Type;
				var.hConstant = table_->GetConstant(0, i);

				uniforms_.push_back(var);
			}
		}

		void D3D9ShaderResource::SetUniform(const char* name, const void* data, uint32 dataSize)
		{
			auto it = std::find_if(uniforms_.begin(), uniforms_.end(), [&name](const UniformVar& var) { return var.name == name; });
			ASSERT(it != uniforms_.end());

			if (it->type == D3DXPT_BOOL)
			{
				uint32 count = MAX(1, dataSize / sizeof(BOOL));
				VERIFYD3DRESULT(table_->SetBoolArray(g_pD3DD, it->hConstant, (const BOOL*)data, count));
			}
			else if (it->type == D3DXPT_INT)
			{
				uint32 count = MAX(1, dataSize / sizeof(int));
				VERIFYD3DRESULT(table_->SetIntArray(g_pD3DD, it->hConstant, (const int*)data, count));
			}
			else if (it->type == D3DXPT_FLOAT)
			{
				uint32 count = MAX(1, dataSize / sizeof(float));
				VERIFYD3DRESULT(table_->SetFloatArray(g_pD3DD, it->hConstant, (const float*)data, count));
			}
			else
			{
				LOG_ERROR("D3D9ShaderResource - SetUniform not support type!");
			}
		}

		uint32 D3D9ShaderResource::GetSamplerStage(const char* name)
		{
			for (const auto& var : uniforms_)
			{
				if (name == var.name && var.type >= D3DXPT_SAMPLER)
					return var.regIndex;
			}
			ASSERT(false && "not find sampler!");
			return 0;
		}
	}
}