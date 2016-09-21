/********************************************************************
	created:	2016/09/21
	created:	21:9:2016   22:55
	filename: 	D:\Code\Sims\Sims\graphics_api\dx9\geometry_def_dx9.h
	file path:	D:\Code\Sims\Sims\graphics_api\dx9
	file base:	geometry_def_dx9
	file ext:	h
	author:		mi2think@gmail.com

	purpose:	Geometry def
*********************************************************************/
#ifndef __GEOMETRY_DEF_DX9_H__
#define __GEOMETRY_DEF_DX9_H__

#include "sims.h"
#include "math/matrix44.h"
#include "utils/geometry_gen.h"
#include "common_dx9.h"

namespace sims
{
	namespace dx9
	{
		// geometry define, T is Vertex, or VertexN, or VertexUVN
		class GeoBase
		{
		public:
			enum
			{
				OFFSET_P = offsetof(VertexUVN, p),
				OFFSET_N = offsetof(VertexUVN, n),
				OFFSET_UV = offsetof(VertexUVN, uv)
			};
			virtual ~GeoBase() {}
			virtual void Draw(D3DXMATRIX* world, D3DMATERIAL9* mtrl, IDirect3DTexture9* tex) = 0;
			virtual void OnDestory() = 0;
		};

		template<typename T>
		class GeoQuad : public GeoBase
		{
		public:
			GeoQuad() : vb_(nullptr) {}
			GeoQuad(float width, float height, float z, const Matrix44f& m)
				: vb_(nullptr)
			{
				Init(width, height, z, m);
			}

			void Init(float width, float height, float z, const Matrix44f& m)
			{
				int vts = GeometryGen::VT_Position;
				vts |= ((T::FVF & D3DFVF_NORMAL) != 0 ? GeometryGen::VT_Normal : 0);
				vts |= ((T::FVF & D3DFVF_TEX1) != 0 ? GeometryGen::VT_UV : 0);

				T vertices[6];
				GeometryGen::VBDesc vbDesc((uint8*)&vertices[0], sizeof(T), 0, OFFSET_UV, OFFSET_N, OFFSET_P);
				vbDesc.SetTransform(m);
				GeometryGen::GenQuad(width, height, z, vbDesc, vts);

				dx9::CHECK_HR = dx9::g_pD3DD->CreateVertexBuffer(sizeof(vertices),
					D3DUSAGE_WRITEONLY,
					T::FVF,
					D3DPOOL_MANAGED,
					&vb_,
					0);
				T* v = nullptr;
				dx9::CHECK_HR = vb_->Lock(0, 0, (void**)&v, 0);
				memcpy(v, &vertices[0], sizeof(vertices));
				dx9::CHECK_HR = vb_->Unlock();
			}
			void Draw(D3DXMATRIX* world, D3DMATERIAL9* mtrl, IDirect3DTexture9* tex)
			{
				if (world)
					dx9::CHECK_HR = dx9::g_pD3DD->SetTransform(D3DTS_WORLD, world);
				if (mtrl)
					dx9::CHECK_HR = dx9::g_pD3DD->SetMaterial(mtrl);
				if (tex)
					dx9::CHECK_HR = dx9::g_pD3DD->SetTexture(0, tex);

				dx9::CHECK_HR = dx9::g_pD3DD->SetStreamSource(0, vb_, 0, sizeof(T));
				dx9::CHECK_HR = dx9::g_pD3DD->SetFVF(T::FVF);
				dx9::CHECK_HR = dx9::g_pD3DD->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
			}
			void OnDestory() { SAFE_RELEASE(vb_); }
		private:
			IDirect3DVertexBuffer9* vb_;
		};

		template<typename T>
		class GeoPlane : public GeoBase
		{
		public:
			GeoPlane()
				: vb_(nullptr)
				, ib_(nullptr)
			{}
			GeoPlane(float width, float height, uint32 slices, uint32 stacks, const Matrix44f& m)
				: vb_(nullptr)
				, ib_(nullptr)
			{
				Init(width, height, slices, stacks, m);
			}

			void Init(float width, float height, uint32 slices, uint32 stacks, const Matrix44f& m)
			{
				int vts = GeometryGen::VT_Position;
				vts |= ((T::FVF & D3DFVF_NORMAL) != 0 ? GeometryGen::VT_Normal : 0);
				vts |= ((T::FVF & D3DFVF_TEX1) != 0 ? GeometryGen::VT_UV : 0);

				int vertexNum = slices * stacks;
				int indexNum = (stacks - 1) * (slices - 1) * 6;
				T* vertices = new T[vertexNum];
				uint16* indices = new uint16[indexNum];

				GeometryGen::VBDesc vbDesc((uint8*)vertices, sizeof(T), 0, OFFSET_UV, OFFSET_N, OFFSET_P);
				vbDesc.SetTransform(m);
				GeometryGen::GenPlane(width, height, slices, stacks, vbDesc, GeometryGen::IBDesc((uint8*)indices, GeometryGen::IBDesc::Index16), vts);

				dx9::CHECK_HR = dx9::g_pD3DD->CreateVertexBuffer(sizeof(T) * vertexNum,
					D3DUSAGE_WRITEONLY,
					T::FVF,
					D3DPOOL_MANAGED,
					&vb_,
					0);

				Vertex* v = nullptr;
				dx9::CHECK_HR = vb_->Lock(0, 0, (void**)&v, 0);
				memcpy(v, vertices, sizeof(T) * vertexNum);
				dx9::CHECK_HR = vb_->Unlock();

				dx9::CHECK_HR = dx9::g_pD3DD->CreateIndexBuffer(sizeof(uint16) * indexNum,
					D3DUSAGE_WRITEONLY,
					D3DFMT_INDEX16,
					D3DPOOL_MANAGED,
					&ib_,
					0);
				uint16* w = nullptr;
				dx9::CHECK_HR = ib_->Lock(0, 0, (void**)&w, 0);
				memcpy(w, indices, sizeof(uint16) * indexNum);
				dx9::CHECK_HR = ib_->Unlock();

				vertexNum_ = vertexNum;
				triNum_ = indexNum / 3;

				delete[] vertices;
				delete[] indices;
			}

			void Draw(D3DXMATRIX* world, D3DMATERIAL9* mtrl, IDirect3DTexture9* tex)
			{
				if (world)
					dx9::CHECK_HR = dx9::g_pD3DD->SetTransform(D3DTS_WORLD, world);
				if (mtrl)
					dx9::CHECK_HR = dx9::g_pD3DD->SetMaterial(mtrl);
				if (tex)
					dx9::CHECK_HR = dx9::g_pD3DD->SetTexture(0, tex);

				dx9::CHECK_HR = dx9::g_pD3DD->SetStreamSource(0, vb_, 0, sizeof(T));
				dx9::CHECK_HR = dx9::g_pD3DD->SetFVF(T::FVF);
				dx9::CHECK_HR = dx9::g_pD3DD->SetIndices(ib_);
				dx9::CHECK_HR = dx9::g_pD3DD->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexNum_, 0, triNum_);
			}

			void OnDestory() { SAFE_RELEASE(vb_); SAFE_RELEASE(ib_); }
		private:
			IDirect3DVertexBuffer9* vb_;
			IDirect3DIndexBuffer9* ib_;
			uint32 vertexNum_;
			uint32 triNum_;
		};

		template<typename T>
		class GeoBox : public GeoBase
		{
		public:
			GeoBox()
				: vb_(nullptr)
				, ib_(nullptr)
			{}
			GeoBox(float width, float height, float depth, const Matrix44f& m)
				: vb_(nullptr)
				, ib_(nullptr)
			{
				Init(width, height, depth, m);
			}

			void Init(float width, float height, float depth, const Matrix44f& m)
			{
				int vts = GeometryGen::VT_Position;
				vts |= ((T::FVF & D3DFVF_NORMAL) != 0 ? GeometryGen::VT_Normal : 0);
				vts |= ((T::FVF & D3DFVF_TEX1) != 0 ? GeometryGen::VT_UV : 0);

				int vertexNum = 8;
				int indexNum = 36;
				if ((vts & GeometryGen::VT_UV) != 0)
					vertexNum = 24;

				T* vertices = new T[vertexNum];
				uint16* indices = new uint16[indexNum];

				GeometryGen::VBDesc vbDesc((uint8*)vertices, sizeof(T), 0, OFFSET_UV, OFFSET_N, OFFSET_P);
				vbDesc.SetTransform(m);

				GeometryGen::GenBox(width, height, depth, vbDesc, GeometryGen::IBDesc((uint8*)indices, GeometryGen::IBDesc::Index16), vts);

				dx9::CHECK_HR = dx9::g_pD3DD->CreateVertexBuffer(sizeof(T) * vertexNum,
					D3DUSAGE_WRITEONLY,
					T::FVF,
					D3DPOOL_MANAGED,
					&vb_,
					0);

				Vertex* v = nullptr;
				dx9::CHECK_HR = vb_->Lock(0, 0, (void**)&v, 0);
				memcpy(v, vertices, sizeof(T) * vertexNum);
				dx9::CHECK_HR = vb_->Unlock();

				dx9::CHECK_HR = dx9::g_pD3DD->CreateIndexBuffer(sizeof(uint16) * indexNum,
					D3DUSAGE_WRITEONLY,
					D3DFMT_INDEX16,
					D3DPOOL_MANAGED,
					&ib_,
					0);
				uint16* w = nullptr;
				dx9::CHECK_HR = ib_->Lock(0, 0, (void**)&w, 0);
				memcpy(w, indices, sizeof(uint16) * indexNum);
				dx9::CHECK_HR = ib_->Unlock();

				vertexNum_ = vertexNum;

				delete[] vertices;
				delete[] indices;
			}

			void Draw(D3DXMATRIX* world, D3DMATERIAL9* mtrl, IDirect3DTexture9* tex)
			{
				if (world)
					dx9::CHECK_HR = dx9::g_pD3DD->SetTransform(D3DTS_WORLD, world);
				if (mtrl)
					dx9::CHECK_HR = dx9::g_pD3DD->SetMaterial(mtrl);
				if (tex)
					dx9::CHECK_HR = dx9::g_pD3DD->SetTexture(0, tex);

				dx9::CHECK_HR = dx9::g_pD3DD->SetStreamSource(0, vb_, 0, sizeof(T));
				dx9::CHECK_HR = dx9::g_pD3DD->SetFVF(T::FVF);
				dx9::CHECK_HR = dx9::g_pD3DD->SetIndices(ib_);
				dx9::CHECK_HR = dx9::g_pD3DD->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexNum_, 0, 12);
			}

			void OnDestory() { SAFE_RELEASE(vb_); SAFE_RELEASE(ib_); }
		private:
			IDirect3DVertexBuffer9* vb_;
			IDirect3DIndexBuffer9* ib_;
			uint32 vertexNum_;
		};

		template<typename T>
		class GeoSkyBox : public GeoBase
		{
		public:
			GeoSkyBox()
				: vb_(nullptr)
				, ib_(nullptr)
			{}
			GeoSkyBox(float width, float height, float depth, const Matrix44f& m)
				: vb_(nullptr)
				, ib_(nullptr)
			{
				Init(width, height, depth, m);
			}

			void Init(float width, float height, float depth, const Matrix44f& m)
			{
				int vts = GeometryGen::VT_Position;
				vts |= ((T::FVF & D3DFVF_NORMAL) != 0 ? GeometryGen::VT_Normal : 0);
				vts |= ((T::FVF & D3DFVF_TEX1) != 0 ? GeometryGen::VT_UV : 0);

				T vertices[14];
				uint16 indices[36];

				GeometryGen::VBDesc vbDesc((uint8*)&vertices[0], sizeof(T), 0, OFFSET_UV, OFFSET_N, OFFSET_P);
				vbDesc.SetTransform(m);
				GeometryGen::GenSkyBox(width, height, depth, vbDesc, GeometryGen::IBDesc((uint8*)&indices[0], GeometryGen::IBDesc::Index16), vts);

				dx9::CHECK_HR = dx9::g_pD3DD->CreateVertexBuffer(sizeof(vertices),
					D3DUSAGE_WRITEONLY,
					T::FVF,
					D3DPOOL_MANAGED,
					&vb_,
					0);
				T* v = nullptr;
				dx9::CHECK_HR = vb_->Lock(0, 0, (void**)&v, 0);
				memcpy(v, &vertices[0], sizeof(vertices));
				dx9::CHECK_HR = vbWall_->Unlock();

				dx9::CHECK_HR = dx9::g_pD3DD->CreateIndexBuffer(sizeof(indices),
					D3DUSAGE_WRITEONLY,
					D3DFMT_INDEX16,
					D3DPOOL_MANAGED,
					&ib_,
					0);
				uint16* w = nullptr;
				dx9::CHECK_HR = ib_->Lock(0, 0, (void**)&w, 0);
				memcpy(w, &indices[0], sizeof(indices));
				dx9::CHECK_HR = ib_->Unlock();
			}

			void Draw(D3DXMATRIX* world, D3DMATERIAL9* mtrl, IDirect3DTexture9* tex)
			{
				if (world)
					dx9::CHECK_HR = dx9::g_pD3DD->SetTransform(D3DTS_WORLD, world);
				if (mtrl)
					dx9::CHECK_HR = dx9::g_pD3DD->SetMaterial(mtrl);
				if (tex)
					dx9::CHECK_HR = dx9::g_pD3DD->SetTexture(0, tex);

				dx9::CHECK_HR = dx9::g_pD3DD->SetStreamSource(0, vb_, 0, sizeof(T));
				dx9::CHECK_HR = dx9::g_pD3DD->SetFVF(T::FVF);
				dx9::CHECK_HR = dx9::g_pD3DD->SetIndices(ib_);
				dx9::CHECK_HR = dx9::g_pD3DD->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 14, 0, 12);
			}

			void OnDestory() { SAFE_RELEASE(vb_); SAFE_RELEASE(ib_); }
		private:
			IDirect3DVertexBuffer9* vb_;
			IDirect3DIndexBuffer9* ib_;
		};
	}
}


#endif