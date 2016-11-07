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

		virtual void HWAttachShader(const ShaderRef&) {}
		virtual void HWBindProgram() {}
	private:
		ShaderRef shaders_[ShaderDomain::Max];
	};
}

#endif