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
#elif SIMS_SDK_IMPL_OGL
	#include "sims_sdk_ogl.h"
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

		template<> VertexDeclarationResource* CreateResource<VertexDeclarationResource>()
		{
			return new d3d9::D3D9VertexDeclarationResource();
		}

		template<> VertexBufferResource* CreateResource<VertexBufferResource>()
		{
			return new d3d9::D3D9VertexBufferResource();
		}

		template<> VertexArrayResource* CreateResource<VertexArrayResource>()
		{
			return new d3d9::D3D9VertexArrayResource();
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
#elif SIMS_SDK_IMPL_OGL
		HWRenderer* GetRenderer()
		{
			static ogl::OGLRenderer s_renderer;
			return &s_renderer;
		}

		template<> TextureResource* CreateResource<TextureResource>()
		{
			return new ogl::OGLTextureResource();
		}

		template<> VertexDeclarationResource* CreateResource<VertexDeclarationResource>()
		{
			// dummy
			return new VertexDeclarationResource();
		}

		template<> VertexBufferResource* CreateResource<VertexBufferResource>()
		{
			return new ogl::OGLVertexBufferResource();
		}

		template<> VertexArrayResource* CreateResource<VertexArrayResource>()
		{
			return new ogl::OGLVertexArrayResource();
		}

		template<> IndexBufferResource* CreateResource<IndexBufferResource>()
		{
			return new ogl::OGLIndexBufferResource();
		}

		template<> ShaderResource* CreateResource<ShaderResource>()
		{
			return new ogl::OGLShaderResource();
		}

		template<> ProgramResource* CreateResource<ProgramResource>()
		{
			return new ogl::OGLProgramResource();
		}
#endif
	}
}