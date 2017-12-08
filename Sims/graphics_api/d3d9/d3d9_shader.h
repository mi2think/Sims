/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   10:28
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_shader.h
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_shader
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	D3D9 Shader
*********************************************************************/
#ifndef __SHADER_DX_H__
#define __SHADER_DX_H__

#include "graphics/render_resource.h"
#include "d3d9_common.h"

namespace sims
{
	namespace d3d9
	{
		class D3D9ShaderResource : public ShaderResource
		{
		public:
			D3D9ShaderResource();
			~D3D9ShaderResource();

			virtual void UpdateResource();
			virtual void BindResource() const;
			virtual void ReleaseResource();

			virtual void SetUniform(const char* name, const void* data, uint32 dataSize);

			uint32 GetSamplerStage(const char* name);
		private:
			void AnalyseUniforms();
			void InternalReleaseResource();

			union 
			{
				void* resource_;
				IDirect3DVertexShader9* vsResource_;
				IDirect3DPixelShader9*  psResource_;
			};
			ID3DXConstantTable* table_;

			struct UniformVar
			{
				uint32 regIndex;
				string name;
				D3DXPARAMETER_TYPE type;
			};
			vector<UniformVar> uniforms_;
			vector<UniformVar> samplers_;
		};
	}
}

#endif
