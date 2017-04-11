/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   2:48
	filename: 	D:\Code\Sims\Sims\graphics_api\sims_sdk_hw.h
	file path:	D:\Code\Sims\Sims\graphics_api
	file base:	sims_sdk_hw
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Sims SDK hardware 
*********************************************************************/
#ifndef __SIMS_SDK_RHI_H__
#define __SIMS_SDK_RHI_H__

#include "graphics/render_resource.h"
#include "hw/hw_renderer.h"

namespace sims
{
	namespace hw
	{
		// renderer
		HWRenderer* GetRenderer();

		template<typename ResourceType>
		ResourceType* CreateResource() { ASSERT(false && "Unknow resource type!"); return nullptr; }

		template<> TextureResource*         CreateResource<TextureResource>();
		template<> VertexBufferResource*    CreateResource<VertexBufferResource>();
		template<> IndexBufferResource*     CreateResource<IndexBufferResource>();
		template<> ShaderResource*          CreateResource<ShaderResource>();
		template<> ProgramResource*			CreateResource<ProgramResource>();
	}
}

#endif