/********************************************************************
	created:	2016/09/10
	created:	10:9:2016   0:48
	filename: 	D:\Code\Sims\Sims\utils\geometry_gen.cpp
	file path:	D:\Code\Sims\Sims\utils
	file base:	geometry_gen
	file ext:	cpp
	author:		mi2think@gmail.com

	purpose:	Geometry Generate
*********************************************************************/
#include "geometry_gen.h"
#include "math/vector2.h"
#include "math/vector3.h"

namespace sims
{
	void GeometryGen::GenNormals(uint32 vertexNum,
		uint32 triNum,
		const VBDesc& vbDesc,
		const IBDesc& ibDesc)
	{
		uint8* vb = vbDesc.vb + vbDesc.offset;
		uint16 posOffset = vbDesc.voffsets[VT_Position];
		uint16 normalOffset = vbDesc.voffsets[VT_Normal];

		uint8* ib = ibDesc.ib + ibDesc.offset;
		uint32* count = new uint32[vertexNum];
		memset(count, 0, vertexNum * sizeof(uint32));

		if (ibDesc.indexType == IBDesc::Index16)
		{
			uint16 triStride = 3 * sizeof(uint16);

			// compute normal
			for (uint32 i = 0; i < triNum; ++i)
			{
				uint32 offset = i * triStride;
				TriIndex16* p = (TriIndex16*)(ib + offset);

				// vertex
				uint8* v0 = vb + (p->i0 * vbDesc.stride);
				uint8* v1 = vb + (p->i1 * vbDesc.stride);
				uint8* v2 = vb + (p->i2 * vbDesc.stride);

				// position of vertex
				Vector3f* p0 = (Vector3f*)(v0 + posOffset);
				Vector3f* p1 = (Vector3f*)(v1 + posOffset);
				Vector3f* p2 = (Vector3f*)(v2 + posOffset);

				auto u = *p1 - *p0;
				auto v = *p2 - *p0;
				auto n = CrossProduct(u, v);

				// normal of vertex
				Vector3f* n0 = (Vector3f*)(v0 + normalOffset);
				Vector3f* n1 = (Vector3f*)(v1 + normalOffset);
				Vector3f* n2 = (Vector3f*)(v2 + normalOffset);

				*n0 += n;
				*n1 += n;
				*n2 += n;

				++count[p->i0];
				++count[p->i1];
				++count[p->i2];
			}
		}
		else if (ibDesc.indexType == IBDesc::Index32)
		{
			uint16 triStride = 3 * sizeof(uint32);

			// compute normal
			for (uint32 i = 0; i < triNum; ++i)
			{
				uint32 offset = i * triStride;
				TriIndex32* p = (TriIndex32*)(ib + offset);

				// vertex
				uint8* v0 = vb + (p->i0 * vbDesc.stride);
				uint8* v1 = vb + (p->i1 * vbDesc.stride);
				uint8* v2 = vb + (p->i2 * vbDesc.stride);

				// position of vertex
				Vector3f* p0 = (Vector3f*)(v0 + posOffset);
				Vector3f* p1 = (Vector3f*)(v1 + posOffset);
				Vector3f* p2 = (Vector3f*)(v2 + posOffset);

				auto u = *p1 - *p0;
				auto v = *p2 - *p0;
				auto n = CrossProduct(u, v);

				// normal of vertex
				Vector3f* n0 = (Vector3f*)(v0 + normalOffset);
				Vector3f* n1 = (Vector3f*)(v1 + normalOffset);
				Vector3f* n2 = (Vector3f*)(v2 + normalOffset);

				*n0 += n;
				*n1 += n;
				*n2 += n;

				++count[p->i0];
				++count[p->i1];
				++count[p->i2];
			}
		}

		// average normal
		for (uint32 i = 0; i < vertexNum; ++i)
		{
			uint32 vIndex = i;
			uint32 offset = vIndex * vbDesc.stride;

			Vector3f* n = (Vector3f*)(vb + offset + normalOffset);
			*n /= count[vIndex];
			n->Normalize();
		}

		delete[] count;
	}

	void GeometryGen::TransformVertex(uint32 vertexNum, const VBDesc& vbDesc, int vts)
	{
		uint8* vb = vbDesc.vb + vbDesc.offset;
		uint16 posOffset = vbDesc.voffsets[VT_Position];
		uint16 normalOffset = vbDesc.voffsets[VT_Normal];

		// apply transform to vertex
		for (uint32 i = 0; i < vertexNum; ++i)
		{
			uint32 offset = i * vbDesc.stride;
			Vector3f* p = (Vector3f*)(vb + offset + posOffset);
			*p = *p * vbDesc.m;
		}

		// if vertex normal need transform
		if ((vts & VT_Normal) != 0)
		{
			Matrix44f mInv;
			MatrixInverse(mInv, vbDesc.m);
			Matrix33f m = mInv.Transpose().LTMatrix33();

			for (uint32 i = 0; i < vertexNum; ++i)
			{
				uint32 offset = i * vbDesc.stride;
				Vector3f* n = (Vector3f*)(vb + offset + normalOffset);
				*n = *n * m;
			}
		}
	}

	bool GeometryGen::GenPlane(float width,
		float height,
		uint32 slices,
		uint32 stacks,
		const VBDesc& vbDesc,
		const IBDesc& ibDesc,
		int vts)
	{
		if (!vbDesc.vb || !ibDesc.ib || (vts & VT_Position) == 0)
			return false;
		if (slices < 2 || stacks < 2)
			return false;

		uint8* vb = vbDesc.vb + vbDesc.offset;
		uint16 posOffset = vbDesc.voffsets[VT_Position];
		uint16 normalOffset = vbDesc.voffsets[VT_Normal];
		uint32 vertexNum = stacks * slices;
		uint32 triNum = (stacks - 1) * (slices - 1) * 2;

		// generate position
		float xStep = width / (slices - 1);
		float zStep = height / (stacks - 1);
		float xOrigin = -width / 2;
		float zOrigin = -height / 2;

		for (uint32 j = 0; j < stacks; ++j)
		{
			for (uint32 i = 0; i < slices; ++i)
			{
				uint32 offset = (j * slices + i) * vbDesc.stride;
				Vector3f* p = (Vector3f*)(vb + offset + posOffset);
				p->x = xOrigin + i * xStep;
				p->y = 0.0f;
				p->z = zOrigin + j * zStep;

				if ((vts & VT_Normal) != 0)
				{
					Vector3f* n = (Vector3f*)(vb + offset + normalOffset);
					n->x = 0.0f;
					n->y = 0.0f;
					n->z = 0.0f;
				}
			}
		}

		// generate triangle index
		uint8* ib = ibDesc.ib + ibDesc.offset;
		if (ibDesc.indexType == IBDesc::Index16)
		{
			uint16 triStride = 3 * sizeof(uint16);

			for (uint32 j = 0; j < stacks - 1; ++j)
			{
				for (uint32 i = 0; i < slices - 1; ++i)
				{
					uint32 triIndex = 2 * ((slices - 1) * j + i);

					uint32 offset = triIndex * triStride;
					TriIndex16* p = (TriIndex16*)(ib + offset);
					p->i0 = (uint16)(j * slices + i);
					p->i1 = (uint16)((j + 1) * slices + i + 1);
					p->i2 = (uint16)(j * slices + i + 1);

					offset += triStride;
					p = (TriIndex16*)(ib + offset);
					p->i0 = (uint16)(j * slices + i);
					p->i1 = (uint16)((j + 1) * slices + i);
					p->i2 = (uint16)((j + 1) * slices + i + 1);
				}
			}
		}
		else if (ibDesc.indexType == IBDesc::Index32)
		{
			uint16 triStride = 3 * sizeof(uint32);

			for (uint32 j = 0; j < stacks - 1; ++j)
			{
				for (uint32 i = 0; i < slices - 1; ++i)
				{
					uint32 triIndex = 2 * ((slices - 1) * j + i);

					uint32 offset = triIndex * triStride;
					TriIndex32* p = (TriIndex32*)(ib + offset);
					p->i0 = j * slices + i;
					p->i1 = (j + 1) * slices + i + 1;
					p->i2 = j * slices + i + 1;

					offset += triStride;
					p = (TriIndex32*)(ib + offset);
					p->i0 = j * slices + i;
					p->i1 = (j + 1) * slices + i;
					p->i2 = (j + 1) * slices + i + 1;
				}
			}
		}

		// generate UV
		if ((vts & VT_UV) != 0)
		{
			float uOrigin = 0.0f;
			float vOrigin = 1.0f;
			float uStep = 1.0f / (slices - 1);
			float vStep = -1.0f / (stacks - 1);
			uint16 uvOffset = vbDesc.voffsets[VT_UV];

			for (uint32 j = 0; j < stacks; ++j)
			{
				for (uint32 i = 0; i < slices; ++i)
				{
					uint32 offset = (j * slices + i) * vbDesc.stride;
					Vector2f* uv = (Vector2f*)(vb + offset + uvOffset);
					uv->x = uOrigin + i * uStep;
					uv->y = vOrigin + j * vStep;
				}
			}
		}

		// generate vertex normal
		if ((vts & VT_Normal) != 0)
		{
			GenNormals(vertexNum, triNum, vbDesc, ibDesc);
		}

		// apply transform to vertex
		if (!vbDesc.m.IsIdentity())
		{
			TransformVertex(vertexNum, vbDesc, vts);
		}

		return true;
	}

	bool GeometryGen::GenQuad(float width, float height, float z, const VBDesc& vbDesc, const IBDesc& ibDesc, int vts)
	{
		if (!vbDesc.vb || !ibDesc.ib || (vts & VT_Position) == 0)
			return false;

		uint8* vb = vbDesc.vb + vbDesc.offset;
		uint16 posOffset = vbDesc.voffsets[VT_Position];
		uint16 normalOffset = vbDesc.voffsets[VT_Normal];
		uint32 vertexNum = 4;
		uint32 triNum = 2;

		// generate position
		float xOrigin = -width / 2;
		float yOrigin = -height / 2;

		for (uint32 j = 0; j < 2; ++j)
		{
			for (uint32 i = 0; i < 2; ++i)
			{
				uint32 offset = (j * 2 + i) * vbDesc.stride;
				Vector3f* p = (Vector3f*)(vb + offset + posOffset);
				p->x = xOrigin + width * i;
				p->y = yOrigin + height * j;
				p->z = z;

				if ((vts & VT_Normal) != 0)
				{
					Vector3f* n = (Vector3f*)(vb + offset + normalOffset);
					n->x = 0.0f;
					n->y = 0.0f;
					n->z = 0.0f;
				}
			}
		}

		// generate index
		uint8* ib = ibDesc.ib + ibDesc.offset;
		if (ibDesc.indexType == IBDesc::Index16)
		{
			uint16 n[6] = {
				0, 3, 1,
				0, 2, 3,
			};
			memcpy(ib, n, sizeof(n));
		}
		else if (ibDesc.indexType == IBDesc::Index32)
		{
			uint32 n[6] = {
				0, 3, 1,
				0, 2, 3,
			};
			memcpy(ib, n, sizeof(n));
		}

		// generate UV
		if ((vts & VT_UV) != 0)
		{
			float u[4] = {0.0f, 1.0f, 0.0f, 1.0f};
			float v[4] = {1.0f, 1.0f, 0.0f, 0.0f};
			uint16 uvOffset = vbDesc.voffsets[VT_UV];

			for (uint32 i = 0; i < vertexNum; ++i)
			{
				uint32 offset = i * vbDesc.stride;
				Vector2f* uv = (Vector2f*)(vb + offset + uvOffset);
				uv->x = u[i];
				uv->y = v[i];
			}
		}

		// generate normal
		if ((vts & VT_Normal) != 0)
		{
			GenNormals(vertexNum, triNum, vbDesc, ibDesc);
		}

		// apply transform to vertex
		if (!vbDesc.m.IsIdentity())
		{
			TransformVertex(vertexNum, vbDesc, vts);
		}

		return true;
	}

	bool GeometryGen::GenQuad(float width, float height, float z, const VBDesc& vbDesc, int vts)
	{
		if (!vbDesc.vb || (vts & VT_Position) == 0)
			return false;

		uint8* vb = vbDesc.vb + vbDesc.offset;
		uint16 posOffset = vbDesc.voffsets[VT_Position];
		uint16 normalOffset = vbDesc.voffsets[VT_Normal];
		uint32 vertexNum = 6;

		// generate position
		float xOrigin = -width / 2;
		float yOrigin = -height / 2;

		// 6 points
		Vector3f pos[6] = { 
			// triangle 0
			Vector3f(xOrigin, yOrigin, z),
			Vector3f(xOrigin + width, yOrigin + height, z),
			Vector3f(xOrigin + width, yOrigin, z),
			// triangle 1
			Vector3f(xOrigin, yOrigin, z),
			Vector3f(xOrigin, yOrigin + height, z),
			Vector3f(xOrigin + width, yOrigin + height, z),
		};

		for (uint32 i = 0; i < 6; ++i)
		{
			uint32 offset = i * vbDesc.stride;
			Vector3f* p = (Vector3f*)(vb + offset + posOffset);
			*p = pos[i];

			if ((vts & VT_Normal) != 0)
			{
				Vector3f* n = (Vector3f*)(vb + offset + normalOffset);
				n->x = 0.0f;
				n->y = 0.0f;
				n->z =-1.0f;
			}
		}

		// generate UV
		if ((vts & VT_UV) != 0)
		{
			float u[6] = { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f };
			float v[6] = { 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f };
			uint16 uvOffset = vbDesc.voffsets[VT_UV];

			for (uint32 i = 0; i < 6; ++i)
			{
				uint32 offset = i * vbDesc.stride;
				Vector2f* uv = (Vector2f*)(vb + offset + uvOffset);
				uv->x = u[i];
				uv->y = v[i];
			}
		}

		// apply transform to vertex
		if (!vbDesc.m.IsIdentity())
		{
			TransformVertex(vertexNum, vbDesc, vts);
		}

		return true;
	}

	bool GeometryGen::GenBox(float width, float height, float depth, const VBDesc& vbDesc, const IBDesc& ibDesc, int vts)
	{
		if (!vbDesc.vb || !ibDesc.ib || (vts & VT_Position) == 0)
			return false;

		uint8* vb = vbDesc.vb + vbDesc.offset;
		uint16 posOffset = vbDesc.voffsets[VT_Position];
		uint16 normalOffset = vbDesc.voffsets[VT_Normal];
		uint32 vertexNum = 8;
		uint32 triNum = 12;

		// generate position
		float xOrigin = -width / 2;
		float yOrigin = -height / 2;
		float zOrigin = -depth / 2;

		// base 8 vertices
		for (uint32 k = 0; k < 2; ++k)
		{
			for (uint32 j = 0; j < 2; ++j)
			{
				for (uint32 i = 0; i < 2; ++i)
				{
					uint32 offset = (k * 4 + j * 2 + i) * vbDesc.stride;
					Vector3f* p = (Vector3f*)(vb + offset + posOffset);
					p->x = xOrigin + width * i;
					p->y = yOrigin + height * j;
					p->z = zOrigin + depth * k;

					if ((vts & VT_Normal) != 0)
					{
						Vector3f* n = (Vector3f*)(vb + offset + normalOffset);
						n->x = 0.0f;
						n->y = 0.0f;
						n->z = 0.0f;
					}
				}
			}
		}
		
		// generate triangle index
		if ((vts & VT_UV) != 0)
		{
			// each vertex has shared by 3 Quad, so we need 3 times base position
			vertexNum = 24;
			memcpy(vb + 8 * vbDesc.stride, vb, 8 * vbDesc.stride);
			memcpy(vb + 16 * vbDesc.stride, vb, 8 * vbDesc.stride);

			// generate triangle index
			uint8* ib = ibDesc.ib + ibDesc.offset;
			if (ibDesc.indexType == IBDesc::Index16)
			{
				uint16 n[36] = {
					0, 3, 1, //front
					0, 2, 3,
					4, 7, 6, //back
					4, 5, 7,
					12,10,8, //left
					12,14,10,
					9, 11,15,//right
					9, 15,13,
					18,23,19,//top
					18,22,23,
					20,17,21,//bottom
					20,16,17
				};
				memcpy(ib, n, sizeof(n));
			}
			else if (ibDesc.indexType == IBDesc::Index32)
			{
				uint32 n[36] = {
					0, 3, 1, //front
					0, 2, 3,
					4, 7, 6, //back
					4, 5, 7,
					12,10,8, //left
					12,14,10,
					9, 11,15,//right
					9, 15,13,
					18,23,19,//top
					18,22,23,
					20,17,21,//bottom
					20,16,17
				};
				memcpy(ib, n, sizeof(n));
			}
		}
		else
		{
			uint8* ib = ibDesc.ib + ibDesc.offset;
			if (ibDesc.indexType == IBDesc::Index16)
			{
				uint16 n[36] = {
					0, 3, 1,//front
					0, 2, 3,
					4, 7, 6,//back
					4, 5, 7,
					4, 2, 0,//left
					4, 6, 2,
					1, 3, 7,//right
					1, 7, 5,
					2, 7, 3,//top
					2, 6, 7,
					4, 1, 5,//bottom
					4, 0, 1
				};
				memcpy(ib, n, sizeof(n));
			}
			else if (ibDesc.indexType == IBDesc::Index32)
			{
				uint32 n[36] = {
					0, 3, 1,//front
					0, 2, 3,
					4, 7, 6,//back
					4, 5, 7,
					4, 2, 0,//left
					4, 6, 2,
					1, 3, 7,//right
					1, 7, 5,
					2, 7, 3,//top
					2, 6, 7,
					4, 1, 5,//bottom
					4, 0, 1
				};
				memcpy(ib, n, sizeof(n));
			}
		}

		// generate UV
		if ((vts & VT_UV) != 0)
		{
			//             +     front         +         back          +                   left & right               +                   top & bottom                +
			float u[24] = {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f};
			float v[24] = {1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f};
			uint16 uvOffset = vbDesc.voffsets[VT_UV];

			for (uint32 i = 0; i < vertexNum; ++i)
			{
				uint32 offset = i * vbDesc.stride;
				Vector2f* uv = (Vector2f*)(vb + offset + uvOffset);
				uv->x = u[i];
				uv->y = v[i];
			}
		}

		// generate normal
		if ((vts & VT_Normal) != 0)
		{
			GenNormals(vertexNum, triNum, vbDesc, ibDesc);
		}

		// apply transform to vertex
		if (!vbDesc.m.IsIdentity())
		{
			TransformVertex(vertexNum, vbDesc, vts);
		}

		return true;
	}

	bool GeometryGen::GenSkyBox(float width, float height, float depth, const VBDesc& vbDesc, const IBDesc& ibDesc, int vts)
	{
		if (!vbDesc.vb || !ibDesc.ib || (vts & VT_Position) == 0)
			return false;

		uint8* vb = vbDesc.vb + vbDesc.offset;
		uint16 posOffset = vbDesc.voffsets[VT_Position];
		uint16 normalOffset = vbDesc.voffsets[VT_Normal];
		uint32 vertexNum = 14;
		uint32 triNum = 12;

		// generate position
		float xOrigin = -width / 2;
		float yOrigin = -height / 2;
		float zOrigin = -depth / 2;

		// base 8 vertices
		for (uint32 k = 0; k < 2; ++k)
		{
			for (uint32 j = 0; j < 2; ++j)
			{
				for (uint32 i = 0; i < 2; ++i)
				{
					uint32 offset = (k * 4 + j * 2 + i) * vbDesc.stride;
					Vector3f* p = (Vector3f*)(vb + offset + posOffset);
					p->x = xOrigin + width * i;
					p->y = yOrigin + height * j;
					p->z = zOrigin + depth * k;

					if ((vts & VT_Normal) != 0)
					{
						Vector3f* n = (Vector3f*)(vb + offset + normalOffset);
						n->x = 0.0f;
						n->y = 0.0f;
						n->z = 0.0f;
					}
				}
			}
		}
		// extends to 14 vertices refer to geometry of box
		memcpy(vb + 8 * vbDesc.stride, vb + 4 * vbDesc.stride, 4 * vbDesc.stride);
		memcpy(vb + 12 * vbDesc.stride, vb + 4 * vbDesc.stride, vbDesc.stride);
		memcpy(vb + 13 * vbDesc.stride, vb + 6 * vbDesc.stride, vbDesc.stride);

		// generate triangle index
		uint8* ib = ibDesc.ib + ibDesc.offset;
		if (ibDesc.indexType == IBDesc::Index16)
		{
			uint16 n[36] = {
				0, 3, 1, //front
				0, 2, 3,
				8, 11,10,//back
				8, 9, 11,
				12,2, 0, //left
				12,13,2,
				1, 3, 11,//right
				1, 11,9,
				2, 7, 3, //top
				2, 6, 7,
				4, 1, 5, //bottom
				4, 0, 1
			};
			memcpy(ib, n, sizeof(n));
		}
		else if (ibDesc.indexType == IBDesc::Index32)
		{
			uint32 n[36] = {
				0, 3, 1, //front
				0, 2, 3,
				8, 11,10,//back
				8, 9, 11,
				12,2, 0, //left
				12,13,2,
				1, 3, 11,//right
				1, 11,9,
				2, 7, 3, //top
				2, 6, 7,
				4, 1, 5, //bottom
				4, 0, 1
			};
			memcpy(ib, n, sizeof(n));
		}

		// generate UV
		if ((vts & VT_UV) != 0)
		{
			// UV compute as:
			//     +--+ 
			//  +--+--+--+--+
			//  +--+--+--+--+
			//     +--+
			float u[14] = { 0.25f, 0.50f, 0.25f, 0.50f, 0.25f, 0.50f, 0.25f, 0.50f, 1.0f, 0.75f, 1.0f, 0.75f, 0.0f, 0.0f };
			float v[14] = { 2 / 3.0f, 2 / 3.0f, 1 / 3.0f, 1 / 3.0f, 1.0f, 1.0f, 0.00f, 0.00f, 2 / 3.0f, 2 / 3.0f, 1 / 3.0f, 1 / 3.0f, 2 / 3.0f, 1 / 3.0f };
			uint16 uvOffset = vbDesc.voffsets[VT_UV];
			for (uint32 i = 0; i < vertexNum; ++i)
			{
				uint32 offset = i * vbDesc.stride;
				Vector2f* uv = (Vector2f*)(vb + offset + uvOffset);
				uv->x = u[i];
				uv->y = v[i];
			}
		}

		// generate normal
		if ((vts & VT_Normal) != 0)
		{
			GenNormals(vertexNum, triNum, vbDesc, ibDesc);
		}

		// apply transform to vertex
		if (!vbDesc.m.IsIdentity())
		{
			TransformVertex(vertexNum, vbDesc, vts);
		}
		return true;
	}
}