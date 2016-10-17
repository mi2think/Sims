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

#if SIMS_SDK_IMPL_DX9
#include "dx9/renderer_dx9.h"
#endif

namespace sims
{
	namespace rhi
	{
#if SIMS_SDK_IMPL_DX9
			RHIRenderer* GetRenderer()
			{
				static dx9::DX9Renderer s_renderer;
				return &s_renderer;
			}

			RHIShader* CreateShader()
			{
			}

			RHIShader* CreateShader(ShaderType type, const string& source)
			{

			}
#endif
	}
}