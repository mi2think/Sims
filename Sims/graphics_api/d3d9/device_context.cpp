/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   23:13
	filename: 	D:\Code\Sims\Sims\graphics_api\d3d9\device_context.cpp
	file path:	D:\Code\Sims\Sims\graphics_api\d3d9
	file base:	device_context
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	D3D9 Device Context
*********************************************************************/
#include "device_context.h"
#include "common.h"
#include "graphics/hw/texture.h"

namespace sims
{
	namespace d3d9
	{
		void D3D9DeviceContext::BeginScene()
		{
			VERIFYD3DRESULT(g_pD3DD->BeginScene());
		}

		void D3D9DeviceContext::EndScene()
		{
			VERIFYD3DRESULT(g_pD3DD->EndScene());
		}

		void D3D9DeviceContext::Clear(uint32 clearFlags, Color color, float depth, uint32 stencil)
		{
			DWORD flags = 0;
			if ((clearFlags & ClearFlags::Color) != 0)
				flags |= D3DCLEAR_TARGET;
			if ((clearFlags & ClearFlags::Depth) != 0)
				flags |= D3DCLEAR_ZBUFFER;
			if ((clearFlags & ClearFlags::Stencil) != 0)
				flags |= D3DCLEAR_STENCIL;

			VERIFYD3DRESULT(g_pD3DD->Clear(0, 0, flags, color.value, depth, stencil));

		}

		void D3D9DeviceContext::Present()
		{
			VERIFYD3DRESULT(g_pD3DD->Present(0, 0, 0, 0));
		}

		void D3D9DeviceContext::DrawPrimitive(PrimitiveType::Type type, uint32 primitiveCount)
		{
			VERIFYD3DRESULT(g_pD3DD->DrawPrimitive(ToD3DPrimitiveType(type), 0, primitiveCount));
		}

		void D3D9DeviceContext::DrawIndexedPrimitive(PrimitiveType::Type type, const RenderResourceRef& ib, uint32 vertexCount, uint32 primitiveCount)
		{
			ib->BindResource();
			VERIFYD3DRESULT(g_pD3DD->DrawIndexedPrimitive(ToD3DPrimitiveType(type), 0, 0, vertexCount, 0, primitiveCount));
		}

		void D3D9DeviceContext::SetTransform(Transform::Type type, const Matrix44f& matrix)
		{
			ASSERT(type < Transform::Max);
			matrixs_[type] = matrix;

			switch (type)
			{
			case Transform::World:
				VERIFYD3DRESULT(g_pD3DD->SetTransform(D3DTS_WORLD, (const D3DXMATRIX*)&matrix.m));
				break;
			case Transform::View:
				VERIFYD3DRESULT(g_pD3DD->SetTransform(D3DTS_VIEW, (const D3DXMATRIX*)&matrix.m));
				break;
			case Transform::Projection:
				VERIFYD3DRESULT(g_pD3DD->SetTransform(D3DTS_PROJECTION, (const D3DXMATRIX*)&matrix.m));
				break;
			default:
				break;
			}
		}

		const Matrix44f& D3D9DeviceContext::GetTransform(Transform::Type type) const
		{
			ASSERT(type < Transform::Max);
			return matrixs_[type];
		}
	}
}