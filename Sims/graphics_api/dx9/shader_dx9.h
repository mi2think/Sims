/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   10:28
	filename: 	D:\Code\Sims\Sims\graphics_api\dx9\shader_dx9.h
	file path:	D:\Code\Sims\Sims\graphics_api\dx9
	file base:	shader_dx9
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Shader dx9
*********************************************************************/
#ifndef __SHADER_DX_H__
#define __SHADER_DX_H__

#include "sims.h"
#include "graphics_api/rhi/shader_rhi.h"

namespace sims
{
	namespace dx9
	{
		class DX9Shader : public rhi::RHIShader
		{
		public:
			DX9Shader();
			DX9Shader(ShaderType type, const string& source);
			~DX9Shader();

			virtual bool Compile(ShaderType type, const string& source);
			virtual bool LoadBinary(ShaderType type, uint8* byteCode, uint32 byteCodeLength);
		};
	}
}

#endif
