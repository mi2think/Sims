/********************************************************************
	created:	2016/10/11
	created:	11:10:2016   11:37
	filename: 	D:\Code\Sims\Sims\graphics_api\dx11\geometry_def_dx11.h
	file path:	D:\Code\Sims\Sims\graphics_api\dx11
	file base:	geometry_def_dx11
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Geometry def
*********************************************************************/
#ifndef __GEOMETRY_DEF_DX11_H__
#define __GEOMETRY_DEF_DX11_H__

#include "sims.h"
#include "math/matrix44.h"
#include "utils/geometry_gen.h"
#include "common_dx11.h"

namespace sims
{
	namespace dx11
	{
		// geometry define, T is Vertex, or VertexN, or VertexUV, or VertexUVN

		class GeoBase
		{
		public:
			GeoBase(uint8* pIAInputSignature, uint32 IAInputSignatureSize)
				: pIAInputSignature_(pIAInputSignature)
				, IAInputSignatureSize_(IAInputSignatureSize)
			{}
			virtual ~GeoBase() {}
			virtual void Draw() = 0;
			virtual void OnDestory() = 0;
		protected:
			uint8* pIAInputSignature_;
			uint32 IAInputSignatureSize_;
		};

		template<typename T>
		class GeoBox : public GeoBase
		{
		public:
			GeoBox(uint8* pIAInputSignature, uint32 IAInputSignatureSize)
				: GeoBase(pIAInputSignature, IAInputSignatureSize)
				: vb_(nullptr)
				, ib_(nullptr)
				, inputLayout_(nullptr)
				, vertexNum_(0)
				, indexNum_(0)
			{}

			void Init(float width, float height, float depth, const Matrix44f& m, const std::function<void(T*, uint32)>& func = {})
			{
				// gen vertices and indices
				int vertexNum = 8;
				int indexNum = 36;
				int vts = GeometryGen::VT_Position;
				vts |= (Offset<T>::N  != 0 ? GeometryGen::VT_Normal : 0);
				vts |= (Offset<T>::UV != 0 ? GeometryGen::VT_UV : 0);
				if ((vts & GeometryGen::VT_UV) != 0)
					vertexNum = 24;

				T* vertices = new T[vertexNum];
				uint16* indices = new uint16[indexNum];

				GeometryGen::VBDesc vbDesc((uint8*)vertices, sizeof(T), 0, Offset<T>::UV, Offset<T>::N, Offset<T>::P);
				vbDesc.SetTransform(m);
				GeometryGen::GenBox(width, height, depth, vbDesc, GeometryGen::IBDesc((uint8*)indices, GeometryGen::IBDesc::Index16), vts);

				// a chance for modify vertices
				if (func)
				{
					func(vertices, vertexNum);
				}

				// create vertex buffer and index buffer
				D3D11_BUFFER_DESC vbd;
				vbd.Usage = D3D11_USAGE_IMMUTABLE;
				vbd.ByteWidth = sizeof(T) * vertexNum;
				vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				vbd.CPUAccessFlags = 0;
				vbd.MiscFlags = 0;
				vbd.StructureByteStride = 0;
				D3D11_SUBRESOURCE_DATA vInitData;
				vInitData.pSysMem = vertices;
				CHECK_HR = g_pD3DD->CreateBuffer(&vbd, &vInitData, &vb_);

				D3D11_BUFFER_DESC ibd;
				ibd.Usage = D3D11_USAGE_IMMUTABLE;
				ibd.ByteWidth = sizeof(uint16) * indexNum;
				ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				ibd.CPUAccessFlags = 0;
				ibd.MiscFlags = 0;
				ibd.StructureByteStride = 0;
				D3D11_SUBRESOURCE_DATA iInitData;
				iInitData.pSysMem = indices;
				CHECK_HR = g_pD3DD->CreateBuffer(&ibd, &iInitData, &ib_);

				// build vertex input layout
				CHECK_HR = g_pD3DD->CreateInputLayout(T::GetDesc(),
					T::GetDescNum(),
					pIAInputSignature_,
					IAInputSignatureSize_,
					inputLayout_);

				vertexNum_ = vertexNum;
				indexNum_ = indexNum;

				delete[] vertices;
				delete[] indices;
			}

			void Draw()
			{
				g_pD3DDC->IASetInputLayout(inputLayout_);
				g_pD3DDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				g_pD3DDC->IASetIndexBuffer(ib_, DXGI_FORMAT_R16_UINT, 0);
				UINT stride = sizeof(T);
				UINT offset = 0;
				g_pD3DDC->IASetVertexBuffers(0, 1, &vb_, &stride, &offset);

				g_pD3DDC->DrawIndexed(indexNum_, 0, 0);
			}

			void OnDestory()
			{ 
				SAFE_RELEASE(vb_); 
				SAFE_RELEASE(ib_);
				SAFE_RELEASE(inputLayout_);
			}
		private:
			ID3D11Buffer* vb_;
			ID3D11Buffer* ib_;
			ID3D11InputLayout* inputLayout_;
			uint32 vertexNum_;
			uint32 indexNum_;
		};


	}
}


#endif
