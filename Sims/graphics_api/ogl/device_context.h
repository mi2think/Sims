/********************************************************************
	created:	2016/10/27
	created:	27:10:2016   16:12
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\device_context.h
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	device_context
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	OGL Device Context
*********************************************************************/
#ifndef __OGL_RENDERER_H__
#define __OGL_RENDERER_H__

#include "graphics_api/hw/device_context.h"
#include "math/matrix44.h"

namespace sims
{
	namespace ogl
	{
		class OGLDeviceContext : public hw::DeviceContext
		{
		public:
			virtual void BeginScene();
			virtual void EndScene();
			virtual void Clear(uint32 clearFlags, Color color, float depth, uint32 stencil);
			virtual void Present();

			virtual void DrawPrimitive(PrimitiveType::Type type, uint32 primitiveCount);
			virtual void DrawIndexedPrimitive(PrimitiveType::Type type, const RenderResourceRef& ib, uint32 vertexCount, uint32 primitiveCount);

			// Legacy
			virtual void SetTransform(Transform::Type type, const Matrix44f& matrix);
			virtual const Matrix44f& GetTransform(Transform::Type type) const;
		private:
			Matrix44f matrixs_[Transform::Max];
		};
	}
}

#endif