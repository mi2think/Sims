/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   2:40
	filename: 	D:\Code\Sims\Sims\graphics_api\rhi\rhi_shader.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\rhi
	file base:	rhi_shader
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Shader
*********************************************************************/
#include "rhi_shader.h"
#include "core/vfs.h"

namespace sims
{
	namespace rhi
	{
		RHIShader::RHIShader()
			: id_(0)
		{}

		RHIShader::~RHIShader()
		{}

		bool RHIShader::CompileFromFile(ShaderDomain::Type type, const string& filename)
		{
			auto stream = VFS::GetVFS().OpenInputStream(filename);
			auto fsize = stream->GetSize();
			string source(fsize, '\0');
			stream->Read((uint8*)&source[0], fsize);

			return Compile(type, source);
		}

		bool RHIShader::LoadBinaryFromFile(ShaderDomain::Type type, const string& filename)
		{
			auto stream = VFS::GetVFS().OpenInputStream(filename);
			vector<uint8> byteCode = stream->Read();

			return LoadBinary(type, &byteCode[0], byteCode.size());
		}
	}
}