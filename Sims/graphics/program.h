/********************************************************************
	created:	2016/11/05
	created:	5:11:2016   22:07
	filename: 	D:\Code\Sims\Sims\graphics\program.h
	file path:	D:\Code\Sims\Sims\graphics
	file base:	program
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Program
*********************************************************************/
#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "graphics_fwd.h"
#include "shader.h"

namespace sims
{
	class Program
	{
	public:
		Program();
		virtual ~Program();

		virtual void AttachShader(const ShaderRef& shader);
		virtual void EndAttachShader();
		virtual void BindProgram() = 0;
		virtual void DeleteProgram() = 0;

		// uniforms
		virtual UniformLoc VSGetUniformLoc(const char* name, UniformLoc parent = nullptr) const = 0;
		virtual UniformLoc FSGetUniformLoc(const char* name, UniformLoc parent = nullptr) const = 0;
		virtual void VSBindUniform(UniformLoc loc, const void* data, uint32 count, DataType::Type type) = 0;
		virtual void FSBindUniform(UniformLoc loc, const void* data, uint32 count, DataType::Type type) = 0;
	protected:
		ShaderRef shaders_[ShaderDomain::Max];
	};
}

#endif