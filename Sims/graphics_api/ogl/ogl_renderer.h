/********************************************************************
	created:	2016/10/27
	created:	27:10:2016   16:12
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\ogl_renderer.h
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	ogl_renderer
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	OGL Renderer
*********************************************************************/
#ifndef __OGL_RENDERER_H__
#define __OGL_RENDERER_H__

#include "graphics_api/hw/hw_renderer.h"
#include "math/matrix44.h"

namespace sims
{
	namespace ogl
	{
		class OGLRenderer : public hw::HWRenderer
		{
		public:
			virtual void BeginFrame(uint32 clearFlags, Color color, float depth, uint32 stencil);
			virtual void EndFrame();
			virtual void PresentFrame();

			virtual void SetTransform(Transform::Type type, const Matrix44f& matrix);
			virtual const Matrix44f& GetTransform(Transform::Type type) const;

			virtual void DrawPrimitive(PrimitiveType::Type type, uint32 primitiveCount);
			virtual void DrawIndexedPrimitive(PrimitiveType::Type type, const IndexBufferResource& ib, uint32 vertexCount, uint32 primitiveCount);
		private:
			Matrix44f matrixs_[Transform::Max];
		};
	}
}

#endif