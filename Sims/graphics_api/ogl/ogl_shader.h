/********************************************************************
	created:	2016/10/23
	created:	23:10:2016   17:58
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_shader.h
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_shader
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	OGL Shader
*********************************************************************/
#ifndef __OGL_SHADER_H__
#define __OGL_SHADER_H__

#include "graphics/shader.h"
#include "ogl_common.h"

namespace sims
{
	namespace ogl
	{
		class OGLShader : public Shader
		{
		public:
			OGLShader();
			~OGLShader();

			virtual bool IsValid() const;
			virtual bool Compile(ShaderDomain::Type type, const string& source);
			virtual bool LoadBinary(ShaderDomain::Type type, uint8* byteCode, uint32 byteCodeLength);
			virtual void Delete();
			virtual void Use();

			virtual UniformLoc GetUniformLoc(const char* name);
			virtual UniformLoc GetUniformLoc(UniformLoc parent, const char* name);
		private:
			GLuint so_; // shader object
		};
	}
}

#endif