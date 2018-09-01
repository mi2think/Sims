/********************************************************************
created:	2016/09/21
created:	21:9:2016   22:27
filename: 	D:\Code\Sims\Sims\graphics_api\hw\device_context.h
file path:	D:\Code\Sims\Sims\graphics_api\hw
file base:	device_context
file ext:	h
author:		mi2think@gmail.com

purpose:	Device Context
*********************************************************************/
#pragma once

#include "graphics/graphics_fwd.h"

namespace sims
{
	namespace hw
	{
		class DeviceContext
		{
		public:
			virtual ~DeviceContext() {}

			virtual void BeginScene() = 0;
			virtual void EndScene() = 0;
			virtual void Clear(uint32 clearFlags, Color color, float depth, uint32 stencil) = 0;
			virtual void Present() = 0;

			virtual void DrawPrimitive(PrimitiveType::Type type, uint32 primitiveCount) = 0;
			virtual void DrawIndexedPrimitive(PrimitiveType::Type type, const RenderResourceRef& ib, uint32 vertexCount, uint32 primitiveCount) = 0;

			// Legacy
			virtual void SetTransform(Transform::Type type, const Matrix44f& matrix) = 0;
			virtual const Matrix44f& GetTransform(Transform::Type type) const = 0;
		};
	}
}
