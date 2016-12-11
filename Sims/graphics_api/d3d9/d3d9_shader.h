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
			virtual void BindResource();
			virtual void ReleaseResource();
		private:
			void* resource_;
			ID3DXConstantTable* table_;
		};
	}
}

#endif
