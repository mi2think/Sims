/********************************************************************
	created:	2016/11/08
	created:	8:11:2016   14:21
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\d3d9_program.h
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	d3d9_program
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	D3D9 Program
*********************************************************************/
#ifndef __D3D9_PROGRAM_H__
#define __D3D9_PROGRAM_H__

#include "graphics/render_resource.h"
#include "d3d9_common.h"

namespace sims
{
	namespace d3d9
	{
		class D3D9ProgramResource : public ProgramResource
		{
		public:
			D3D9ProgramResource();
			~D3D9ProgramResource();

			virtual void UpdateResource();
			virtual void BindResource() const;
			virtual void ReleaseResource();
		};
	}
}

#endif