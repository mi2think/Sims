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
#include "graphics_api/sims_sdk_hw.h"

namespace sims
{
	Shader::Shader()
		: invalid_(true)
	{}
	Shader::Shader(ShaderDomain::Type type)
		: invalid_(true)
		, type_(type)
	{}
	Shader::Shader(ShaderDomain::Type type, const char* source, const char* entryName)
		: invalid_(true)
		, type_(type)
	{
		SetSource(source);
		SetEntryName(entryName);
	}

	Shader::~Shader()
	{}

	void Shader::SetSource(const char* source)
	{
		source_ = source;
		invalid_ = true;
	}

	void Shader::SetSourceFromFile(const char* filename)
	{
		auto stream = VFS::GetVFS().OpenInputStream(filename);
		ASSERT(stream);
		auto fsize = stream->GetSize();
		source_.resize(fsize + 1);
		stream->Read(&source_[0], fsize);
		source_[fsize] = 0;
		invalid_ = true;
	}

	void Shader::SetType(ShaderDomain::Type type)
	{
		type_ = type;
		invalid_ = true;
	}

	void Shader::SetEntryName(const char* entryName)
	{
		entryName_ = entryName;
		invalid_ = true;
	}

	const char* Shader::GetTypeDesc() const
	{
		if (type_ == ShaderDomain::Vertex)
			return "vertex shader";
		else if (type_ == ShaderDomain::Fragment)
			return "fragment shader";

		return "<Unknown>";
	}

	void Shader::Invalidate()
	{
		if (!invalid_)
			return;
		invalid_ = true;

		IResourceOperation::Invalidate();
	}

	void Shader::Create()
	{
		if (HWResource_)
		{
			Release();
		}

		HWResource_ = hw::CreateResource<ShaderResource>();
	}
}