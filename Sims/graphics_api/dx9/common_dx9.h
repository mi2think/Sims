/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   22:48
	filename: 	D:\Code\Sims\Sims\graphics_api\dx9\common_dx9.h
	file path:	D:\Code\Sims\Sims\graphics_api\dx9
	file base:	common_dx9
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Common def
*********************************************************************/
#ifndef __COMMON_DX9_H__
#define __COMMON_DX9_H__

#include "sims.h"
#include "math/vector2.h"
#include "math/vector3.h"

#include <d3dx9.h>

#define red    D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)
#define green  D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)
#define blue   D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)
#define white  D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define black  D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)
#define yellow D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)

#define GenMaterial(m, a, d, s, e, p) { m.Ambient = a; m.Diffuse = d, m.Specular = s; m.Emissive = e; m.Power = p; }

namespace sims
{
	namespace dx9
	{
		// error checker
		class DXErrorChecker
		{
		public:
			DXErrorChecker(const char* file, uint32 line);
			~DXErrorChecker();

			DXErrorChecker& operator=(HRESULT hr);
		private:
			const char* file_;
			uint32 line_;
			HRESULT hr_;
		};

#define CHECK_HR DXErrorChecker(__FILE__, __LINE__)

		// common functions
		const char* D3DFormat2String(D3DFORMAT format);

		void PrintDisplayMode();
		void PrintAdapterCount();
		void PrintAdapterIdentifier();

		// vertex define
		struct Vertex
		{
			Vector3f p;
			Vertex() = default;
			Vertex(const Vector3f& _p)
				: p(_p)
			{}
			static const uint32 FVF = D3DFVF_XYZ;
		};

		struct VertexN
		{
			Vector3f p;
			Vector3f n;
			VertexN() = default;
			VertexN(const Vector3f& _p, const Vector3f& _n)
				: p(_p)
				, n(_n)
			{}
			static const uint32 FVF = D3DFVF_XYZ | D3DFVF_NORMAL;
		};

		struct VertexUVN
		{
			Vector3f p;
			Vector3f n;
			Vector2f uv;
			VertexUVN() = default;
			VertexUVN(const Vector3f& _p, const Vector3f& _n, const Vector2f& _uv)
				: p(_p)
				, n(_n)
				, uv(_uv)
			{}
			static const uint32 FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
		};
	}
}


#endif