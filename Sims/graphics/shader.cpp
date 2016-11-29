/********************************************************************
	created:	2016/11/06
	created:	6:11:2016   23:19
	filename: 	D:\Code\Sims\Sims\graphics\shader.cpp
	file path:	D:\Code\Sims\Sims\graphics
	file base:	shader
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Shader
*********************************************************************/
#include "shader.h"
#include "core/vfs.h"

namespace sims
{
	Shader::Shader()
	{}

	Shader::~Shader()
	{}

	bool Shader::CompileFromFile(ShaderDomain::Type type, const string& filename)
	{
		auto stream = VFS::GetVFS().OpenInputStream(filename);
		auto fsize = stream->GetSize();
		string source(fsize, '\0');
		stream->Read(&source[0], fsize);

		return Compile(type, source);
	}

	bool Shader::LoadBinaryFromFile(ShaderDomain::Type type, const string& filename)
	{
		auto stream = VFS::GetVFS().OpenInputStream(filename);
		Buffer byteCode = stream->Read();

		return LoadBinary(type, byteCode.GetData(), byteCode.GetSize());
	}
}