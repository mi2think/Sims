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

namespace sims
{
	Program::Program()
	{
	}

	Program::~Program()
	{
	}

	void Program::AttachShader(const ShaderRef& shader)
	{
		shaders_[shader->GetType()] = shader;
	}

	void Program::EndAttachShader()
	{
	}
}