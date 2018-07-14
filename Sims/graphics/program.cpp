/********************************************************************
	created:	2016/11/05
	created:	5:11:2016   22:10
	filename: 	D:\Code\Sims\Sims\graphics\program.cpp
	file path:	D:\Code\Sims\Sims\graphics
	file base:	program
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Program
*********************************************************************/
#include "program.h"
#include "graphics_api/sims_sdk_hw.h"

namespace sims
{
	Program::Program()
		: desc_("<Unknown>")
		, invalid_(true)
	{
	}

	Program::Program(const string& desc)
		: desc_(desc)
		, invalid_(true)
	{}

	Program::~Program()
	{
	}

	void Program::AddShader(const ShaderRef& shader)
	{
		if (shaders_[shader->GetType()] != nullptr)
			LOG_WARN("Program %s has changed %s", desc_.c_str());

		shaders_[shader->GetType()] = shader;
		invalid_ = true;
	}

	const ShaderRef& Program::GetShader(ShaderDomain::Type type) const
	{
		return shaders_[type];
	}

	void Program::Invalidate()
	{
		if (!invalid_)
			return;
		invalid_ = false;

		IResourceOperation::Invalidate();
	}

	void Program::Create()
	{
		if (HWResource_)
		{
			Release();
		}

		HWResource_ = hw::CreateResource<ProgramResource>();
	}
}