/********************************************************************
	created:	2016/10/27
	created:	27:10:2016   16:16
	filename: 	D:\Code\Sims\Sims\graphics_api\ogl\device_context.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\ogl
	file base:	device_context
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	OGL Device Context
*********************************************************************/
#include "device_context.h"
#include "common.h"
#include "graphics/hw/texture.h"

namespace sims
{
	namespace ogl
	{
		void OGLDeviceContext::BeginScene()
		{
		}

		void OGLDeviceContext::EndScene()
		{
		}

		void OGLDeviceContext::Clear(uint32 clearFlags, Color color, float depth, uint32 stencil)
		{
			uint32 flags = 0;
			if ((clearFlags & ClearFlags::Color) != 0)
			{
				auto c = color.GetRGBAVector4();
				glClearColor(c.x, c.y, c.z, c.w);
				flags |= GL_COLOR_BUFFER_BIT;
			}
			if ((clearFlags & ClearFlags::Depth) != 0)
			{
				glClearDepth(depth);
				flags |= GL_DEPTH_BUFFER_BIT;
			}
			if ((clearFlags & ClearFlags::Stencil) != 0)
			{
				glClearStencil(stencil);
				flags |= GL_STENCIL_BUFFER_BIT;
			}
			glClear(flags);
		}

		void OGLDeviceContext::Present()
		{
			glut_swap_buffers();
		}

		void OGLDeviceContext::DrawPrimitive(PrimitiveType::Type type, uint32 primitiveCount)
		{
			glDrawArrays(ToGLPrimitiveType(type), 0, primitiveCount * GetVertexNumPerPrimitive(type));

			gl_check_error("DrawPrimitive");
		}

		void OGLDeviceContext::DrawIndexedPrimitive(PrimitiveType::Type type, const RenderResourceRef& ib, uint32 vertexCount, uint32 primitiveCount)
		{
			ib->BindResource();
			glDrawElements(ToGLPrimitiveType(type), primitiveCount * GetVertexNumPerPrimitive(type), GL_UNSIGNED_SHORT, 0);

			gl_check_error("DrawIndexedPrimitive");
		}

		void OGLDeviceContext::SetTransform(Transform::Type type, const Matrix44f& matrix)
		{
			ASSERT(type < Transform::Max);
			matrixs_[type] = matrix;

			if (type == Transform::World || type == Transform::View)
			{
				Matrix44f worldView;
				MatrixMultiply(worldView, matrixs_[Transform::World], matrixs_[Transform::View]);

				glMatrixMode(GL_MODELVIEW);
				glLoadMatrixf((const GLfloat*)&worldView.m[0]);
			}
			else if (type == Transform::Projection)
			{
				glMatrixMode(GL_PROJECTION);
				glLoadMatrixf((const GLfloat*)&matrixs_[type].m[0]);
			}
		}

		const Matrix44f& OGLDeviceContext::GetTransform(Transform::Type type) const
		{
			ASSERT(type < Transform::Max);
			return matrixs_[type];
		}
	}
}