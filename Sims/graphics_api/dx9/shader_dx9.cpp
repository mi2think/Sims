/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   10:42
	filename: 	D:\Code\Sims\Sims\graphics_api\dx9\shader_dx9.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\dx9
	file base:	shader_dx9
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Shader dx9
*********************************************************************/
#include "shader_dx9.h"

namespace sims
{
	namespace dx9
	{
		DX9Shader::DX9Shader()
			: RHIShader()
		{}

		DX9Shader::DX9Shader(ShaderType type, const string& source)
			: RHIShader(type, source)
		{}

		DX9Shader::~DX9Shader()
		{

		}

		bool DX9Shader::Compile(ShaderType type, const string& source)
		{
			return true;
		}

		bool DX9Shader::LoadBinary(ShaderType type, uint8* byteCode, uint32 byteCodeLength)
		{
			return true;
		}
	}
}