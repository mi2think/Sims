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
		class OGLProgramResource : public ProgramResource
		{
		public:
			OGLProgramResource();
			~OGLProgramResource();

			virtual void UpdateResource();
			virtual void BindResource() const;
			virtual void ReleaseResource();

		private:
			void InternalReleaseResource();

			GLuint prog_;
		};
	}
}


#endif