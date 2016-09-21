/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   22:27
	filename: 	D:\Code\Sims\Sims\graphics_api\renderer_rhi.h
	file path:	D:\Code\Sims\Sims\graphics_api
	file base:	renderer_rhi
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Renderer RHI
*********************************************************************/
#ifndef __RENDERER_RHI_H__
#define __RENDERER_RHI_H__

#include "sims.h"

namespace sims
{
	class RHIRenderer
	{
	public:
		static RHIRenderer* GetRenderer();

		virtual ~RHIRenderer() {}
	};
}

#endif