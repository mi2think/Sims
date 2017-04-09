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
	{}
	Shader::Shader(ShaderDomain::Type type)
		: type_(type)
	{}

	Shader::~Shader()
	{}

	void Shader::SetSource(const char* source)
	{
		source_ = Buffer(source, strlen(source));
	}

	void Shader::SetSourceFromFile(const char* filename)
	{
		auto stream = VFS::GetVFS().OpenInputStream(filename);
		auto fsize = stream->GetSize();
		source_.Resize(fsize);
		stream->Read(&source_[0], fsize);
	}

	void Shader::SetType(ShaderDomain::Type type)
	{
		type_ = type;
	}

	void Shader::SetEntryName(const char* entryName)
	{
		entryName_ = Buffer(entryName, strlen(entryName));
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
		if ((storageFlags_ & StorageFlags::Hardware) == 0)
			return;

		// update shader
		if (!HWResource_)
			HWResource_ = hw::CreateResource<ShaderResource>();

		HWResource_->Attach(this);
		HWResource_->UpdateResource();
	}
}