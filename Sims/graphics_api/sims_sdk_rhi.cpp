/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   2:51
	filename: 	D:\Code\Sims\Sims\graphics_api\sims_sdk_rhi.cpp
	file path:	D:\Code\Sims\Sims\graphics_api
	file base:	sims_sdk_rhi
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Sims SDK rhi
*********************************************************************/
#include "sims_sdk_rhi.h"

#if SIMS_SDK_IMPL_D3D9
#include "sims_sdk_d3d9.h"
#endif

namespace sims
{
	namespace rhi
	{
#if SIMS_SDK_IMPL_D3D9
			RHIRenderer* GetRenderer()
			{
				static d3d9::D3D9Renderer s_renderer;
				return &s_renderer;
			}

			//RHIShader* CreateShader()
			//{
			//	return new d3d9::D3D9Shader();
			//}

			//RHIShader* CreateShader(ShaderDomain::Type type, const string& source)
			//{
			//	RHIShader* p = new d3d9::D3D9Shader();
			//	p->Compile(type, source);
			//	return p;
			//}

			template<> TextureResource* CreateResource<TextureResource>()
			{
				return new d3d9::D3DTextureResource();
			}

			template<> VertexBufferResource* CreateResource<VertexBufferResource>()
			{
				return new d3d9::D3D9VertexBufferResource();
			}

			template<> IndexBufferResource* CreateResource<IndexBufferResource>()
			{
				return new d3d9::D3D9IndexBufferResource();
			}
#endif
	}
}