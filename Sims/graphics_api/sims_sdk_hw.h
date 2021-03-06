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

#include "graphics/hw/render_resource.h"
#include "hw/device_context.h"
#include "hw/window.h"

#if SIMS_SDK_IMPL_D3D9
	#include "d3d9/window.h"
	typedef sims::hw::Window<sims::d3d9::Window> HWWindow;
#elif SIMS_SDK_IMPL_OGL
	#include "ogl/window.h"
	typedef sims::hw::Window<sims::ogl::Window> HWWindow;
#endif

namespace sims
{
	namespace hw
	{
		// DC
		hw::DeviceContext* GetDeviceContext();

		template<typename ResourceType>
		ResourceType* CreateResource() { ASSERT(false && "Unknow resource type!"); return nullptr; }

		template<> TextureResource*				CreateResource<TextureResource>();
		template<> VertexDeclarationResource*	CreateResource<VertexDeclarationResource>();
		template<> VertexBufferResource*		CreateResource<VertexBufferResource>();
		template<> VertexArrayResource*			CreateResource<VertexArrayResource>();
		template<> IndexBufferResource*			CreateResource<IndexBufferResource>();
		template<> ShaderResource*				CreateResource<ShaderResource>();
		template<> ProgramResource*				CreateResource<ProgramResource>();

		template<typename T>
		void SetUniform(ProgramRef prog, const char* name, const T& data, ShaderDomain::Type type = ShaderDomain::Vertex)
		{
#if SIMS_SDK_IMPL_D3D9
			ShaderRef shader = prog->GetShader(type);
			if (shader)
			{
				shader->Resource().As<ShaderResource>()->SetUniform<T>(name, data);
			}
#elif SIMS_SDK_IMPL_OGL
			prog->Resource().As<ProgramResource>()->SetUniform<T>(name, data);
#endif
		}
	}
}

#endif