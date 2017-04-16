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

#include "graphics/render_resource.h"
#include "ogl_common.h"

namespace sims
{
	namespace ogl
	{
		class OGLShaderResource : public ShaderResource
		{
		public:
			OGLShaderResource();
			~OGLShaderResource();

			virtual void UpdateResource();
			virtual void BindResource() const;
			virtual void ReleaseResource();

			GLuint GetResource() const { return resource_; }
		private:
			GLuint resource_;
		};
	}
}

#endif