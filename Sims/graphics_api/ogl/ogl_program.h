/********************************************************************
	created:	2016/11/07
	created:	7:11:2016   22:51
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_program.h
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_program
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	OGL Program
*********************************************************************/
#ifndef __OGL_PROGRAM_H__
#define __OGL_PROGRAM_H__

#include "graphics/program.h"
#include "ogl_common.h"

namespace sims
{
	namespace ogl
	{
		class OGLProgram : public Program
		{
		public:
			OGLProgram();
			~OGLProgram();

			virtual void AttachShader(const ShaderRef& shader);
			virtual void EndAttachShader();
			virtual void BindProgram();
			virtual void DeleteProgram();

			virtual UniformLoc VSGetUniformLoc(const char* name, UniformLoc parent = nullptr) const;
			virtual UniformLoc FSGetUniformLoc(const char* name, UniformLoc parent = nullptr) const;
			virtual void VSBindUniform(UniformLoc loc, const void* data, uint32 count, UniformDataType::Type type);
			virtual void FSBindUniform(UniformLoc loc, const void* data, uint32 count, UniformDataType::Type type);
		private:
			void DeleteProgramInternal();
			UniformLoc GetUniformLoc(const char* name) const;
			void BindUniform(UniformLoc loc, const void* data, uint32 count, UniformDataType::Type type);

			GLuint prog_;
		};
	}
}


#endif