/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   2:48
	filename: 	D:\Code\Sims\Sims\graphics_api\sims_sdk_rhi.h
	file path:	D:\Code\Sims\Sims\graphics_api
	file base:	sims_sdk_rhi
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Sims SDK rhi 
*********************************************************************/
#ifndef __SIMS_SDK_RHI_H__
#define __SIMS_SDK_RHI_H__

#include "graphics/render_resource.h"
#include "rhi/rhi_renderer.h"

// rhi implement by ?
#define SIMS_SDK_IMPL_D3D9 1

namespace sims
{
	namespace rhi
	{
		// renderer
		RHIRenderer* GetRenderer();

		template<typename ResourceType>
		ResourceType* CreateResource() { ASSERT(false && "Unknow resource type!"); return nullptr; }

		template<> TextureResource*         CreateResource<TextureResource>();
		template<> VertexBufferResource*    CreateResource<VertexBufferResource>();
		template<> IndexBufferResource*     CreateResource<IndexBufferResource>();
		template<> ShaderResource*          CreateResource<ShaderResource>();
	}
}

#endif