/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   2:40
	filename: 	D:\Code\Sims\Sims\graphics_api\rhi\shader_rhi.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\rhi
	file base:	shader_rhi
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Shader
*********************************************************************/
#include "shader_rhi.h"

namespace sims
{
	RHIShader::RHIShader()
		: id_(0)
	{}

	RHIShader::RHIShader(ShaderType type, const string& source)
		: id_(0)
	{
		Compile(type, source);
	}
}