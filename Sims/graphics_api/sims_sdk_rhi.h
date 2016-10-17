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

#include "rhi/renderer_rhi.h"
#include "rhi/shader_rhi.h"

// rhi implement by ?
#define SIMS_SDK_IMPL_DX9 1

namespace sims
{
	namespace rhi
	{
		// renderer
		RHIRenderer* GetRenderer();
		// shader
		RHIShader* CreateShader();
		RHIShader* CreateShader(ShaderType type, const string& source);
	}
}

#endif