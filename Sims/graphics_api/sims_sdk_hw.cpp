/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   2:51
	filename: 	D:\Code\Sims\Sims\graphics_api\sims_sdk_hw.cpp
	file path:	D:\Code\Sims\Sims\graphics_api
	file base:	sims_sdk_hw
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Sims SDK hardware
*********************************************************************/
#include "sims_sdk_hw.h"



#if SIMS_SDK_IMPL_D3D9
#include "sims_sdk_d3d9.h"
#endif

namespace sims
{
	namespace hw
	{
#if SIMS_SDK_IMPL_D3D9
			HWRenderer* GetRenderer()
			{
				static d3d9::D3D9Renderer s_renderer;
				return &s_renderer;
			}

			template<> TextureResource* CreateResource<TextureResource>()
			{
				return new d3d9::D3D9TextureResource();
			}

			template<> VertexBufferResource* CreateResource<VertexBufferResource>()
			{
				return new d3d9::D3D9VertexBufferResource();
			}

			template<> IndexBufferResource* CreateResource<IndexBufferResource>()
			{
				return new d3d9::D3D9IndexBufferResource();
			}

			template<> ShaderResource* CreateResource<ShaderResource>()
			{
				return new d3d9::D3D9ShaderResource();
			}

			template<> ProgramResource* CreateResource<ProgramResource>()
			{
				return new d3d9::D3D9ProgramResource();
			}
#endif
	}
}